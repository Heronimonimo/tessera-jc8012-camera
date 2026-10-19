# JC8012-camera

Lokale Tessera-plugin voor de ingebouwde OV02C10-camera van de Guition
JC8012P4A1 v3 (`jc8012p4a1v3`). De camera verschijnt als native ESPHome-camera
in Home Assistant. De bovenbalk kan tonen wanneer een foto of stream actief is.

## Hardware

- sensor: OV02C10, SCCB-adres `0x36`, product-id `0x5602`;
- interface: het bord voert twee MIPI-CSI-datalanes uit; het bewezen
  `1288x728`-sensorprofiel gebruikt lane 0;
- SCCB: gedeelde Tessera-bus `touch_bus`, SDA GPIO7 en SCL GPIO8;
- geen externe XCLK-, reset- of power-downpin volgens het bordschema;
- vast sensorformaat: `1288x728`, maximaal 2 fps in deze plugin.

## Installeren

Kopieer de hele map `jc8012_camera` naar:

```text
/config/tessera-plugins/jc8012_camera/
```

Open daarna Tessera, kies **Plugins**, installeer **JC8012-camera** op het
scherm en laat Tessera bouwen. Tessera beheert `woonkamer.plugins.yaml`; voeg
daarom geen tweede handmatige include voor dezelfde plugin toe.

Voor een volledig lokale ontwikkelopstelling kan `woonkamer.plugins.yaml`
rechtstreeks bevatten:

```yaml
packages:
  jc8012_camera: !include tessera-plugins/jc8012_camera/plugin.yaml

external_components:
  - source:
      type: local
      path: tessera-plugins/jc8012_camera/components
    components: [esp_video_camera, jc8012_camera]
```

Pas het relatieve pad aan als `tessera-plugins` niet naast de hoofdconfiguratie
staat. Gebruik deze handmatige vorm alleen zonder Tessera's pluginbeheer.

## Beperkingen

Experimenteel. Buildvalidatie vervangt geen test op het bord. De OV02C10-driver
is afkomstig van `sullb/esphome-p4-csi-camera`; kleurkalibratie, langdurige
stabiliteit en werkelijke framerate moeten op de hardware worden gecontroleerd.

## Herkomst

De ESPHome/esp-video-basis komt uit Tessera's `screen_camera`-plugin. De lokale
OV02C10-driver en sensortabellen komen uit `sullb/esphome-p4-csi-camera`. Zie
`components/esp_video_camera/NOTICE` en `LICENSE`.
