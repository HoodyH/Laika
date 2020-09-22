#include <stdio.h>
#include "arduino.h"
#include "../pins.h"
#include "../config.h"
#include "../utility/errors.h"
#include "../utility/debug.h"
#include "../dropper/motors.h"
#include "../dropper/loadcell.h"
#include "../lcd/display.h"

#include "feed.h"

Motors_Class motors;
Load_Cell_Class load_cell;

unsigned long current_millis;

void Feed_Class::setup()
{
	motors.setup();

	// set the power suppy
	pinMode(PS_ON, OUTPUT);

	pinMode(SWITCH_DOOR_DX_PIN, INPUT_PULLUP);
	pinMode(SWITCH_DOOR_SX_PIN, INPUT_PULLUP);

	open_trapdoor(); // it will not power up the power supply
}

// drop the food from the container in to the weighing box
// when the weighing reach the target meal_qt_gr stop
bool Feed_Class::drop_and_weigh(int16_t meal_qt_gr)
{
	DEBUG_PRINT("Drop and weigh: ");
	DEBUG_PRINTLN(meal_qt_gr);

	// define local vars
	int16_t load_cell_reading;
	int16_t main_motor_rotation_per_min;
	unsigned long last_millis_load_cell_time_ceck = 0;

	// prepare the system
	digitalWrite(MOTOR_MAIN_ENABLE_PIN, LOW); // power up motor driver
	load_cell.power_up();					  // power up the load cell
	load_cell.setup();						  // ritara la cella di carico prima di ogni pesata

	// first weight to set the tare, and to reset currently weight value from the previous loop
	currently_weight = load_cell.get_weight();

	// set the weighing start time
	start_millis_weighing_timeout = millis();

	// this will go false if there are problems
	bool is_success = true;

	// drop and weight
	while (true)
	{
		// update current_millis in this loop
		// millis it's a time consuming operation that will be used several times
		// store it for future use
		current_millis = millis();

		// the weighing process have a time limit, after that it will be aborted
		// check before do anything
		if ((current_millis - start_millis_weighing_timeout) > WEIGHING_TIMEOUT)
		{
			if (currently_weight == 0)
				error.system_status(FATAL_ERROR_9103);

			error.system_status(ERROR_1501);
			is_success = false;
			break;
		}

		// check the weight in the weighing box
		if ((current_millis - last_millis_load_cell_time_ceck) > LOAD_CELL_TIME_CECK)
		{
			last_millis_load_cell_time_ceck = current_millis; // update the last check time
			load_cell_reading = load_cell.get_weight();		  // get the weith from the cell

			// errors check
			if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
			{
				error.system_status(ERROR_0201); // soft error, spings are not strong enough

				// during the weigh procedure the fool leak out
				if (load_cell_reading < currently_weight)
				{
					error.system_status(CRITICAL_ERROR_8202);
					is_success = false;
					break;
				}
			}
			// update the current weighted value and display it
			currently_weight = load_cell_reading;
			display_food_val(meal_qt_gr, currently_weight);
		} // end check the weight in the weighing box

		// calculate the motor speed
		// if the weight has reached a fixed value
		// start decrese the motor speed
		if (currently_weight <= (meal_qt_gr - WEIGHT_FOR_FINAL_SPEED))
		{
			int16_t qt_gr_left = meal_qt_gr - currently_weight;
			main_motor_rotation_per_min = map(qt_gr_left, WEIGHT_FOR_FINAL_SPEED, meal_qt_gr, MAIN_MOTOR_MIN_ROTATION_PER_MIN, MAIN_MOTOR_MAX_ROTATION_PER_MIN);
		}
		else
			main_motor_rotation_per_min = MAIN_MOTOR_FINAL_ROTATION_PER_MIN;

		// move the motor at certain speed
		// if sucess it will return true else the motor is stuck (fatal)
		if (!motors.move_stepper(main_motor_rotation_per_min)) // main_motor_rotation_per_min
		{
			is_success = false;
			break;
		}

		// check if the weight has been reached
		// if yes stop the motor and leave the cicle
		if (currently_weight >= meal_qt_gr)
		{
			currently_weight = load_cell.get_weight(); // check the weight one more time at the end
			motors.move_stepper_back(); // move the motor to the rest position
			break;
		}

	} // end while cicle

	digitalWrite(MOTOR_MAIN_ENABLE_PIN, HIGH); //shutdown motor driver
	load_cell.power_down();					   //shutdown load cell chip

	// if no error has occured, return success else no
	if (is_success)
		return true;
	else
		return false;
}

