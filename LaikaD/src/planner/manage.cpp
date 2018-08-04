#include "../../base.h"


void Manage_Class::past_life() { //calcola quanti anni, mesi, giorni ha il cane 

	if (day >= date_of_birth_dog[0])
		d_nope = 0; //il giorno è stato superato
	else
		d_nope = 1; //il giorno non è stato superato

	if (year == date_of_birth_dog[2] && month >= date_of_birth_dog[1]) {
		past_time_months = past_months = (month - date_of_birth_dog[1]) - d_nope;
	}
	else {
		past_time_years = year - date_of_birth_dog[2];
		past_time_months = ((past_time_years * 12 - date_of_birth_dog[1]) + month) - d_nope;
		past_months = ((12 - date_of_birth_dog[1]) + month) - d_nope;
	}

	if (d_nope == 0)
		past_days = day - date_of_birth_dog[0];
	else
		past_days = (days_per_month[month - 1] - date_of_birth_dog[0]) + day;
}




uint16_t Manage_Class::croquettes_daily_value() {

	past_life(); //controlla quanto tempo ha il cane e lo memorizza nelle variabili di classe

	if (past_time_months <= months_for_adult)
		its_adult = false;
	else 
		its_adult = true;
	//controlla in che fase della vita si trova il cane
	if (croquettes_for_oldness) {
		if (past_time_years > age_for_mature && past_time_years <= age_for_ageing) {
			its_mature = true;
			its_ageing = false;
		}
		if (past_time_years > age_for_ageing) {
			its_mature = true;
			its_ageing = true;
		}
		if (its_ageing)
			return value_gr_croquettes_ageing;
		if (its_mature)
			return value_gr_croquettes_mature;
	}

	if (its_adult)
		return value_gr_croquettes_adult;
	//incremente ogni giorno la quantià di crocchette di modo da evitare salti tra un mese e l'altro
	else {
		int16_t gap = value_gr_croquettes_15_months[past_time_months] - value_gr_croquettes_15_months[past_time_months - 1];
		uint16_t val = value_gr_croquettes_15_months[past_time_months - 1] + ((gap / 30) * past_days);
		return val;
	}		
}




//calcola le porzioni in base ai pasti deve dare al giorno
void Manage_Class::daily_calculation() { 

	gr_today_croquettes = gr_today_croquettes_left = croquettes_daily_value();

	uint16_t waste;
	uint16_t portion;

		waste = gr_today_croquettes % n_of_meals;
		portion = (gr_today_croquettes - waste) / n_of_meals;
		
		original_gr_meal[0] = portion + waste;
		for(uint8_t i = 1; i < n_of_meals; i++){
			original_gr_meal[i] = portion;
		}		
}




bool Manage_Class::its_the_moment() {

	int i = 0;

	while (i < n_of_meals) {
		if (done_meal[i] == true)//controllo a quale pasto � arrivato
			i++;
		else if (hour < timetable[i * 2]) //controllo se � l'ora
			return false;
		else if (minute >= (15 * timetable[(i * 2) + 1])) //controlla se � al quarto d'ora
			return true;
	}
	return false;
}

void Manage_Class::main_function() {

	//se è vero il calcolo giornaliero dei valori e ancora da eseguire
	if (daily_ceck) {
		daily_calculation();
		daily_ceck = false; //controllo eseguito
	}

	data_time.get_data_time(&year, &month, &day, &hour, &minute, &second);
	daily_calculation();
	
	Serial.println(past_time_years);
	Serial.println(past_time_months);
	Serial.println(past_days);
	Serial.print("gr_today_croquettes: ");
	Serial.println(gr_today_croquettes);
	Serial.print("gr_today_croquettes_left: ");
	Serial.println(gr_today_croquettes_left);
	Serial.print("original_gr_meal: ");
	Serial.print(original_gr_meal[0]);
	Serial.print(" ");
	Serial.print(original_gr_meal[1]);
	Serial.print(" ");
	Serial.println(original_gr_meal[2]);

	if (start_cicle) {
		
		
		Serial.println("OK");

		Serial.print("Data ");
		Serial.print(year);
		Serial.print(" ");
		Serial.print(month);
		Serial.print(" ");
		Serial.println(day);

		Serial.print("ora ");
		Serial.print(hour);
		Serial.print(" ");
		Serial.print(minute);
		Serial.print(" ");
		Serial.println(second);
		
	}

}