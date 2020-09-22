#include <U8glib.h>
#include "arduino.h"
#include "../pins.h"
#include "../config.h"
#include "../utility/debug.h"
#include "../planner/manage.h"

#include "display.h"
#include "main_screen_bitmap.h"
#include "custom_bootscreen.h"

U8GLIB_ST7920_128X64_1X u8g(LCD_ENABLE_PIN, LCD_RW_PIN, LCD_RS_PIN); // SPI Com: e=18,rw=17,rs=16

// the status of the system
int8_t display_status_flag = S_READY;

// current text status
// it will show
char status_text[20];

// the status of the system
int16_t food_weight = 0;
int16_t food_targhet = 0;

// value to know when the system has done the init cycle
bool is_ready = false;

// do pre operations on the display
void update()
{
	switch (display_status_flag)
	{
	case S_READY:
		sprintf(status_text, "Ready");
		break;
	case S_EROGATION:
		sprintf(status_text, "Erogated: %d/%d", food_targhet, food_weight);
		break;
	}
}

// start food erogation once the button is pressed
void update_display_button_state()
{
	if (digitalRead(BTN_ENC_PIN) == LOW)
	{
		DEBUG_PRINTLN("Button Pressed");
		display_status_flag = S_EROGATION;
		update();
		u8g.sleepOff(); //power on the display
		manage.manual_erogation(); // not good

		display_status_flag = S_READY;
		update();
	}
}


void display_startup_screen()
{
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_fub11);
		u8g.drawStr(5, 35, "Power Up");
	} while (u8g.nextPage());
}


void display_main_screen()
{
	// u8g.drawXBM(0, 20, 10, 10, LOGO);
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_fub11);
		u8g.drawStr(0, 12, "Laika");
		
		// status data
		u8g.setFont(u8g_font_6x12);
		u8g.drawStr(0, 63, status_text);
	} while (u8g.nextPage());
}


void display_food_val(int16_t targhet, int16_t weight){
	food_weight = weight;
	food_targhet = targhet;
	update();
	display_main_screen();
}


bool Display_Class::setup()
{

	if (u8g.getMode() == U8G_MODE_R3G3B2)
	{
		u8g.setColorIndex(255);
	}
	else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
	{
		u8g.setColorIndex(3);
	}
	else if (u8g.getMode() == U8G_MODE_BW)
	{
		u8g.setColorIndex(1);
	}
	else if (u8g.getMode() == U8G_MODE_HICOLOR)
	{
		u8g.setHiColorByRGB(255, 255, 255);
	}

	pinMode(BTN_EN1_PIN, INPUT_PULLUP);
	pinMode(BTN_EN2_PIN, INPUT_PULLUP);
	pinMode(BTN_ENC_PIN, INPUT_PULLUP);

	display_startup_screen();
}

// main function that will periodically update all the ui

void Display_Class::update_ui()
{
	// on fist loop set the display ready
	if (is_ready == false){
		is_ready = true;
		display_status_flag = S_READY;
		update();
	}

	update_display_button_state();
	display_main_screen();
}