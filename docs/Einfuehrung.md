---
layout: page
title: Linienroboter - Einführung
lang: de
lang-ref: index
---

## Einführung

In der Einführung sollst du dich zunächst mit der Arduino IDE und dem ESP 32 vertraut machen sowie elektro- sowie informationstechnische Grundlagen erlernen, um später den Linienroboter problemlos aufbauen zu können.
Zuerst solltest mithilfe der [Materialliste](Material.pdf) sichergehen, alle nötigen Bauteile zusammensuchen. Die Bauteile für diese Einleitung sind nicht in dem Bauteilsatz für den Linienroboter enthalten. Bei der Durchführung der Einführung können dir [dieses Diagramm von den Pins des ESP32](00-Pinout-ESP32-DEVKIT-V1.pdf) und [dieser Arduino Cheat Sheet](00-CheatSheet-Arduino.pdf) helfen.

{: .info}
Diese und die kommenden Anleitungen sollen dich bei deinem Praktikum unterstützen. Falls es zwischendurch Fragen geben sollte oder du anhand der Anleitung nicht weiterkommst, kannst du uns jederzeit [ansprechen](./index.md/#ansprechpartner)!

### Grundsätzliches
Wir beginnen mit dem Mikrocontroller, der den Roboter am Ende steuern wird. Das ist ein ESP32-Entwicklungsboard.
<img scr="img/ESP32.png" alt="Abbildung eines ESP32 DEV Board"></img>
Der eigentliche Mikrocontroller ist das silberne Quadrat in der oberen Hälfte. Der Rest sind bspw. eine WLAN-Antenne und die Stromversorgung des Mikrocontroller. Solche Mikrocontroller können z.B. für die Auswertung von Sensordaten, die Datenaufnahme oder kleinere Berechnungen eingesetzt werden.
Die Metallstifte an der Seite sind die Anschlüsse für verschiedene Schnittstellen, die der Mikrocontroller unterstützt. Wir nennen sie „Pins“. Welcher Metallstift welche Schnittstelle [unterstützt](00-Pinout-ESP32-DEVKIT-V1.pdf) unterscheidet sich, je nachdem welches Entwicklungsboard man verwendet. Wenn du also selbst zu Hause mit einem ESP32-Board arbeiten möchtest, musst du immer die passende Pin-Belegung („Pinout“) suchen.
Im Praktikum wird vorgegeben, welcher Pin verwendet werden soll.
Der ESP32 kann über verschiedene sogenannte Bus-Schnittstellen kommunizieren, die für uns aber nicht relevant sind. Wir nutzen vor allem die **GPIOs**. Das steht für **G**eneral **P**urpose **I**nput **O**utput, also allgemeine Eingabe und Ausgabe.
Eingaben sind z.B. Werte, die Sensoren aufnehmen und die wir im Programm verarbeiten, Ausgaben sind Signale, um bspw. die Motoren ein- und auszuschalten.