// check if the trapdoor is closed
// if not close it, check time out
bool Feed_Class::close_trapdoor()
{
	DEBUG_PRINTLN("Close trapdore");

	motors.servo_attach();											   // enable servo motors
	motors.servo_move(SERVO_MOTOR_MIN_ANGLE, SERVO_MOTOR_CLOSE_SPEED); // close the trapdoor
	motors.servo_detach();											   // disable servo motors

	// check if the trapdoor is closed, if not throw an error
	// and try to fix the problem reopening and closing the trapdoor TO DO
	if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
	{
		return error.system_status(ERROR_0303);
	}
	else
		return true;
}

bool Feed_Class::open_trapdoor()
{
	DEBUG_PRINTLN("Unload Food!");
	motors.servo_attach();
	motors.start_vibration();

	// open the trapdoor
	// servo_move function has a delay that let make funcion weith the motor to move
	DEBUG_PRINTLN("Open trapdoor");

	int16_t servo_speed;

	for (int16_t pos = SERVO_MOTOR_MIN_ANGLE; pos < SERVO_MOTOR_MAX_ANGLE; pos++)
	{
		if (pos > SERVO_MOTOR_IN_GRAD_SLOW_SPEED && pos < SERVO_MOTOR_OUT_GRAD_SLOW_SPEED)
			servo_speed = SERVO_MOTOR_SLOW_OPEN_SPEED;
		else
			servo_speed = SERVO_MOTOR_FAST_OPEN_SPEED;

		// move at differents speeds
		motors.servo_move(pos, servo_speed);
	}

	// check if the trapdoor is open
	if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_CLOSE || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_CLOSE)
	{
		motors.servo_detach();
		return error.system_status(CRITICAL_ERROR_8301);
	}
	// close trapdoor
	DEBUG_PRINTLN("Close trapdoor");
	for (int16_t pos = SERVO_MOTOR_MAX_ANGLE; pos > SERVO_MOTOR_MIN_ANGLE; pos--)
		motors.servo_move(pos, SERVO_MOTOR_CLOSE_SPEED);

	// check if the trapdoor has closed
	if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
	{
		error.system_status(ERROR_0302);
		// loop the unload food if the trapdoor failed to close
		open_trapdoor();
	}
	else
	{
		DEBUG_PRINTLN("Food Unloaded");
		// stop servos and return success
		motors.servo_detach();
		motors.stop_vibration();
		return true;
	}
}

// public function to drop a certain amount of food into the bowl
bool Feed_Class::feed(int16_t meal_qt_gr)
{
	start_millis_feed_timeout = millis(); // inizialize on the start of the cicle
	digitalWrite(PS_ON, HIGH);			  // power up the power supply

	do
	{
		current_millis = millis();

		// check if the process has gone in timeout
		if ((current_millis - start_millis_feed_timeout) > FEED_TIMEOUT)
			return error.system_status(ERROR_1500);

		if (close_trapdoor())
		{
			// maniplulable var with the fraction of food that will be dropped in this cicle
			int16_t future_meal_qt_gr = meal_qt_gr;

			//check if the given qt it's not too big for the WEIGHING COLLECTOR
			if (future_meal_qt_gr > MAX_WEIGHT_IN_WEIGHING_COLLECTOR + OVER_WEIGHT_IN_WEIGHING_COLLECTOR)
				future_meal_qt_gr = MAX_WEIGHT_IN_WEIGHING_COLLECTOR;

			// drop and weighing
			// save error errors
			DEBUG_PRINT("Future meal to be weighted: ");
			DEBUG_PRINTLN(future_meal_qt_gr);
			bool drop_and_weigh_error = drop_and_weigh(future_meal_qt_gr);

			// update the variable with the amount weighted on this cicle
			total_currently_weight += currently_weight;
			meal_qt_gr -= currently_weight;

			// unload the food from the WEIGHING COLLECTOR into the bowl
			// check for errors
			if (!open_trapdoor())
			{
				digitalWrite(PS_ON, LOW); // shutdown power supply
				return false;
			}

			DEBUG_PRINT("Food total weight: ");
			DEBUG_PRINTLN(total_currently_weight);

			// error during drop_and_weigh, exit the cicle and do not iterate again.
			// unload, save weighed value and exit
			if (!drop_and_weigh_error)
			{
				digitalWrite(PS_ON, LOW); // shutdown power supply
				return false;
			}
		}

	} while (meal_qt_gr > 0);

	digitalWrite(PS_ON, LOW); // shutdown power supply
	return true;
}