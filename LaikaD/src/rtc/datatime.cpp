#include "../../base.h"

RTC_DS1307 RTC;

void Data_Time_Class::rtc_setup() {
	
	Wire.begin();
	RTC.begin();

	RTC.adjust(DateTime(__DATE__, __TIME__));

	if (RTC.isrunning()) {
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("rtc setup completed");
	}
	else
		error.system_status(CRITICAL_ERROR_8400);
}

void Data_Time_Class::get_data_time(uint16_t *y, byte *m, byte *d, byte *h, byte *min, byte *s) {
	
	DateTime now = RTC.now();

	*y = now.year();
	*m = now.month();
	*d = now.day();
	*h = now.hour();
	*min = now.minute();
	*s = now.second();
}


bool Data_Time_Class::ceck() {
	
	if (RTC.isrunning()) {
		return true;
	}
	return error.system_status(CRITICAL_ERROR_8400);
}
