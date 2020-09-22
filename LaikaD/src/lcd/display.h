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
void display_food_val(int16_t target, int16_t weight);


class Display_Class
{
protected:

public:

	bool setup();
	void update_ui();

};
extern Display_Class display;

#endif
