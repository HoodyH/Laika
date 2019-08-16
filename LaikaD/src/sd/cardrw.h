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
	 bool setup();
	 //String(*f)()
	 bool write (String file_name);
	 bool read(String file_name);
	 
	 //funzioni statiche per lettura e scrittura Data
	 //informazioni Base sul cane
	 void save_base_data();
	 void load_base_data();

	 //file salvataggio pasti completati
	 void save_record();

	 //file salvataggio data di processo
	 bool save_daily_data();
	 bool load_daily_data();
	 bool save_cycle_data();
	 bool load_cycle_data();

	 bool error_log(String error);
};
extern Card_rw_Class card_rw;

#endif
