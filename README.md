# JC8012 camera

See the built-in camera of the Guition JC8012P4A1 v3 (the 10.1-inch screen) in Home Assistant. The screen becomes a
camera of its device there: a picture on a dashboard, a stream when you open it, and anything Home Assistant does with a
camera (a snapshot in an automation, a recording).

The picture is made on the screen in hardware, only while Home Assistant asks for one. While it does, a camera shows in
the screen's top bar, so whoever stands in front of it knows.

The camera also gives the screen a **light level** and a **motion** sensor, worked out on the screen itself: dim the
screen in the evening, switch a light when someone comes in, and wake the screen when someone walks up to it.

> **Beta.** The camera works on a JC8012P4A1 v3; its colours, the light level and motion are still being tuned. Please
> [open an issue](https://github.com/Heronimonimo/tessera-jc8012-camera/issues) with what you see (see "Trying it" below).

## What you need

- A Guition JC8012P4A1 **v3** running Tessera (board `jc8012p4a1v3`). The first JC8012P4A1 and the V2 are not
  supported yet.
- Nothing else in Home Assistant: the camera comes with the screen's device in the ESPHome integration.

## Set up

1. In Tessera, open **Plugins**, choose **Add with a link** and paste
   `https://github.com/Heronimonimo/tessera-jc8012-camera`.
2. Choose **JC8012 camera** and tick your JC8012P4A1 v3 screen. The screen builds once with it, which takes a while.
3. In Home Assistant, open the screen's device (Settings, Devices): its camera is there as **Camera**. Put it on a
   dashboard with a Picture entity or a Picture glance card.
4. If the picture is mirrored or upside down, switch **Mirror the picture** or **Turn the picture upside down** in the
   plugin's settings on the screen's Plugins tab in Tessera (or the **Camera mirror** and **Camera flip** switches on the
   device in Home Assistant). The next picture comes out turned.
5. Optional: add **Camera in use** to a page's top bar (Top bar, Add, From plugins). It shows while pictures leave the
   screen: "Live" for a stream, "Camera" for a picture now and then. **Camera senses motion** shows an eye while the
   camera measures light and motion.
6. **Light level** and **Motion** are on the screen's device in Home Assistant. Motion wakes the screen from standby;
   switch **Wake the screen on motion** off in the plugin's settings if you don't want that, or **Light level and
   motion** off to stop both.

## Light level and motion

- **Light level** is an estimate in lux from the picture's brightness, the exposure time and the sensor's gain. It is
  right in proportion (twice the light reads about twice the lux), not exact: pick the thresholds for your automations
  by looking at what it reads in your room. It updates every ten seconds, and at once when a light goes on or off.
- **Motion** compares the picture in a coarse grid twice a second and turns on when part of it changes, for at least ten
  seconds. A change of the whole picture (a light switched on, the camera adjusting its exposure) is not motion.
- While this is on the camera runs all the time, at two frames a second. No picture is encoded or sent for it: only the
  two numbers leave the screen.

## Privacy

Nothing but the light level and motion leaves the screen until Home Assistant asks for a picture, and every picture goes
only to your Home Assistant. While light level and motion are on, the camera looks all the time, but what it sees stays
on the screen; add **Camera senses motion** to the top bar to show that.
Everyone who can open the screen's device or a dashboard with its camera can see through it. To stop that for a while,
disable the camera entity in Home Assistant; to stop it for good, remove the plugin from the screen.

## Trying it

The first pictures from a new board are what this plugin needs most. After the build, open the screen's device in Home
Assistant and look at **Camera status** (under Diagnostic). The camera starts before Home Assistant connects, so this is
where what it said while it started ends up:

- `Detected Camera sensor PID=0x5602`: the camera answers.
- No `failed to get configuration to initialize ISP controller`: the image tuning for this camera is in use, so the
  picture adjusts its brightness and colours by itself.
- `Failed: ...`: the camera did not start, and the rest says why. Copy it into the issue.

Then take a picture in a bright room and one in a dim room, and say in an issue whether the colours look right, whether
the picture is the right way round, and whether the screen stays smooth while you watch the stream.

## What it does not do yet

- Colours start from a general tuning, not one measured for this camera: they may be a little off until it has been
  tuned on a real board.
- The picture is 1288 x 728 at up to two a second. The sensor can do 1920 x 1080 over two lanes; that waits for a test on
  the board.

## For makers

The camera is ESPHome's camera with id `ts_camera` (the feature `camera`), so another plugin can build on it.

- `components/esp_video_camera/`: the camera driver of Tessera's Screen camera plugin (`esp_video_camera`, from
  [n-IA-hane/esphome-esp-video-camera](https://github.com/n-IA-hane/esphome-esp-video-camera), after ESPHome pull request
  esphome/esphome#16944), with an OV02C10 sensor driver beside it (`ov02c10_sensor.c`): Espressif's camera library has
  none. The driver sets exposure and gain on the sensor's own registers, so the ISP pipeline's auto exposure works, and
  mirrors and flips without changing the colour order.
- `components/esp_video_camera/build_stubs/ov02c10_ipa/`: the image tuning (`ov02c10.json`, started from Espressif's
  SC2336 tuning with a wider white balance search, black level 16, the rev3 gamma and a colour matrix halfway to
  neutral) and an empty ESP-IDF component whose `project_include.cmake` hands it
  to `esp_ipa`.
- `components/jc8012_camera/`: the plugin itself: the top bar items, the camera status, and the light level and motion,
  read from the camera's processed frames (esp_video_camera's `RawVideoFrameConsumer`) in a 32 x 18 grid. Motion wakes
  the screen through Tessera's `wake_display` script (core YAML, not the plugin API).
- The sensor answers on the board's touch bus (`touch_bus`, GPIO7 and GPIO8) at `0x36`, has its own clock and no reset or
  power-down pin.
- `.github/workflows/build.yml` builds the whole screen's firmware with this plugin on every push.

Licences and where every file comes from: `components/esp_video_camera/NOTICE` and `LICENSE`.
