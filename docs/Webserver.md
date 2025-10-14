---
layout: page
title: Linienroboter - Webserver
lang: de
lang-ref: index
---
## Webserver
Um den Linienroboter fernsteuern zu können, steht der Code für einen Webserver zur Verfügung. Über diesen Webserver kann man den Roboter bequem über eine Website steuern.

### Vorbereitung
Um den Webserver nutzen zu können, müssen die Arduino IDE einige Bibliotheken hinzugefügt werden:
+ "ESP Async WebServer" von "ESP32Async"
+ "Async TCP" von "ESP32Async"
+ "ESPUI" von "Lukas Bachschwell"

{:.info}
Bibliotheken können über den Bibliotheksverwalter min der linken Seitenleiste installiert werden.

Des Weiteren muss der [Code für den Webserver](Webserver.cpp) in das Quellverzeichnis des Arduinosketches gespeichert werden. In der `.ino`-Datei muss dieser dann aufgerufen werden:

```C
#include Webserver.cpp
```

In der `setup()`-Funktion muss der Server initialisiert und die `start()`-Methode aufgerufen werden:

```C
Webserver server;
server.start("ESP32", "");
```
Dabei ist der erste Teil vor dem Komma der Name des Netzwerks und der Teil dahinter das Passwort, welches leer bleiben kann.
In der `loop()`-Funktion muss `server.getPadBack();` aufgerufen werden.

Beispiel:

```C
#include "Webserver.cpp"

Webserver server;

void setup(){
    Serial.begin(9600);
    server.start("ESP32", "");
}

void loop(){
    server.getPadBack();
}
```
### Webserver öffnen
Um den Webserver nutzen zu können, musst du dein Handy oder deinen PC entsprechend der Angaben aus dem Code mit dem Netzwerk des ESP32 verbinden.
Der Webserver ist unter der IP-Adresse des ESP32 `192.168.4.1` zu erreichen. Sollte die Adresse abweichen, kann sie auch im seriellen Monitor abgelesen werden.

### Codeanpassungen
Jetzt muss nur noch der Code angepasst werden, um die Funktionen des Webservers zu nutzen.
Die Funktionen, welche du bereits programmiert hast, sollen nur aktiv werden, wenn der jeweilige Button auf der Weboberfläche gedrückt ist. Hierfür müssen die Codeblöcke in ein _If_-Statement eingebettet werden.

```C
if(server.getObstacleButton()){
	//Dein Code, welcher ausgeführt werden soll
}
```
Welche Funktionen es gibt, kann man über die Autocompletion der IDE (_Str+Leertaste_) oder die Datei Webserver.cpp einsehen.

|        |       |
|:-------|------:|
|<a href="Hindernis-ueberholen.html"><- Hindernis überholen</a>||