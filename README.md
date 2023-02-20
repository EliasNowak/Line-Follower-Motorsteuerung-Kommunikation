# Line-Follower

# Funktionsweise
Der Roboter ist mit einer Kamera ausgestattet, die Bilder von der Umgebung aufnimmt und an ein eingebettetes System (Raspberry Pi) weiterleitet. Das eingebettete System verarbeitet die Bilder, um die Linie auf dem Boden zu erkennen. Sobald die Linie erkannt ist, werden [Instructions](#Instructions) an den Arduino gesendet um der Linie zu folgen.

Um die Bewegungen des Roboters zu steuern, die die Geschwindigkeit und Richtung des Roboters basierend auf der Position der Linie und der aktuellen Position des Roboters anpassen. Man könnte auf die Verwendung von neuronalen Netzen oder maschinellem Lernen zurückgreifen, um die Bewegungen des Roboters anzupassen.














# Instructions
| T       |      | L    | V | V | Beschreibung                                                |
|---------|------|------|---|---|-------------------------------------------------------------|
| STOP    | 0x00 | 0x00 | - | - | Roboter stoppt                                              |
| DIR     | 0x01 | 0x02 | vL| vR| vL -> Geschwindigkeit Left, vR -> Geschwindigkeit Right     |
| BACK    | 0x03 | 0x01 | v | - | Roboter fährt zurück   (v -> Geschwindigkeit)               |
| FORWARD | 0x04 | 0x01 | v | - | Roboter fährt vorwärts (v -> Geschwindigkeit)               |
