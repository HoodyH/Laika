#include <stdint.h>
#include "arduino.h"
#include "../pins.h"

#include "buzzer.h"

void Buzzer_Class::setup()
{

	pinMode(BEEPER_PIN, OUTPUT);
}

//-------------------------------------------------------------------------------------------------------
//riproduce inizio erogazione cibo
const int16_t FOOD_EROGATION_START_MELODY[] = {NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5, 0, 0};
const int16_t DURATIONS_FOOD_EROGATION_START_MELODY[] = {4, 8, 8, 4, 4, 4, 4, 4, 1, 1};
#define N_NOTE_FOOD_EROGATION_START_MELODY 10
//-------------------------------------------------------------------------------------------------------
//riproduce fine erogazione cibo
const int16_t FOOD_EROGATION_END_MELODY[] = {NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5, 0, 0};
const int16_t DURATIONS_FOOD_EROGATION_END_MELODY[] = {4, 8, 8, 4, 4, 4, 4, 4, 1, 1};
#define N_NOTE_FOOD_EROGATION_END_MELODY 10
//-------------------------------------------------------------------------------------------------------
const int16_t CRITICAL_ERROR_MELODY[] = {NOTE_D5, NOTE_D5, NOTE_D6};
const int16_t DURATION_CRITICAL_ERROR_MELODY[] = {2, 8, 8};
#define N_NOTE_CRITICAL_ERROR_MELODY 3
//-------------------------------------------------------------------------------------------------------
const int16_t FATAL_ERROR_MELODY[] = {NOTE_A5, NOTE_A5, NOTE_A6};
const int16_t DURATION_FATAL_ERROR_MELODY[] = {2, 8, 8};
#define N_NOTE_FATAL_ERROR_MELODY 3
//-------------------------------------------------------------------------------------------------------

void Buzzer_Class::play_melody(uint16_t select)
{

	switch (select)
	{

	case FOOD_EROGATION_START_MELODY_SELCT:
		for (int this_note = 0; this_note < N_NOTE_FOOD_EROGATION_START_MELODY; this_note++)
		{
			int time = 1000 / DURATIONS_FOOD_EROGATION_START_MELODY[this_note];
			play_note(FOOD_EROGATION_START_MELODY[this_note], time);
		}
		break;

	case FOOD_EROGATION_END_MELODY_SELCT:
		for (int this_note = 0; this_note < N_NOTE_FOOD_EROGATION_END_MELODY; this_note++)
		{
			int time = 1000 / DURATIONS_FOOD_EROGATION_END_MELODY[this_note];
			play_note(FOOD_EROGATION_END_MELODY[this_note], time);
		}
		break;

	case CRITICAL_ERROR_MELODY_SELCT:
		for (int this_note = 0; this_note < N_NOTE_CRITICAL_ERROR_MELODY; this_note++)
		{
			int time = 1000 / DURATION_CRITICAL_ERROR_MELODY[this_note];
			play_note(CRITICAL_ERROR_MELODY[this_note], time);
		}
		break;

	case FATAL_ERROR_MELODY_SELCT:
		for (int this_note = 0; this_note < N_NOTE_FATAL_ERROR_MELODY; this_note++)
		{
			int time = 1000 / DURATION_FATAL_ERROR_MELODY[this_note];
			play_note(FATAL_ERROR_MELODY[this_note], time);
		}
		break;
	}
}

void Buzzer_Class::play_note(int period, word length)
{

	tone(BEEPER_PIN, period, length);
	delay(length);
}
