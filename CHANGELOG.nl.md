# Wijzigingen

## 0.2.2 - 2026-10-19
- Opgelost: **Beeld ondersteboven draaien** liet de camera stoppen (het beeld schoof één rij over de rand van de sensor).
  Na de update werkt de camera met de instelling aan en uit.

## 0.2.1 - 2026-10-19
- Nieuw: **Camera status** bij het apparaat van het scherm in Home Assistant (onder Diagnose). Hij zegt of de camera
  werkt, en zo niet, wat de camera meldde terwijl het scherm opstartte. Stuur hem mee als je een probleem meldt.

## 0.2.0 - 2026-10-19
- Het beeld regelt nu zelf helderheid en witbalans: de camera heeft een eigen beeldafstemming, en de sensordriver zet
  belichting en versterking in de eigen registers van de OV02C10.
- Nieuwe instellingen: beeld spiegelen en ondersteboven draaien.
- De camera heet **Camera** bij het apparaat van het scherm in Home Assistant.
- Nog niet op een echt bord geprobeerd: meld graag wat je ziet.

## 0.1.0 - 2026-10-18
- Eerste camera-implementatie voor de OV02C10 op de JC8012P4A1 v3.
- Gebruikt `touch_bus` voor SCCB en de MIPI-CSI/JPEG-pijplijn van de ESP32-P4.
