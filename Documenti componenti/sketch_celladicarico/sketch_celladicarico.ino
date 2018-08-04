#include "HX711.h"
#define DOUT A1
#define CLK A0
HX711 bilancia(DOUT, CLK);
int peso = 0; // zona di memorizzazione del peso corrente
int pesoprec = 0; // zona di memorizzazione dell'ultimo peso esposto
//
void setup()
{
 Serial.begin(9600);
 Serial.println("calcolo della tara - tare");
 Serial.println("non mettere nulla sul piatto – don’t place anything on the scale...");
 delay (100);
 bilancia.set_scale(2022); // inserire il valore di scala, al posto di 2022 - insert scale value instead of 2022
 bilancia.tare(60); // il peso a vuoto e' considerato tara
 Serial.println("sistema pronto - system ready");
}
//
void loop()
{
 peso = bilancia.get_units(20), 3;
 if (!(peso == pesoprec)) // se e' variato il peso
  {
    pesoprec = peso; // memorizza il peso corrente
    Serial.print("peso - weigth: ");
    Serial.print(peso);
    Serial.println("g");
  }
 delay(400); // attende 400 millisecondi prima di procedere
// alla successiva pesata
}
