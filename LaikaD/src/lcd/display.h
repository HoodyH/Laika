// display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#define S_STARTUP 0
#define S_READY 1
#define S_EROGATION 2
#define S_COMPLEATED 3
#define S_ERROR 4


class Display_Class
{
protected:

	void update_status();
	void update_time();
	void display_startup_screen();
	void display_main_screen();
	void update_display_button_state();


	// the status of the system
	int8_t display_status_flag = S_READY;

public:

// update the amount of food erogated
// on display during erogation status
void food_val(int16_t weight, int16_t target, int16_t weight_tot, int16_t target_tot);

// the values are negative if the food is erogated and positive if not
// n_meals are the lenght of the array
void today_food(uint16_t *values, uint8_t n_meals);

// display the furure time when the food will be automatically released
void next_food_schedule(int8_t hour, int8_t minute);

// display the erogation data
void start_erogation() {display_status_flag = S_EROGATION;}

// when the erogation is done and is time to set again the ready message
void operation_completed() {display_status_flag = S_READY;}

// force the update of the display externally
void update();

bool setup();
void update_ui();

};
extern Display_Class display;

#endif
