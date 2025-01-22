---
layout: page
title: Linienroboter
lang: de
lang-ref: index
---

## Kurzer Überblick

Hallo und Willkommen zu Ihrem Praktikum bei WAGO. In diesem Modul geht es darum, einen Linienroboter aufzubauen, zu löten und zu programmieren. Zuerst sollten Sie sicherstellen, dass alle Bauteile in dem Bauteilsatz vollständig sind. Eine Liste aller Bauteile und welche Bauteile Sie für welche Aufgaben benötigen finden Sie [hier](Material.pdf). Bei Problemen oder Rückfragen können Sie sich gerne an [Herrn Rehburg oder Herrn Lauxtermann wenden](#ansprechpartner).

## Einführung

In der [Einführung](01-ESP-Einführung.pdf) sollen Sie sich zurerst mit der Arduino IDE bekanntmachen und sich elektro- und informationstechnische Grundlagen aneignen, damit Sie später ohne Probleme mit dem Linienroboter umgehen können. Zuerst sollten Sie dafür in der [Materialliste](Material.pdf) sichergehen, dass Sie alle nötigen Bauteile dafür haben. Die Teile für die Einleitung sind nicht in dem Bauteilsatz enthalten. Bei der Bewältigung der Einführung könnte Ihnen [dieses Diagramm von den Pins des ESP32](00-Pinout-ESP32-DEVKIT-V1.pdf) und [dieser Arduino Cheat Sheet](00-CheatSheet-Arduino.pdf) helfen.

## Aufbauanleitung

Wenn Sie sichergestellt haben dass der Bauteilsatz vollständig ist, können Sie mit dem Aufbau starten. Um den Roboter aufbauen zu können, werden alle Bauteile in dem WAGO-Karton benötigt. Bevor Sie mit dem Löten anfangen, geben Sie einem Betreuer bescheid. Folgen Sie dann der [Lötanleitung](02-Lötanleitung.pdf) und anschließed der [Aufbauanleitung](03-Aufbau.pdf).

## Programmieraufgaben für den Roboter

Nach einem Erfolgreichem Aufbau gibt es verschiedene Aufgaben zum bearbeiten. Diese Aufgaben bauen aufeinadner auf und sollten der Reihe nach bearbeitet werden. 

### Linie Folgen

In dieser Aufgabe sollen Sie Ihren Roboter dazu bringen, einer Linie zu folgen. Das ist die Elementarste aller Aufgaben. Das Aufgabenblatt dazu finden Sie [hier](04-Linie-Folgen.pdf). 

### Licht Folgen

Jetzt sollen sie den Code ihres Roboters so erweitern, dass er in der Lage ist, mithilfe der beiden Photowiderstände einem Licht zu folgen. Das Aufgabenblatt dazu finden Sie [hier](05-Licht-Folgen.pdf). 

### Hindernis erkennen 

Die nächste Programmieraufgabe für ihren Roboter besteht darin, ein Hindernis mit dem Ultraschallsensor zu erkennen und darauf zu reagieren. Das Aufgabenblatt dazu finden Sie [hier](06-Hindernis-Erkennung.pdf).

### Hindernis überholen

Als Königsdiziplin der Linienroboter sollen Sie Ihrem Roboter ermöglichen, ein Hindernis zu überholen. Das Aufgabenblatt dazu finden Sie [hier](07-Hindernis-Überholen.pdf).

### Webserver

Um Ihren Linienroboter fernsteuern zu können, steht Ihnen der Code für einen Webserver zur Verfügung. Über diesen Webserver können Sie den Roboter bequem über eine Website steuern. Das zugehörige Dokument mit weiteren Details finden Sie hier.

### Gesamtlösung

Zum testen, ob alles richtig gelötet wurde, können sie hier die Gesamtlösung in einer Zipdatei [herunterladen](docs/Loesung.zip).

## Ansprechpartner

<maik.rehburg@wago.com>

<tobias.lauxtermann@wago.com>
