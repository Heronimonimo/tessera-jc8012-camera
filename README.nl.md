# JC8012-camera

Bekijk de ingebouwde camera van de Guition JC8012P4A1 v3 (het 10,1-inch scherm) in Home Assistant. Het scherm wordt daar
een camera van zijn apparaat: een beeld op een dashboard, een stream als je het opent, en alles wat Home Assistant met
een camera doet (een momentopname in een automatisering, een opname).

Het beeld wordt op het scherm in hardware gemaakt, alleen zolang Home Assistant erom vraagt. Zolang dat zo is, staat er
een camera in de bovenbalk van het scherm, zodat wie ervoor staat het weet.

> **Bèta.** Deze plugin bouwt, maar is nog niet op een echte JC8012P4A1 v3 geprobeerd. Probeer je hem, open dan
> [een issue](https://github.com/Heronimonimo/tessera-jc8012-camera/issues) met wat je ziet (zie "Uitproberen" hieronder).

## Wat je nodig hebt

- Een Guition JC8012P4A1 **v3** met Tessera (bord `jc8012p4a1v3`). De eerste JC8012P4A1 en de V2 worden nog niet
  ondersteund.
- Verder niets in Home Assistant: de camera komt mee met het apparaat van het scherm in de ESPHome-integratie.

## Instellen

1. Open in Tessera **Plugins**, kies **Toevoegen met een link** en plak
   `https://github.com/Heronimonimo/tessera-jc8012-camera`.
2. Kies **JC8012-camera** en vink je JC8012P4A1 v3 aan. Het scherm bouwt één keer opnieuw, dat duurt even.
3. Open in Home Assistant het apparaat van het scherm (Instellingen, Apparaten): de camera staat er als **Camera**. Zet
   hem op een dashboard met een Afbeelding-entiteit- of Afbeelding-overzicht-kaart.
4. Is het beeld gespiegeld of ondersteboven, zet dan **Beeld spiegelen** of **Beeld ondersteboven draaien** aan bij de
   instellingen van de plugin op het tabblad Plugins van het scherm in Tessera (of de schakelaars **Camera mirror** en
   **Camera flip** bij het apparaat in Home Assistant). Het volgende beeld komt gedraaid binnen.
5. Optioneel: zet **Camera in gebruik** in de bovenbalk van een pagina (Bovenbalk, Toevoegen, Uit plugins). Hij staat er
   zolang er beelden van het scherm gaan: "Live" bij een stream, "Camera" bij af en toe een beeld.

## Privacy

Er gaat niets van het scherm af tot Home Assistant om een beeld vraagt, en elk beeld gaat alleen naar jouw Home
Assistant. Iedereen die het apparaat van het scherm of een dashboard met de camera kan openen, kan erdoor kijken. Wil je
dat even niet, schakel dan de camera-entiteit in Home Assistant uit; voorgoed, verwijder dan de plugin van het scherm.

## Uitproberen

De eerste beelden van een echt bord zijn wat deze plugin het hardst nodig heeft. Open na het bouwen het apparaat van
het scherm in Home Assistant en kijk bij **Camera status** (onder Diagnose). De camera start voordat Home Assistant
verbinding maakt, dus hier komt terecht wat hij tijdens het opstarten meldde:

- `Detected Camera sensor PID=0x5602`: de camera antwoordt.
- Geen `failed to get configuration to initialize ISP controller`: de beeldafstemming voor deze camera wordt gebruikt,
  dus het beeld regelt zelf helderheid en kleur.
- `Failed: ...`: de camera is niet gestart, en de rest zegt waarom. Kopieer het in het issue.

Maak daarna een beeld in een lichte en een in een donkere kamer, en meld in een issue of de kleuren kloppen, of het beeld
goed om staat en of het scherm soepel blijft terwijl je de stream bekijkt.

## Wat nog niet kan

- De kleuren beginnen bij een algemene afstemming, niet bij een die voor deze camera is gemeten: ze kunnen wat afwijken
  tot hij op een echt bord is afgestemd.
- Het beeld is 1288 x 728, hooguit twee per seconde. De sensor kan 1920 x 1080 over twee lanes; dat wacht op een test op
  het bord.

## Voor makers

Zie `README.md`, "For makers".
