#include "./src/planner/manage.h"
#include "./src/lcd/display.h"

Manage_Class manage;
Display_Class display;

void setup()
{
	Serial.begin(9800);
	manage.setup();
	display.setup();
}

void loop()
{
	manage.main_function();
	display.update_ui();
}
