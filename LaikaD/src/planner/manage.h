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
	byte days_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	//variabili che memorizzano quanti anni, mesi, giorni ha il cane
	uint16_t past_time_years;  //rappresenta la vita complessiva del cane in anni
	uint16_t past_time_months; //rappresenta la vita complessiva del cane in mesi
	byte past_months;		   //rappresenta solo i mesi passati entro un anno
	byte past_days;			   //rappresente i giorni passati entro un mese
	byte d_nope;			   //variabile che definisce se � gi� passato o meno il giorno del complemese

	//funzioni private
	void past_life();
	bool birthday();
	void portion_calculation(uint16_t food); //aggiorna tutte le vatiabili relative alle dosi giornaliere
	void update_dayly_food_value();
	bool its_the_moment();
	bool reset_to_do();
	void reset_day();
	bool food_left_in_tank();
	
	void update_current_food_schedule();
	void update_next_food_schedule();

	bool is_manual = false;

public:
	// data e ora
	uint16_t year;
	byte month;
	byte day;
	byte hour;
	byte minute;
	byte second;

	//-------------------------------------------------------------------------
	// Informazioni Base statiche sul cane
	//-------------------------------------------------------------------------

	uint16_t date_of_birth_dog[3] = {3, 3, 2018}; //[0]giorno,[1]mese,[2]anno
	byte n_meals = 3;						  //numero di pasti giornalieri
	byte timetable[8] = {7, 0, 12, 2, 17, 3, 0, 0};

	//valori precaricati dalle tabelle nutrizionali dei produttori di crocchette
	//basati sulle tabelle di Royal Canin (di default)
	//JUNIOR
	//mesi  1  2   3   4   5   6   7   8   9   10  11  12  13  14  15
	uint16_t value_gr_food_15_months[15] = {0, 309, 406, 448, 531, 603, 603, 600, 600, 523, 523, 436, 436, 429, 429};
	//ADULT
	byte months_for_adult = 15; //il mese sucessivo a quello indicato inizia l'erogazione per adulto
	uint16_t value_gr_food_adult = 473;

	//se questo valore � vero vengono utilizzate le crocchette 6+/8+/10+/12+ e quindi vengono attivate le variabili sottostanti
	bool food_for_oldness = false;

	//MATURE
	byte age_for_mature = 6; //l'anno indicato � compreso
	uint16_t value_gr_food_mature = 0;
	//AGEING
	byte age_for_ageing = 10; //l'anno indicato � compreso
	uint16_t value_gr_food_ageing = 0;

	//-------------------------------------------------------------------------
	//variabili per il controllo giornaliero
	//-------------------------------------------------------------------------
	uint16_t tank_food_left = 18000;
	bool low_food_warning = false;

	bool its_adult = false;
	bool its_mature = false;
	bool its_ageing = false;

	bool happy_birthday = false;
	bool daily_ceck_to_do = true; //se � vero il calcolo giornaliero dei valori e ancora da eseguire
	bool adult_portion_to_calculate = true;

	bool today_all_done = false; //da resettare ogni giorno
	bool reset_done = false;

	//-------------------------------------------------------------------------
	//variabili per il controllo di ciclo
	//-------------------------------------------------------------------------
	uint16_t gr_today_food;
	uint16_t gr_today_food_left;
	uint16_t gr_today_food_provided;

	byte real_timetable[8];

	// the value for each meal by the foot table
	uint16_t original_gr_meal[4];

	// if there weight process exceex in the drop
	// here the  amount given will be adjusted
	uint16_t adj_gr_meal[4];

	// the index of the current meal
	// must be resetted every day
	byte index_of_this_meal = 0;

	// save each time the food erogation is done
	// must be resetted every day
	bool done_meal[4] = {false, false, false, false};

	//errors
	bool error_occur_in_feed = false;

	// to remove (SD vincle)
	int16_t currently_weight = 0;
	int16_t total_currently_weight = 0;
	bool status_start_cicle = false;
	bool status_unload_sequence = false;
	bool status_end_cicle = false;

	//funzioni pubbliche
	void setup();

	void main_function();
	void manual_erogation();
	void firstStart();
};
extern Manage_Class manage;

#endif
