# Changelog

## 0.3.1 - 2026-10-19
- The picture is no longer a mirror image by default: left is left, as a camera on the wall should show it. If you had
  switched **Mirror the picture** on to correct it, switch it off again.
- New: **Camera activity** on the screen's device in Home Assistant: Live, Picture, Sensing, Idle or Failed, as the top
  bar shows it.

## 0.3.0 - 2026-10-19
- New: **Light level** (lux, an estimate) and **Motion** on the screen's device in Home Assistant, worked out from the
  camera on the screen itself: no picture leaves the screen for them. Use the light level to dim the screen in the
  evening, motion for automations.
- New: motion wakes the screen from standby (setting **Wake the screen on motion**).
- Both are on after the update. Turn off **Light level and motion** in the plugin's settings to stop them; the camera
  then only runs while Home Assistant asks for pictures, as before.
- Optional top bar item **Camera senses motion**: an eye while the camera senses.

## 0.2.5 - 2026-10-19
- Sharper pictures with cleaner colours: they were compressed far too much (JPEG quality 12 of 100, now 80). Each
  picture is larger, about 150 to 300 KB.

## 0.2.4 - 2026-10-19
- Truer colours: whites and walls no longer look green-blue. Take a picture again to compare.

## 0.2.3 - 2026-10-19
- Better picture: deeper blacks and more contrast instead of a grey haze, less of a teal cast in the shadows, and
  calmer colours. Take a picture again in the same room to compare.

## 0.2.2 - 2026-10-19
- Fixed: **Turn the picture upside down** stopped the camera (the picture was moved one row past the sensor's edge).
  After the update the camera works with it on or off.

## 0.2.1 - 2026-10-19
- New: **Camera status** on the screen's device in Home Assistant (under Diagnostic). It says whether the camera works,
  and when it does not, what the camera reported while the screen started. Please send it along when you report a
  problem.

## 0.2.0 - 2026-10-19
- The picture now sets its own brightness and white balance: the camera has image tuning of its own, and the sensor
  driver sets exposure and gain on the OV02C10's own registers.
- New settings: mirror the picture and turn it upside down.
- The camera is called **Camera** on the screen's device in Home Assistant.
- Not yet tried on a real board: please report what you see.

## 0.1.0 - 2026-10-18
- Initial JC8012P4A1 v3 OV02C10 camera implementation.
- Reuses `touch_bus` for SCCB and the ESP32-P4 MIPI-CSI/JPEG pipeline.
