//include "cardrw.h"

#ifndef _CARDRW_H
#define _CARDRW_H

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "arduino.h"
	#else
		#include "WProgram.h"
	#endif


class Card_rw_Class
{
 protected:

 public:
	 char a[50];
	 bool setup();
	 bool write (String(*f)(), String file_name);
	 bool read(String file_name, String out);

};

extern Card_rw_Class card_rw;

#endif
