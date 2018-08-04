// defines pins numbers
const int stepPin = 26; 
const int dirPin = 28;
const int en = 24;
const int vel = 500;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(en,OUTPUT);
  Serial.begin(9600);
}
void loop() {
  Serial.println("ciclo sistema");
  digitalWrite(en,LOW);
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 4000; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(vel); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(vel); 
  }
  delay(200); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 1000; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(vel);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(vel);
  }
  delay(200);
}
