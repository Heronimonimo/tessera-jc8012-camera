# JC8012 camera

Local Tessera plugin for the built-in OV02C10 camera on the Guition
JC8012P4A1 v3 (`jc8012p4a1v3`). It exposes a native ESPHome camera to Home
Assistant and supplies a top-bar activity indicator.

## Set up

Install the complete folder as `/config/tessera-plugins/jc8012_camera/`, then
select **JC8012 camera** in Tessera's Plugins page. Tessera manages the generated
`woonkamer.plugins.yaml` file.

After the build, open the screen device in Home Assistant. Add its
**JC8012 camera** entity to a Picture entity or Picture glance card. The
optional **Camera in use** bar item shows when pictures leave the screen.

## Hardware

Hardware mapping: the board routes two MIPI-CSI lanes, while the proven
`1288x728` sensor profile uses lane 0. The OV02C10 is at SCCB address `0x36` on
the existing `touch_bus` (GPIO7 SDA, GPIO8 SCL), with no external XCLK/reset/
power-down GPIO. The plugin requests at most 2 fps.

This plugin is experimental and requires validation on the physical board.
See `README.nl.md` for configuration details and
`components/esp_video_camera/NOTICE` for source attribution.
