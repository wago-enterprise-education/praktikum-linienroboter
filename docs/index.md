---
layout: page
title: Linienroboter
lang: de
lang-ref: index
---

## Kurzer Überblick

Hallo und Willkommen zum Praktikum bei WAGO. In diesem Modul geht es darum, einen Linienroboter aufzubauen. Dieser kann, wie der Name vermuten lässt, einer Linie folgen.
Je nach Umfang des Praktikums können aber auch weitere Funktionen, wie das Überholen eines Hindernisses oder dem Folgen einer Taschenlampe implementiert werden.
Dazu muss der Roboter zunächst montiert und gelötet werden, um anschließend die genannten Funktionen programmieren zu können.
Bei Problemen oder Rückfragen kannst du dich gerne an unsere [Ansprechpartner](#ansprechpartner) wenden.

## Aufbauanleitung

Wenn du sichergestellt hast, dass der Bauteilsatz vollständig ist, kannst du mit dem Aufbau starten. Um den Roboter aufbauen zu können, werden alle Bauteile in dem WAGO-Karton benötigt.
Bevor du mit dem Löten anfängst, gib einem Betreuer Bescheid. Folge dann der [Lötanleitung](02-Lötanleitung.pdf) und anschließed der [Aufbauanleitung](03-Aufbau.pdf).

## Programmieraufgaben für den Roboter

Nach dem erfolgreichen Aufbau können nun die verschiedenen Funktionen programmiert werden. Diese Aufgaben bauen aufeinander auf und sollten daher in der dargestellten Reihenfolge bearbeitet werden.

### Linie Folgen

In dieser Aufgabe soll der Roboter dazu gebracht werden, einer Linie zu folgen. Das ist die elementarste aller Aufgaben. Das Aufgabenblatt dazu findet sich [hier](04-Linie-Folgen.pdf).

### Licht Folgen

Jetzt sollen Sie den Code so erweitern, dass der Roboter in der Lage ist, mithilfe der beiden Photowiderstände einem Licht zu folgen. Das Aufgabenblatt dazu befindet sich [hier](05-Licht-Folgen.pdf).

### Hindernis erkennen

Die nächste Programmieraufgabe für den Roboter besteht darin, ein Hindernis mit dem Ultraschallsensor zu erkennen und darauf zu reagieren. Das Aufgabenblatt dazu findest du [hier](06-Hindernis-Erkennung.pdf).

### Hindernis überholen

Als Königsdiziplin der Linienroboter sollen dem Ihrem Roboter ermöglicht werden, ein Hindernis zu überholen. Das Aufgabenblatt dazu findest du [hier](07-Hindernis-Überholen.pdf).

### Webserver

Um den Linienroboter fernsteuern zu können, steht der Code für einen Webserver zur Verfügung. Über diesen Webserver kann man den Roboter bequem über eine Website steuern. Das zugehörige Dokument mit weiteren Details findest du [hier](08-Webserver.pdf). Die benötigte Webserver-Bibliothek steht [hier](Webserver.cpp) zum Download bereit.


## Ansprechpartner

- [André Bell](mailto:Andre.Bell@wago.com)
- [Tobias Lauxtermann](mailto:Tobias.Lauxtermann@wago.com)
- [Maik Rehburg](mailto:Maik.Rehburg@wago.com)

<div style="text-align:right"><a href="./Einfuehrung.html">Einführung -></a></div>