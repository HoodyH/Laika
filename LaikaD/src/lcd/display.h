// display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

class Display_Class
{
protected:

public:
	bool setup();
	void draw();
	void main_screen();

	void update_ui();
};
extern Display_Class display;

#endif
