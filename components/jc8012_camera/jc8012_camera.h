#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/components/camera/camera.h"
#include "esphome/components/smart_display/plugin_api.h"
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif

namespace esphome::jc8012_camera {

// The plugin: an item for the top bar that shows while pictures leave the screen's camera, in red while it streams. It listens to the camera as
// ESPHome's API does (camera::CameraListener), on the main loop: a picture, or a stream that starts or stops.
// And the camera's status for Home Assistant: the camera starts before Home Assistant connects, so what it said while it
// started (the sensor found or not, the image pipeline up or not) is kept from the log and shown as a text.
class JC8012Camera : public Component, public tessera::Plugin, public camera::CameraListener {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::LATE; }
  void on_interval(uint32_t now_ms) override;

  void set_camera(camera::Camera *c) { camera_ = c; }
#ifdef USE_TEXT_SENSOR
  void set_status(text_sensor::TextSensor *s) { status_ = s; }
#endif
  // Called while the firmware is made, before any component starts, so the camera's own start is in the log it keeps.
  void listen_to_log();

  void on_camera_image(const std::shared_ptr<camera::CameraImage> &image) override;
  void on_stream_start() override;
  void on_stream_stop() override;

  // Whether someone is watching: a picture left the screen a moment ago. Live: a stream runs and its pictures still
  // come, so a stream that ends without a word from the API (a viewer that went away) goes quiet by itself.
  bool watching() const;
  bool live() const;

 protected:
  void on_log_(uint8_t level, const char *tag, const char *message, size_t length);
  void keep_line_(const char *message, size_t length);
  void publish_status_();

  camera::Camera *camera_{};
#ifdef USE_TEXT_SENSOR
  text_sensor::TextSensor *status_{};
#endif
  bool streaming_{false};
  bool shown_{false};          // what the top bar shows now, so a change draws it at once
  bool pictured_{false};       // a picture left the camera since it started
  uint32_t last_picture_{0};   // millis() of the last picture, 0 before the first
  uint32_t last_stream_{0};    // millis() of the last sign of a stream: a request for it or one of its pictures
  // What the camera said while it started: the sensor's name and the first errors and warnings, " | " between them.
  char said_[200]{};
  size_t said_len_{0};
  char shown_status_[220]{};   // the status Home Assistant has, so only a change is sent
};

}  // namespace esphome::jc8012_camera
