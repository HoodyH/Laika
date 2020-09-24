#include <Wire.h>
#include <RTClib.h>
#include "../config.h"
#include "../utility/errors.h"
#include "../utility/debug.h"

#include "datatime.h"

RTC_DS1307 RTC;

void Data_Time_Class::rtc_setup()
{

	Wire.begin();
	RTC.begin();

	RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

	if (RTC.isrunning())
		DEBUG_PRINTLN("rtc setup completed");
	else
		error.system_status(CRITICAL_ERROR_8400);

	// query the rtc module
	startup_datetime = RTC.now();
	char date_time_setup[16];

	// generate the string on startup to show the date time on serial
	sprintf(
		date_time_setup, 
		"%04d/%02d/%02d %02d:%02d:%02d", 
		now.year(), 
		now.month(),
		now.day(),
		now.hour(),
		now.minute(),
		now.second()
	);

	DEBUG_PRINT("Rtc setup time: ");
	DEBUG_PRINTLN(date_time_setup);
}

// return time from the rtc module, without exposing the DateTime object
void Data_Time_Class::get_data_time(uint16_t *y, byte *m, byte *d, byte *h, byte *min, byte *s)
{

	DateTime now = RTC.now();

	*y = now.year();
	*m = now.month();
	*d = now.day();
	*h = now.hour();
	*min = now.minute();
	*s = now.second();
}

// check if the Rtc module it's ok, if not raise a critical error.
bool Data_Time_Class::ceck()
{

	if (RTC.isrunning())
	{
		return true;
	}
	return error.system_status(CRITICAL_ERROR_8400);
}
