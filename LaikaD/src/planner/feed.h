// feed.h

#ifndef _FEED_h
#define _FEED_h

class Feed_Class
{
private:
	// time for the timeout
	long start_millis_feed_timeout;
	long start_millis_weighing_timeout;

	// the current weight in the single weight loop
	int16_t currently_weight;

	// the weight erogated to the bowl in all the loops
	// read this value after drop_and_weigh function execution
	int16_t total_currently_weight;
	int16_t total_target_weight;

	// internal function to do one drop and weight cicle
	bool drop_and_weigh(int16_t meal_qt_gr);

public:
	void setup();
	bool close_trapdoor();
	bool open_trapdoor();

	bool feed(int16_t meal_qt_gr);

	int16_t get_total_currently_weight();
};
extern Feed_Class feed;

#endif
