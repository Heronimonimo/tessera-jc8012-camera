# Wijzigingen

## 0.2.0 - 2026-10-19
- Het beeld regelt nu zelf helderheid en witbalans: de camera heeft een eigen beeldafstemming, en de sensordriver zet
  belichting en versterking in de eigen registers van de OV02C10.
- Nieuwe instellingen: beeld spiegelen en ondersteboven draaien.
- De camera heet **Camera** bij het apparaat van het scherm in Home Assistant.
- Nog niet op een echt bord geprobeerd: meld graag wat je ziet.

## 0.1.0 - 2026-10-18
- Eerste camera-implementatie voor de OV02C10 op de JC8012P4A1 v3.
- Gebruikt `touch_bus` voor SCCB en de MIPI-CSI/JPEG-pijplijn van de ESP32-P4.
