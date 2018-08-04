#include <LiquidCrystal.h>

LiquidCrystal lcd(11,10,9,8,7,6,5,4,3,2);

int encoderPin1 = 10;
int encoderPin2 = 11;
int fase = 5;
int neutro = 6;

int statoPrecedenteEncoder;
long valoreEncoder = 0;
long previousMillis = 0;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;
int commutate = LOW;
int fadeValue = 255;
  
 
void setup() {
  Serial.begin (9600);
  lcd.begin(128,64);
 
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(fase, OUTPUT);
  pinMode(neutro, OUTPUT);
  
  digitalWrite(encoderPin1, HIGH); //abilita resistenza di pullup
  digitalWrite(encoderPin2, HIGH);
 
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE); 
}
 
void loop() {
  
  Serial.println(valoreEncoder);
  
  motore();
  
  lcd.setCursor(0,0);
  lcd.print(1);
  lcd.setCursor(0,1);
  lcd.print("e:");
  lcd.setCursor(4,1);
  lcd.print(valoreEncoder);
  
}
 
void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
 
  int encoded = (MSB << 2)|(www << 1) |LSB; //converte i valori dei 2 pin in un unico numero binario
  int stato  = (statoPrecedenteEncoder << 2) | encoded; //esegue l'end tra il precedente traslato di 2 e quello appena acquisito
 
  if(stato == 0b1101 || stato == 0b0100 || stato == 0b0010 || stato == 0b1011) valoreEncoder ++;
  if(stato == 0b1110 || stato == 0b0111 || stato == 0b0001 || stato == 0b1000) valoreEncoder --;
 
  statoPrecedenteEncoder = encoded; //memorizza ultimo stato
}

void motore(){
  
  int intervallo = 50;
  
  unsigned long currentMillis = millis();
  
    if(currentMillis - previousMillis > intervallo)
       previousMillis = currentMillis; 


    
     for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=1) { 
       analogWrite(fase, fadeValue);         
       delay(30);}
       
     for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
       analogWrite(fase, fadeValue);         
       delay(30);        
      } 
}
