// feed.h

#ifndef _FEED_h
#define _FEED_h

class Feed_Class
{
private:
	//tempi per funzioni cicliche
	long last_millis_load_cell_time_ceck = 0;
	long last_millis_ceck_trapdoor_closed = 0;
	//tempi per il timeout
	long start_millis_feed_timeout;
	long start_millis_weighing_timeout;

	int16_t load_cell_reading;

	int16_t main_motor_rotation_per_min;

	int16_t servo_speed;

public:
	void setup();
	bool drop_and_weigh(int16_t meal_qt_gr);
	bool ceck_trapdoor_closed();
	bool unload_food();

	bool feed(int16_t meal_qt_gr);
};
extern Feed_Class feed;

#endif
