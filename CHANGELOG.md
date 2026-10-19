# Changelog

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
