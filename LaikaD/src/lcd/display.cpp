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

// values to update the encoder on the display
int counter = 0;
int actual_state;
int last_state;
float rad2grad = 57.295779513;

void update_display_encoder()
{

	actual_state = digitalRead(BTN_EN1_PIN); // Reads the "current" state of the outputA
											 // If the previous and the current state of the outputA are different, that means a Pulse has occured
	if (actual_state != last_state)
	{
		// If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
		if (digitalRead(BTN_EN2_PIN) != actual_state)
		{
			if (counter > 23)
				counter = 0;
			else
				counter++;
		}

		else
		{
			if (counter < 0)
				counter = 23;
			else
				counter--;
		}
		DEBUG_PRINT("Position: ");
		DEBUG_PRINTLN(counter);
	}

	last_state = actual_state; // Updates the previous state of the outputA with the current state
}

void update_display_button_state()
{
	if (digitalRead(35) == LOW)
	{
		DEBUG_PRINTLN("Button Pressed");
		manage.manual_erogation(); // not good
	}
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
}

void Display_Class::draw()
{
	u8g.setFont(u8g_font_fub11);
	u8g.drawStr(0, 12, "Laika");

	// Signature
	u8g.setFont(u8g_font_6x12);
	u8g.drawStr(0, 63, "Simone Not");
}

void Display_Class::main_screen()
{
	// u8g.drawXBM(0, 20, 10, 10, LOGO);
	draw();
}

// main function that will periodically update all the ui

void Display_Class::update_ui()
{
	u8g.firstPage();
	update_display_encoder();
	update_display_button_state();

	do
	{
		main_screen();

	} while (u8g.nextPage());
}