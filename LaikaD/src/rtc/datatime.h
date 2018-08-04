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
	void get_data_time(uint16_t *y, uint8_t *m, uint8_t *d, uint8_t *h, uint8_t *min, uint8_t *s);
	bool ceck();

	uint16_t past_months(uint16_t data1[], uint16_t data2[]);

};

extern Data_Time_Class data_time;

#endif

