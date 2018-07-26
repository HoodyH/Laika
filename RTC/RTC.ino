#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
char buffer1[20];
char buffer2[20];
RTC_DS1307 RTC;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int setpin = 6;
boolean statepin = 0;
void setup () {
  pinMode (6, INPUT);
  digitalWrite (6, HIGH);
  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  if (! RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}
void loop () {
  statepin = digitalRead (setpin);
  if (statepin == LOW)
  {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  sprintf(buffer1,  "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.setCursor(0,0);
  lcd.print( buffer1 );
  sprintf(buffer2,  "%02d/%02d/%d", now.day(), now.month(), now.year());
  lcd.setCursor(0,1);
  lcd.print( buffer2 );
  delay(1000);
}
