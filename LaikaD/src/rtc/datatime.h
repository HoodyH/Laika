// datatime.h
#ifndef _DATATIME_h
#define _DATATIME_h

#include <RTClib.h>


class Data_Time_Class
{
protected:

	DateTime startup_datetime;
	
public:

	void rtc_setup();
	void get_data_time(uint16_t *y, byte *m, byte *d, byte *h, byte *min, byte *s);
	bool ceck();
	
}; extern Data_Time_Class data_time;


#endif

