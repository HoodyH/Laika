#include <stdio.h>
#include "arduino.h"
#include "../pins.h"
#include "../config.h"
#include "../utility/errors.h"
#include "../planner/manage.h"
#include "../dropper/motors.h"
#include "../dropper/loadcell.h"

#include "feed.h"

Motors_Class motors;
Load_Cell_Class load_cell;

unsigned long current_millis;

void Feed_Class::setup()
{
	motors.setup();

	pinMode(SWITCH_DOOR_DX_PIN, INPUT_PULLUP);
	pinMode(SWITCH_DOOR_SX_PIN, INPUT_PULLUP);
}

// drop the food from the container in to the weighing box
// when the weighing reach the target meal_qt_gr stop
bool Feed_Class::drop_and_weigh(int16_t meal_qt_gr)
{
	// prepare the system
	digitalWrite(PS_ON, HIGH);				  //accende l'alimentatore
	digitalWrite(MOTOR_MAIN_ENABLE_PIN, LOW); //accende il driver del motore
	load_cell.power_up();					  //accende il chip della cella di carico

	load_cell.setup();								  //ritara la cella di carico prima di ogni pesata
	manage.currently_weight = load_cell.get_weight(); //prima pesata che dovrebbe essere 0, serve per riscrivere la variabile dalla pesata precedente
	start_millis_weighing_timeout = millis();		  //inizializza con il tempo di start ciclo

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
			if (manage.currently_weight == 0)
				error.system_status(FATAL_ERROR_9103);
			return error.system_status(ERROR_1501);
		}

		// check the weight in the weighing box
		if ((current_millis - last_millis_load_cell_time_ceck) > LOAD_CELL_TIME_CECK)
		{
			last_millis_load_cell_time_ceck = current_millis; // update the last check time
			load_cell_reading = load_cell.get_weight();		  // get the weith from the cell

			Serial.println(load_cell_reading); //DEBUG

			// errors check
			if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
			{
				error.system_status(ERROR_0201); // soft error, spings are not strong enough

				// during the weigh procedure the fool leak out
				if (load_cell_reading < manage.currently_weight)
				{
					digitalWrite(MOTOR_MAIN_ENABLE_PIN, HIGH); //spegne il driver motore
					digitalWrite(PS_ON, LOW);				   //spegne l'alimentatore
					load_cell.power_down();					   //spegne il chip della cella di carico
					return error.system_status(CRITICAL_ERROR_8202);
				}
			}
			// update the current weighted value
			manage.currently_weight = load_cell_reading;
		} // end check the weight in the weighing box

		// calculate the motor speed
		// if the weight has reached a fixed value
		// start decrese the motor speed
		if (manage.currently_weight <= (meal_qt_gr - WEIGHT_FOR_FINAL_SPEED))
		{
			int16_t qt_gr_left = meal_qt_gr - manage.currently_weight;
			main_motor_rotation_per_min = map(qt_gr_left, WEIGHT_FOR_FINAL_SPEED, meal_qt_gr, MAIN_MOTOR_MIN_ROTATION_PER_MIN, MAIN_MOTOR_MAX_ROTATION_PER_MIN);
		}
		else
			main_motor_rotation_per_min = MAIN_MOTOR_FINAL_ROTATION_PER_MIN;

		// move the motor at certain speed
		// if sucess it will return true else the motor is stuck (fatal)
		if (!motors.move_stepper(main_motor_rotation_per_min)) // main_motor_rotation_per_min
			return false;

		// check if the weight has been reached
		// if yes stop the motor and leave the cicle
		if (manage.currently_weight >= meal_qt_gr)
		{
			manage.currently_weight = load_cell.get_weight(); //pesa un ultima volta dopo che il motore si � fermato

			digitalWrite(MOTOR_MAIN_ENABLE_PIN, HIGH); //spegne il driver del motore
			digitalWrite(PS_ON, LOW);				   //spegne l'alimentatore
			load_cell.power_down();					   //spegne il chip della cella di carico
			manage.status_drop_and_weigh = false;
			break;
		}

	} // end while cicle

	return true;
}

