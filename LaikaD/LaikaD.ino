#include "./src/planner/manage.h"

Manage_Class manage;

void setup()
{
	Serial.begin(9800);
	manage.setup_all();
}

void loop()
{
	manage.main_function();
	// manage.test();
}
