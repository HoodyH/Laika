// display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#define S_STARTUP 0
#define S_READY 1
#define S_EROGATION 2
#define S_COMPLEATED 3
#define S_ERROR 4

// update the amount of food erogated
// on display during erogation status
void display_food_val(int16_t weight, int16_t target, int16_t weight_tot, int16_t target_tot);

// the values are negative if the food is erogated and positive if not
// n_meals are the lenght of the array
void display_today_food(uint16_t *values, uint8_t n_meals);

// when the erogation is done and is time to set again the ready message
void display_operation_completed();


class Display_Class
{
protected:

public:

	bool setup();
	void update_ui();

};
extern Display_Class display;

#endif
