# Laika

Software to auto feed my dog.
Basically it's a dog feeder on a much larger scale.

# Requrements to compile

Install this libraries from the library handler in the arduino IDE

- RTClib (by adafruit v 1.11.2)
- ArduinoJson (by Benoit Blanchon v5.12.0)
- U8glib (by oliver v1.19.1)
- HX711 Arduino Library (by bogdan necula v0.7.4)

### TODO

Calcolo divisione pasto

Riduce la quantià di crocchette giornaliera di modo che il numero ottenuto diviso in numero dei pasti dia resto 0.
La quantità sottratta viene aggiunta al primo pasto per avere più possibilità di correzioni

Funzionalità extra menu programmazione

- l'ora viene impostata in modo che il pasto sucessivo non sia prima del precedente
- il primo pasto non può essere dopo le 11.15pm di modo che il 2o sia alle 11.30pm ed il 3o alle 11.45pm
- se modifichi il primo, se il secondo è già impostato non viene modificato a meno che non ci sia meno di un quarto d'ora di differenza.
