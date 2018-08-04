// manage.h

#ifndef _MANAGE_h
#define _MANAGE_h

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "arduino.h"
	#else
		#include "WProgram.h"
	#endif


class Manage_Class
{
 protected:

	 uint16_t uptime = 0;

	 uint8_t days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	 //variabili che memorizzano quanti anni, mesi, giorni ha il cane
	 uint16_t past_time_years = 0; //rappresenta la vita complessiva del cane in anni
	 uint16_t past_time_months = 0; //rappresenta la vita complessiva del cane in mesi
	 uint8_t past_months = 0; //rappresenta solo i mesi passati entro un anno
	 uint8_t past_days = 0; //rappresente i giorni passati entro un mese
	 uint8_t d_nope; //variabile che definisce se è già passato o meno il giorno del complemese


	 //funzioni private
	 uint16_t croquettes_daily_value();
	 void daily_calculation();
	 void past_life();
	 void daily_reset();
	 bool new_SD();
	 bool update_SD();
	 bool save_data();
	 bool its_the_moment();

 public:
	// data e ora
	uint16_t year=2018;
	uint8_t month = 8;
	uint8_t day = 4;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;

	// variabili gestione Salvate e richiamate ad ogni avvio
	 
	uint16_t date_of_birth_dog[3] = { 3,3,2018 }; //[0]giorno,[1]mese,[2]anno

	//valori precaricati dalle tabelle nutrizionali dei produttori di crocchette
	//basati sulle tabelle di Royal Canin

		//JUNIOR
											   //mesi  1  2   3   4   5   6   7   8   9   10  11  12  13  14  15
		uint16_t value_gr_croquettes_15_months[15] = { 0,309,406,448,531,603,603,600,600,523,523,436,436,429,429};
	    //ADULT 
		uint8_t months_for_adult = 15; //il mese sucessivo a quello indicato inizia l'erogazione per adulto
		bool its_adult = false;
		uint16_t value_gr_croquettes_adult = 473;

		//se questo valore è vero vengono utilizzate le crocchette 6+/8+/10+/12+ e quindi vengono attivate le variabili sottostanti 
		bool croquettes_for_oldness = false; 
		//MATURE
		uint8_t age_for_mature = 6;
		bool its_mature = false;
		uint16_t value_gr_croquettes_mature = 0;
		//AGEING
		uint8_t age_for_ageing = 10;
		bool its_ageing = false;
		uint16_t value_gr_croquettes_ageing = 0;

	//variabili per il calcolo delle crocchette giornaliere

	bool daily_ceck = true; //se è vero il calcolo giornaliero dei valori e ancora da eseguire

	uint8_t n_of_meals = 3;

	uint16_t gr_today_croquettes;
	uint16_t gr_today_croquettes_left;

	uint16_t original_gr_meal[4];
	uint16_t adj_gr_meal[4];

	uint8_t timetable[8] = { 7,0,12,2,18,0,0,0 };
	bool done_meal[4] = { true,true,true,true };

	//variabili di controllo
	bool start_cicle = false;
	bool end_cicle = false;

	//funzioni pubbliche
	void main_function();
};



#endif

