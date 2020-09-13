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
	u8g.drawStr(0, 12, "Test Encoder");

	u8g.setFont(u8g_font_fub20);

	char buf[8];
	sprintf(buf, "%d %d", digitalRead(BTN_ENC_PIN), (counter));
	u8g.drawStr(10, 40, buf);

	u8g.drawCircle(96, 34, 19);
	u8g.drawCircle(96, 34, 20);

	// Line
	// enc 20 step = 360/20 = 18�
	float ang = (((counter)*18) / rad2grad);
	byte x = (96 + (sin(ang) * 19));
	byte y = (34 - (cos(ang) * 19));
	u8g.drawLine(96, 34, x, y);

	// Signature
	u8g.setFont(u8g_font_6x12);
	u8g.drawStr(0, 63, "www.mauroalfieri.it");

	u8g.drawFrame(98, 7, 20, 50);
}

void Display_Class::main_screen()
{

	u8g.drawXBM(0, 0, 128, 64, frame_display);
	//u8g.setPrintPos(0, 0);
	u8g_uint_t xy = 0;

	u8g_uint_t h = 64;
	u8g_uint_t l = 128;
	//u8g.drawBitmapP(xy, xy, l, h, frame_display);

	//u8g.drawFrame(105, 7, 20, 50);
	//int h

	//byte kg_left = manage.tank_food_left / 1000;
	//map()
	//u8g.drawBox(105, 7, 20, 50);
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