#include "./src/planner/manage.h"
#include "./src/lcd/display.h"
#include "./src/rtc/datatime.h"

Manage_Class manage;
Display_Class display;
Data_Time_Class data_time;

void setup()
{
	Serial.begin(9800);
  	display.setup();
	manage.setup();
}

void loop()
{
	manage.main_function();
	display.update_ui();
}
