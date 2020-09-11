// display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

class Display_Class
{
protected:
	int counter = 0;
	int actual_state;
	int last_state;
	float rad2grad = 57.295779513;

public:
	bool setup();
	void draw();
	void update_encoder();

	void main_screen();

	void main_loop();
};
extern Display_Class display;

#endif
