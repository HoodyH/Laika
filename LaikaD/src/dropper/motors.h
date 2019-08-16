// motors.h

#ifndef _MOTORS_h
#define _MOTORS_h

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "arduino.h"
	#else
		#include "WProgram.h"
	#endif



#define ADJ_COEFICENT							4	//int
#define COUNT_VALUE_RESET						1   //int
#define MOTOR_BLOCKED_TIME_CECK					200 //millis
#define TIMES_ERROR_BLOCK_BEFORE_STOP			3
class Motors_Class
{
 protected:
	 
	int16_t microsec_half_step;
	long last_micros_stepper = 0;
	
	long last_micros_motor_blocked = 0;
	int16_t last_encoder_count_value = COUNT_VALUE_RESET;
	byte motor_blocked_error_count = 0;

	int16_t step_count_value = COUNT_VALUE_RESET;
	float difference_encoder_stepper;
	int16_t backwards_steps_count = 0;
	bool move_backwards = false;

	
	

	int16_t servo_delay;

	int pos = 0;

	//void update_encoder();

 public:

	Servo servo_door_dx;
	Servo servo_door_sx;

	void setup();

	bool move_stepper(uint16_t rotation_per_min);
	void servo_move(int16_t grade, int8_t speed, int16_t *actual_dx_position, int16_t *actual_sx_position);

	void servo_attach();
	void servo_detach();

}; extern Motors_Class motors;

#endif

