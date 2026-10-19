#include "jc8012_camera.h"

#include <cstdio>
#include <cstring>

#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#ifdef USE_LOGGER
#include "esphome/components/logger/logger.h"
#endif

namespace esphome::jc8012_camera {

// Tessera's camera icon.
static constexpr uint32_t ICON_CAMERA = 0xF07AE;   // cctv
// A picture keeps the item in the top bar this long: Home Assistant's dashboards ask for a new one every ten seconds.
static constexpr uint32_t PICTURE_SHOWN_MS = 12000;
// A stream is live while its pictures come; at five a second, a gap this long means it ended.
static constexpr uint32_t STREAM_QUIET_MS = 3000;
static const char *const TAG = "jc8012_camera";

// The parts of the camera that speak in ESP-IDF's log (forwarded by ESPHome under the tag "esp-idf"): the sensor
// driver, Espressif's video pipeline, its image processor and the camera bus.
static const char *const CAMERA_TAGS[] = {"ov02c10", "esp_video", "isp", "ipa", "cam_sensor", "sccb", "csi", "cam_ctlr"};

void JC8012Camera::listen_to_log() {
#if defined(USE_LOGGER) && defined(USE_LOG_LISTENERS)
  if (logger::global_logger != nullptr) {
    logger::global_logger->add_log_callback(
        this, [](void *self, uint8_t level, const char *tag, const char *message, size_t length) {
          static_cast<JC8012Camera *>(self)->on_log_(level, tag, message, length);
        });
  }
#endif
}

void JC8012Camera::setup() {
  if (camera_ != nullptr) camera_->add_listener(this);
  add_bar_item("watching", [this]() {
    tessera::BarItem item;
    if (watching()) {
      item.shown = true;
      item.icon = ICON_CAMERA;
      // A stream lights the camera up in red, as a camera that records; a picture now and then keeps the bar's grey.
      item.tone = live() ? tessera::Tone::ALERT : tessera::Tone::NORMAL;
      item.text = text(live() ? "live" : "picture");
    }
    return item;
  });
  publish_status_();
}

// Whether ESP-IDF wrote this line at this level: its letter before the time, "E (1234)". The message need not end in a
// NUL, so this looks only within its length.
static bool idf_level(const char *message, size_t length, char letter) {
  for (size_t i = 0; i + 2 < length; i++) {
    if (message[i] == letter && message[i + 1] == ' ' && message[i + 2] == '(') return true;
  }
  return false;
}

// A line of the log without its colours and its header: "[E][esp_video_camera:445]: esp_video_init() failed: ..."
// becomes "esp_video_init() failed: ...", and ESP-IDF's "E (1234) ov02c10: Get sensor ID failed" becomes
// "ov02c10: Get sensor ID failed".
static size_t clean_line(const char *message, size_t length, char *out, size_t room) {
  size_t n = 0;
  for (size_t i = 0; i < length && n + 1 < room; i++) {
    if (message[i] == '\033') {   // a colour: ESC [ ... m
      while (i < length && message[i] != 'm') i++;
      continue;
    }
    if (message[i] == '\r' || message[i] == '\n') continue;
    out[n++] = message[i];
  }
  out[n] = '\0';
  const char *start = out;
  if (const char *header = strstr(start, "]: ")) start = header + 3;
  // ESP-IDF's own level and time: "E (1234) ".
  if ((start[0] == 'E' || start[0] == 'W' || start[0] == 'I') && start[1] == ' ' && start[2] == '(') {
    if (const char *after = strstr(start, ") ")) start = after + 2;
  }
  n = strlen(start);
  memmove(out, start, n + 1);
  return n;
}

void JC8012Camera::on_log_(uint8_t level, const char *tag, const char *message, size_t length) {
  if (said_len_ + 8 >= sizeof(said_)) return;   // full: the first lines are the ones that tell why
  if (strcmp(tag, "esp_video_camera") == 0) {
    if (level <= ESPHOME_LOG_LEVEL_WARN) keep_line_(message, length);
    return;
  }
  if (strcmp(tag, "esp-idf") != 0) return;
  char line[160];
  size_t n = clean_line(message, length, line, sizeof(line));
  if (n == 0) return;
  bool camera = false;
  for (const char *name : CAMERA_TAGS) {
    if (strncmp(line, name, strlen(name)) == 0) camera = true;
  }
  if (!camera) return;
  bool trouble = idf_level(message, length, 'E') || idf_level(message, length, 'W');
  bool found = strstr(line, "Detected Camera sensor") != nullptr;
  if (trouble || found) keep_line_(message, length);
}

void JC8012Camera::keep_line_(const char *message, size_t length) {
  char line[160];
  size_t n = clean_line(message, length, line, sizeof(line));
  if (n == 0) return;
  if (said_len_ > 0 && said_len_ + 3 < sizeof(said_)) {
    memcpy(said_ + said_len_, " | ", 3);
    said_len_ += 3;
  }
  size_t room = sizeof(said_) - 1 - said_len_;
  if (n > room) n = room;
  memcpy(said_ + said_len_, line, n);
  said_len_ += n;
  said_[said_len_] = '\0';
}

void JC8012Camera::publish_status_() {
#ifdef USE_TEXT_SENSOR
  if (status_ == nullptr || camera_ == nullptr) return;
  char status[sizeof(shown_status_)];
  const char *state = camera_->is_failed() ? "Failed" : pictured_ ? "Working" : "Started, no picture asked for yet";
  if (said_len_ > 0) {
    snprintf(status, sizeof(status), "%s: %s", state, said_);
  } else if (camera_->is_failed()) {
    snprintf(status, sizeof(status), "%s, without a reason in the log", state);
  } else {
    snprintf(status, sizeof(status), "%s", state);
  }
  if (strcmp(status, shown_status_) == 0) return;
  memcpy(shown_status_, status, sizeof(shown_status_));
  if (camera_->is_failed()) ESP_LOGE(TAG, "Camera status: %s", status);
  status_->publish_state(status);
#endif
}

bool JC8012Camera::watching() const {
  return last_picture_ != 0 && millis() - last_picture_ < PICTURE_SHOWN_MS;
}

bool JC8012Camera::live() const {
  return streaming_ && millis() - last_stream_ < STREAM_QUIET_MS;
}

void JC8012Camera::on_camera_image(const std::shared_ptr<camera::CameraImage> &image) {
  last_picture_ = millis();
  if (last_picture_ == 0) last_picture_ = 1;
  last_stream_ = last_picture_;
  pictured_ = true;
  if (!shown_) {
    shown_ = true;
    tessera::refresh();
  }
}

void JC8012Camera::on_stream_start() {
  if (!streaming_) {
    ESP_LOGD(TAG, "Stream started");
  }
  streaming_ = true;
  last_stream_ = millis();
}

void JC8012Camera::on_stream_stop() {
  if (streaming_) {
    ESP_LOGD(TAG, "Stream stopped");
  }
  streaming_ = false;
  tessera::refresh();
}

void JC8012Camera::on_interval(uint32_t now_ms) {
  // The item goes as soon as the last picture is old, not at the bar's next look; a stream that went quiet is no
  // longer live.
  if (shown_ && !watching()) {
    shown_ = false;
    tessera::refresh();
  }
  if (streaming_ && !live()) {
    ESP_LOGD(TAG, "Stream went quiet");
    streaming_ = false;
    tessera::refresh();
  }
  publish_status_();
}

}  // namespace esphome::jc8012_camera
