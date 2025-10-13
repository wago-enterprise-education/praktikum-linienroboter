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

<img scr="img/ESP32.png" alt="Abbildung eines ESP32 DEV Board">

Der eigentliche Mikrocontroller ist das silberne Quadrat in der oberen Hälfte. Der Rest sind bspw. eine WLAN-Antenne und die Stromversorgung des Mikrocontroller. Solche Mikrocontroller können z.B. für die Auswertung von Sensordaten, die Datenaufnahme oder kleinere Berechnungen eingesetzt werden.
Die Metallstifte an der Seite sind die Anschlüsse für verschiedene Schnittstellen, die der Mikrocontroller unterstützt. Wir nennen sie „Pins“. Welcher Metallstift welche Schnittstelle [unterstützt](00-Pinout-ESP32-DEVKIT-V1.pdf) unterscheidet sich, je nachdem welches Entwicklungsboard man verwendet. Wenn du also selbst zu Hause mit einem ESP32-Board arbeiten möchtest, musst du immer die passende Pin-Belegung („Pinout“) suchen.
Im Praktikum wird vorgegeben, welcher Pin verwendet werden soll.
Der ESP32 kann über verschiedene sogenannte Bus-Schnittstellen kommunizieren, die für uns aber nicht relevant sind. Wir nutzen vor allem die **GPIOs**. Das steht für **G**eneral **P**urpose **I**nput **O**utput, also allgemeine Eingabe und Ausgabe.
Eingaben sind z.B. Werte, die Sensoren aufnehmen und die wir im Programm verarbeiten, Ausgaben sind Signale, um bspw. die Motoren ein- und auszuschalten.

### Aufgabe 1
Wir starten mit einem sogenannten „Hello World“ Programm.
Das ist immer ein möglichst einfaches Programm ohne große Funktionalität, mit dem erst einmal nur getestet wird, ob die Verbindung zu dem Gerät bzw. das Gerät selbst funktioniert. Dafür muss die Arduino IDE aus dem Ordner „arduino-1.8.13-windows-portable“ gestartet werden. Dann öffnet sich ein leeres Projekt, dass aus zwei Funktionen besteht. Diese beiden Funktionen sind immer der Grund-Bestandteil aller Projeke:
+	_setup()_: Diese Funktion wird beim Starten des ESP32 einmalig ausgeführt und wird genutzt, um die Ein- und Ausgänge den Anschluss-Pins des ESP32 zuzuordnen oder eine Verbindung zu initialisieren.

+	_loop()_: Diese Funktion wird solange das Board eingeschaltet ist wiederholt und zum Abfragen bzw. Beschalten der Ein- und Ausgänge genutzt.
Schreiben Sie zunächst diesen Programm-Code in die Funktionen:

```C
void setup() {
   Serial.begin(9600);
}
  
void loop() {
   Serial.println("Hello World");
   delay(2000);
}
```
#### Verbindung zum ESP32 herstellen
Verbinde den ESP32 mit einem microUSB-Kabel mit einem PC. Das passende ESP32-Board (ESP32 DOIT DEVKIT V1) ist in dieser Version der Arduino IDE schon vor-ausgewählt. Stelle als nächstes den Verbindungsport ein. Unter „Werkzeuge > Ports“ werden die möglichen Ports angezeigt. Jetzt kann das Programm in der Arduino IDE das Programm kompiliert und auf den ESP32 geladen werden. Dafür muss das Pfeil-Symbol oben links betätigt werden.
Wenn das Hochladen abgeschlossen ist, lässt sich über _Werkzeuge > Serieller Monitor_ ein weiteres Fenster öffnen, in dem alle 2 Sekunden der Text „Hello World“ ausgegeben wird. Die Kommunikation zwischen dem ESP32 und dem PC funktioniert also.
Dieses neu-geöffnete Fenster ist der Serielle Monitor. Über die Code-Zeile `Serial.begin(9600)` wird eine Verbindung dazu aufgebaut und mit `Serial.println("Hello World")` können dann Text-Zeilen ausgegeben werden. Das kann später, z.B. für Kontroll-Ausgaben, verwendet werden. Falls die Ausgaben nicht menschenlesbar sind, sollte sichergestellt werden, dass die **Baudraten** von Code und seriellem Monitor übereinstimmen.
