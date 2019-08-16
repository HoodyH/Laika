// datatime.h

#ifndef _DATATIME_h
#define _DATATIME_h

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "arduino.h"
	#else
		#include "WProgram.h"
	#endif

class Data_Time_Class
{
 protected:
	
 public:

	void rtc_setup();
	void get_data_time(uint16_t *y, byte *m, byte *d, byte *h, byte *min, byte *s);
	bool ceck();
	
}; extern Data_Time_Class data_time;


#endif

