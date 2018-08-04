#include "../../base.h"

RTC_DS1307 RTC;

void Data_Time_Class::rtc_setup() {
	
	Wire.begin();
	RTC.begin();

	if (!RTC.isrunning()) {
		RTC.adjust(DateTime(__DATE__, __TIME__));
	}
}

void Data_Time_Class::get_data_time(uint16_t *y, uint8_t *m, uint8_t *d, uint8_t *h, uint8_t *min, uint8_t *s) {
	
	DateTime now = RTC.now();

	*y = now.year();
	*m = now.month();
	*d = now.day();
	*h = now.hour();
	*min = now.minute();
	*s = now.second();

}


bool Data_Time_Class::ceck() {
	
	if (!RTC.isrunning()) {
		return true;
	}
	return false;
}

