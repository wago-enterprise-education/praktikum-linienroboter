---
layout: page
title: Linienroboter - Licht folgen
lang: de
lang-ref: index
---

## Licht folgen

Als nächstes soll der Roboter dem Licht einer Taschenlampe folgen können. Dafür nutzen wir die Fotowiderstände. Man nennt diese Bauteile auch LDR ("Light Dependent Resistor"). Das sind Widerstände, deren Widerstandswerte lichtabhängig sind. Je mehr Licht auf einen Fotowiderstand fällt, umso kleiner ist der Widerstandswert.

<img src="img/fotowiderstand.png>" alt="Fotowiderstand">

Auf der Platine ist der Fotowiderstand in einem Spannungsteiler mit einem 470 Ω Widerstand. Die Gesamtspannung, die über den beiden Widerständen abfällt, ist immer gleich. Ändert sich der Wert des Widerstandes, ändert sich auch die Teil-Spannung U, die darüber abfällt. Diese Spannung liegt am Eingang des ESP32 an. Die Veränderung können wir also wieder, wie bei dem Potentiometer aus den Aufgaben in Teil 1, über einen analogen Eingang auswerten.

Lass dir die Werte der Sensoren zunächst im Seriellen Monitor ausgeben umd schaue, wie sich die Werte entwickeln, wenn sich die Helligkeit verändert. Da das Tageslicht durch ein Fenster viel stärker ist als das einer Taschenlampe, müssen z. B. auch verschiedene Tageszeiten oder die Helligkeit in Räumen beachtet werden.

Der ausgelesene Wert lässt sich mithilfe eines zuvor gelernten Befehls in Motorstärke „umformen“. Da für den Befehl die übergebenen Werte nur innerhalb eines bestimmten Bereichs sein dürfen, verwenden wir den Befehl
`constrain(x, minWert, maxWert);`