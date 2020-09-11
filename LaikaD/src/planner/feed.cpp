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

void Feed_Class::setup()
{

	motors.setup();

	pinMode(SWITCH_DOOR_DX_PIN, INPUT_PULLUP);
	pinMode(SWITCH_DOOR_SX_PIN, INPUT_PULLUP);
}

bool Feed_Class::drop_and_weigh(int16_t meal_qt_gr)
{
	if (manage.setup_drop_and_weigh)
	{

		//controlla se la quantit� da pesare sta nel contenitore di pesatura se no divide la pesata
		if (meal_qt_gr > MAX_WEIGHT_IN_WEIGHING_COLLECTOR + OVER_WEIGHT_IN_WEIGHING_COLLECTOR)
			meal_qt_gr = MAX_WEIGHT_IN_WEIGHING_COLLECTOR;

		digitalWrite(PS_ON, HIGH);				  //accende l'alimentatore
		digitalWrite(MOTOR_MAIN_ENABLE_PIN, LOW); //accende il driver del motore
		load_cell.power_up();					  //accende il chip della cella di carico

		load_cell.setup();								  //ritara la cella di carico prima di ogni pesata
		manage.currently_weight = load_cell.get_weight(); //prima pesata che dovrebbe essere 0, serve per riscrivere la variabile dalla pesata precedente
		start_millis_weighing_timeout = millis();		  //inizializza con il tempo di start ciclo

		manage.setup_drop_and_weigh = false;
	}

	if (manage.status_drop_and_weigh)
	{

		if ((millis() - last_millis_load_cell_time_ceck) > LOAD_CELL_TIME_CECK)
		{
			last_millis_load_cell_time_ceck = millis();
			load_cell_reading = load_cell.get_weight();
			if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
			{
				error.system_status(ERROR_0201);

				if (load_cell_reading < manage.currently_weight)
				{
					digitalWrite(MOTOR_MAIN_ENABLE_PIN, HIGH); //spegne il driver motore
					digitalWrite(PS_ON, LOW);				   //spegne l'alimentatore
					load_cell.power_down();					   //spegne il chip della cella di carico
					return error.system_status(CRITICAL_ERROR_8202);
				}
			}
			manage.currently_weight = load_cell_reading;
		}

		//calcola la velocit� a cui deve andare il motore sulla base di quanto deve pesare.
		if (manage.currently_weight <= (meal_qt_gr - WEIGHT_FOR_FINAL_SPEED))
		{
			main_motor_rotation_per_min = meal_qt_gr - manage.currently_weight;
			main_motor_rotation_per_min = map(main_motor_rotation_per_min, WEIGHT_FOR_FINAL_SPEED, meal_qt_gr, MAIN_MOTOR_MIN_ROTATION_PER_MIN, MAIN_MOTOR_MAX_ROTATION_PER_MIN);
		}
		else
			main_motor_rotation_per_min = MAIN_MOTOR_FINAL_ROTATION_PER_MIN;

		if (!motors.move_stepper(main_motor_rotation_per_min)) //da il comando di muovere il motore
			return false;

		if ((millis() - start_millis_weighing_timeout) > WEIGHING_TIMEOUT)
		{
			if (manage.currently_weight == 0)
				error.system_status(FATAL_ERROR_9103);
			return error.system_status(ERROR_1501);
		}
	}

	if (manage.currently_weight >= meal_qt_gr)
	{

		manage.currently_weight = load_cell.get_weight(); //pesa un ultima volta dopo che il motore si � fermato

		digitalWrite(MOTOR_MAIN_ENABLE_PIN, HIGH); //spegne il driver del motore
		digitalWrite(PS_ON, LOW);				   //spegne l'alimentatore
		load_cell.power_down();					   //spegne il chip della cella di carico
		manage.status_drop_and_weigh = false;
	}

	return true;
}

bool Feed_Class::ceck_trapdoor_closed()
{

	motors.servo_attach();

	if ((millis() - last_millis_ceck_trapdoor_closed) > TIME_TO_INITIAL_CLOSE_TRAPDOOR)
	{
		last_millis_ceck_trapdoor_closed = millis();

		motors.servo_move(SERVO_MOTOR_MIN_ANGLE, SERVO_MOTOR_CLOSE_SPEED, &servo_dx_position, &servo_sx_position);

		if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_OPEN || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_OPEN)
			return error.system_status(ERROR_0303);
		else
		{
			//scollega servomotori
			motors.servo_detach();
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

		motors.servo_move(pos, servo_speed, &servo_dx_position, &servo_sx_position);
	}
	//controllo se � aperto
	if (digitalRead(SWITCH_DOOR_DX_PIN) == SWITCH_DOOR_CLOSE || digitalRead(SWITCH_DOOR_SX_PIN) == SWITCH_DOOR_CLOSE)
	{
		motors.servo_detach();
		return error.system_status(CRITICAL_ERROR_8301);
	}
	//chiusura
	for (int16_t pos = SERVO_MOTOR_MAX_ANGLE; pos > SERVO_MOTOR_MIN_ANGLE; pos--)
	{
		motors.servo_move(pos, SERVO_MOTOR_CLOSE_SPEED, &servo_dx_position, &servo_sx_position);
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

bool Feed_Class::feed(int16_t meal_qt_gr)
{

	start_millis_feed_timeout = millis(); //inizializza con il tempo di start ciclo
	//nel caso in cui salti la corrente o il ciclo fallisca, continua a pesare da dove era rimasto
	meal_qt_gr -= (manage.total_currently_weight + manage.currently_weight);

	manage.status_drop_and_weigh = true;
	do
	{

		if (ceck_trapdoor_closed())
		{

			//controlla se la quantit� da pesare sta nel contenitore di pesatura se no divide la pesata
			if (meal_qt_gr > MAX_WEIGHT_IN_WEIGHING_COLLECTOR + OVER_WEIGHT_IN_WEIGHING_COLLECTOR)
				//nel caso che il peso sia maggiore dei OVER_WEIGHT_IN_WEIGHING_COLLECTOR esegue una pesata ricorsiva
				if (manage.status_drop_and_weigh)
					drop_and_weigh(MAX_WEIGHT_IN_WEIGHING_COLLECTOR);
				else if (manage.status_drop_and_weigh)
					drop_and_weigh(meal_qt_gr);

			if (!manage.status_drop_and_weigh)
			{
				manage.total_currently_weight += manage.currently_weight;
				meal_qt_gr -= manage.currently_weight;

				Serial.print("Crocchette Pesate: ");		   //DEBUG
				Serial.println(manage.total_currently_weight); //DEBUG
				Serial.println("unload");					   //DEBUG

				if (!unload_food())
					return false; //l'errore viene segnalato in unload food
			}
		}

		if ((millis() - start_millis_feed_timeout) > FEED_TIMEOUT)
			return error.system_status(ERROR_1500);

	} while (meal_qt_gr > 0);

	return true;
}