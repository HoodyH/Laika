#include "base.h"

Manage_Class manage;
Data_Time_Class data_time;

void setup()
{
	Serial.begin(9800);
	data_time.rtc_setup();
	card_rw.setup();
	
}


void loop()
{
	
	manage.main_function();
	
	

	delay(5000);
}


