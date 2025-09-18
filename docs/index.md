---
layout: page
title: Linienroboter
lang: de
lang-ref: index
---

## Kurzer Überblick

Hallo und willkommen zum Praktikum bei WAGO. In diesem Modul geht es darum, einen Linienroboter aufzubauen. Dieser kann, wie der Name vermuten lässt, einer Linie folgen. 
Je nach Umfang des Praktikums können aber auch weitere Funktionen implementiert werden, wie dem Überholen eines Hindernisses oder dem Folgen einer Taschenlampe. Dazu muss der Roboter zunächst montiert und gelötet werden, um anschließend die genannten Funktionen programmieren zu können.
Bei Problemen oder Rückfragen können Sie sich gerne an unsere [Ansprechpartner](#ansprechpartner) wenden.

## Einführung

In der [Einführung](01-ESP-Einführung.pdf) sollen Sie sich mit der Arduino IDE vertraut machen und elektro- sowie informationstechnische Grundlagen erlernen, um später den Linienroboter problemlos aufbauen zu können. Zuerst sollten Sie mithilfe der [Materialliste](Material.pdf) sichergehen, dass Sie alle nötigen Bauteile haben. Die Bauteile für diese Einleitung sind nicht in dem Bauteilsatz enthalten. Bei der Durchführung der Einführung könnte Ihnen [dieses Diagramm von den Pins des ESP32](00-Pinout-ESP32-DEVKIT-V1.pdf) und [dieser Arduino Cheat Sheet](00-CheatSheet-Arduino.pdf) helfen.

## Aufbauanleitung

Wenn Sie sichergestellt haben, dass der Bauteilsatz vollständig ist, können Sie mit dem Aufbau starten. Um den Roboter aufbauen zu können, werden alle Bauteile in dem WAGO-Karton benötigt. Bevor Sie mit dem Löten anfangen, geben Sie einem Betreuer Bescheid. Folgen Sie dann der [Lötanleitung](02-Lötanleitung.pdf) und anschließed der [Aufbauanleitung](03-Aufbau.pdf).

## Programmieraufgaben für den Roboter

Nach dem erfolgreichen Aufbau können nun die verschiedenen Funktionen programmiert werden. Diese Aufgaben bauen aufeinadner auf und sollten daher in der dargestellten Reihenfolge bearbeitet werden. 

### Linie Folgen

In dieser Aufgabe sollen Sie Ihren Roboter dazu bringen, einer Linie zu folgen. Das ist die elementarste aller Aufgaben. Das Aufgabenblatt dazu finden Sie [hier](04-Linie-Folgen.pdf). 

### Licht Folgen

Jetzt sollen sie den Code so erweitern, dass der Roboter in der Lage ist, mithilfe der beiden Photowiderstände einem Licht zu folgen. Das Aufgabenblatt dazu finden Sie [hier](05-Licht-Folgen.pdf). 

### Hindernis erkennen 

Die nächste Programmieraufgabe für ihren Roboter besteht darin, ein Hindernis mit dem Ultraschallsensor zu erkennen und darauf zu reagieren. Das Aufgabenblatt dazu finden Sie [hier](06-Hindernis-Erkennung.pdf).

### Hindernis überholen

Als Königsdiziplin der Linienroboter sollen Sie Ihrem Roboter ermöglichen, ein Hindernis zu überholen. Das Aufgabenblatt dazu finden Sie [hier](07-Hindernis-Überholen.pdf).

### Webserver

Um Ihren Linienroboter fernsteuern zu können, steht Ihnen der Code für einen Webserver zur Verfügung. Über diesen Webserver können Sie den Roboter bequem über eine Website steuern. Das zugehörige Dokument mit weiteren Details finden Sie [hier](08-Webserver.pdf). Die benötigte Webserver-Bibliothek finden Sie [hier](Webserver.cpp).


## Ansprechpartner

- [André Bell](mailto:Andre.Bell@wago.com)
- [Tobias Lauxtermann](mailto:Tobias.Lauxtermann@wago.com)
- [Maik Rehburg](mailto:Maik.Rehburg@wago.com)