// check if the trapdoor is closed
// if not close it, check time out
bool Feed_Class::ceck_trapdoor_closed()
{

	motors.servo_attach(); // enable servo motors

	// Close the trapdoor
	motors.servo_move(SERVO_MOTOR_MIN_ANGLE, SERVO_MOTOR_CLOSE_SPEED);

	if ((current_millis - last_millis_ceck_trapdoor_closed) > TIME_TO_INITIAL_CLOSE_TRAPDOOR)
	{
		last_millis_ceck_trapdoor_closed = current_millis;

		if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
			return error.system_status(ERROR_0303);
		else
		{
			motors.servo_detach(); // disable servo motors
			return true;
		}
	}
	else
		return false;
}

bool Feed_Class::unload_food()
{

	motors.servo_attach();

	//apertura
	for (int16_t pos = SERVO_MOTOR_MIN_ANGLE; pos < SERVO_MOTOR_MAX_ANGLE; pos++)
	{
		if (pos > SERVO_MOTOR_IN_GRAD_SLOW_SPEED && pos < SERVO_MOTOR_OUT_GRAD_SLOW_SPEED)
			servo_speed = SERVO_MOTOR_SLOW_OPEN_SPEED;
		else
			servo_speed = SERVO_MOTOR_FAST_OPEN_SPEED;

		motors.servo_move(pos, servo_speed);
	}
	//check if the trapdoor is open
	if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_CLOSE || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_CLOSE)
	{
		motors.servo_detach();
		return error.system_status(CRITICAL_ERROR_8301);
	}
	//chiusura
	for (int16_t pos = SERVO_MOTOR_MAX_ANGLE; pos > SERVO_MOTOR_MIN_ANGLE; pos--)
	{
		motors.servo_move(pos, SERVO_MOTOR_CLOSE_SPEED);
	}

	//controllo se � chiuso
	if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
	{
		error.system_status(ERROR_0302);
		unload_food();
	}
	else
	{
		//scollega servomotori
		motors.servo_detach();
		return true;
	}
}


// public function to drop a certain amount of food into the bowl
bool Feed_Class::feed(int16_t meal_qt_gr)
{
	start_millis_feed_timeout = millis(); //inizializza con il tempo di start ciclo

	// nel caso in cui salti la corrente o il ciclo fallisca, continua a pesare da dove era rimasto
	meal_qt_gr -= (manage.total_currently_weight + manage.currently_weight);

	manage.status_drop_and_weigh = true;
	do
	{
		current_millis = millis();

		// check if the process has gone in timeout
		if ((current_millis - start_millis_feed_timeout) > FEED_TIMEOUT)
			return error.system_status(ERROR_1500);

		if (ceck_trapdoor_closed())
		{
			// maniplulable var with the fraction of food that will be dropped in this cicle
			int16_t future_meal_qt_gr = meal_qt_gr; 

			//check if the given qt it's not too big for the WEIGHING COLLECTOR
			if (future_meal_qt_gr > MAX_WEIGHT_IN_WEIGHING_COLLECTOR + OVER_WEIGHT_IN_WEIGHING_COLLECTOR)
				future_meal_qt_gr = MAX_WEIGHT_IN_WEIGHING_COLLECTOR;

			// drop and weighing
			// check for errors
			if (!drop_and_weigh(future_meal_qt_gr))
				return false;

			// update the variable with the amount weighted on this cicle
			manage.total_currently_weight += manage.currently_weight;
			meal_qt_gr -= manage.currently_weight;

			// unload the food from the WEIGHING COLLECTOR into the bowl
			// check for errors
			if (!unload_food())
				return false;

			Serial.print("Crocchette Pesate: ");		   //DEBUG
			Serial.println(manage.total_currently_weight); //DEBUG
		}

	} while (meal_qt_gr > 0);

	return true;
}