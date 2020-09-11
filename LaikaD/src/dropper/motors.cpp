#include "arduino.h"
#include <Servo.h>
#include "../pins.h"
#include "../config.h"
#include "../utility/errors.h"

#include "motors.h"

//encoder.
//necessario per rendere la funzione statica e quindi modificabile solo dalla funzione attachInterrupt
int16_t encoder_count_value = COUNT_VALUE_RESET;
int32_t encoder_total_cicle_count_value = COUNT_VALUE_RESET;
byte last_encoder_reading;

void update_encoder()
{

	byte MSB = digitalRead(ENCODER_MAIN_ENA_PIN);
	byte LSB = digitalRead(ENCODER_MAIN_ENB_PIN);

	byte actual_encoder_reading = (MSB << 1) | LSB;
	byte state = (last_encoder_reading << 2) | actual_encoder_reading; //esegue l'end tra il precedente traslato di 2 e quello appena acquisito

	if (state == 0b1101 || state == 0b0100 || state == 0b0010 || state == 0b1011)
	{
		encoder_count_value++;
		encoder_total_cicle_count_value++;
	}
	if (state == 0b1110 || state == 0b0111 || state == 0b0001 || state == 0b1000)
	{
		encoder_count_value--;
		encoder_total_cicle_count_value--;
	}

	last_encoder_reading = actual_encoder_reading; //memorizza ultimo stato
}

void Motors_Class::setup()
{

	//stepper main setup
	pinMode(MOTOR_MAIN_STEP_PIN, OUTPUT);
	pinMode(MOTOR_MAIN_DIR_PIN, OUTPUT);
	pinMode(MOTOR_MAIN_ENABLE_PIN, OUTPUT);
	//pinMode(MOTOR_MAIN_CS_PIN, OUTPUT);

	//stepper main setup
	pinMode(MOTOR_DOSER_STEP_PIN, OUTPUT);
	pinMode(MOTOR_DOSER_DIR_PIN, OUTPUT);
	pinMode(MOTOR_DOSER_ENABLE_PIN, OUTPUT);
	//pinMode(MOTOR_DOSER_CS_PIN, OUTPUT);

	//encoder
	pinMode(ENCODER_MAIN_ENA_PIN, INPUT_PULLUP);
	pinMode(ENCODER_MAIN_ENB_PIN, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(ENCODER_MAIN_ENA_PIN), update_encoder, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENCODER_MAIN_ENB_PIN), update_encoder, CHANGE);
}

bool Motors_Class::move_stepper(uint16_t rotation_per_min)
{

	// move the motor forward at a given speed
	// if the motor is blocked move the motor counter clockwise for a certain amount

	//					  secondi to microsec / rotazioni                  quanti step deve fare per un giro
	microsec_half_step = ((60 * 1000000 / rotation_per_min) / (MAIN_MOTOR_STEP_PER_ROTATION * MAIN_MOTOR_MICROSTEP)) / 2;

	difference_encoder_stepper = float(abs(step_count_value)) - float(abs(encoder_count_value)) * (float(MAIN_MOTOR_STEP_PER_ROTATION) * float(MAIN_MOTOR_MICROSTEP) / float(ENCODER_MAIN_PULSE_PER_ROTATION)) - ADJ_COEFICENT;

	Serial.println(move_backwards); //DEBUG
	if (difference_encoder_stepper > MAX_DIF_ENCODER_STEPPER * MAIN_MOTOR_MICROSTEP || difference_encoder_stepper < -MAX_DIF_ENCODER_STEPPER * MAIN_MOTOR_MICROSTEP)
	{
		// il motore si e' bloccato procedere all'inversione di marcia
		error.system_status(ERROR_0203);
		move_backwards = true;
		backwards_steps_count = MAIN_MOTOR_STD_BACKWARDS_ROTATION * MAIN_MOTOR_MICROSTEP;
		encoder_count_value = step_count_value = COUNT_VALUE_RESET;
	}
	else
	{
		// all is right rotate the motor clockwise
		digitalWrite(MOTOR_MAIN_DIR_PIN, HIGH);
	}

	// the rotor is stuck, move the motor counter clockwise
	if (move_backwards)
	{
		digitalWrite(MOTOR_MAIN_DIR_PIN, LOW);
		if (backwards_steps_count <= 0)
		{
			// when the backwards_steps_count has reached 0 move the motor forward again
			move_backwards = false;
			encoder_count_value = step_count_value = COUNT_VALUE_RESET;
		}
	}

	/*
	if ((millis() - last_micros_motor_blocked) > MOTOR_BLOCKED_TIME_CECK)
	{
		last_micros_motor_blocked = millis();
		if (encoder_count_value == last_encoder_count_value)
		{
			motor_blocked_error_count++;
			error.system_status(ERROR_1204);
			if (motor_blocked_error_count >= TIMES_ERROR_BLOCK_BEFORE_STOP)
				return error.system_status(FATAL_ERROR_9205);
		}
		else
			motor_blocked_error_count = 0;
		last_encoder_count_value = encoder_count_value;
	}
	*/

	if ((micros() - last_micros_stepper) > microsec_half_step)
	{
		digitalWrite(MOTOR_MAIN_STEP_PIN, HIGH);
	}
	if ((micros() - last_micros_stepper) > microsec_half_step * 2)
	{
		last_micros_stepper = micros();
		digitalWrite(MOTOR_MAIN_STEP_PIN, LOW);
		step_count_value++;
		backwards_steps_count--;
	}

	return true;
}

void Motors_Class::servo_move(int16_t grade, int8_t speed, int16_t *actual_dx_position, int16_t *actual_sx_position)
{
	servo_delay = (1000 / speed);

	servo_door_dx.write(grade);
	servo_door_sx.write(map(grade, SERVO_MOTOR_MIN_ANGLE, SERVO_MOTOR_MAX_ANGLE, SERVO_MOTOR_MAX_ANGLE, SERVO_MOTOR_MIN_ANGLE));

	//actual_dx_position = servo_door_dx.read();
	//actual_sx_position = servo_door_sx.read();

	delay(servo_delay);
}

void Motors_Class::servo_attach()
{
	servo_door_dx.attach(SERVO_DOOR_DX_PIN);
	servo_door_sx.attach(SERVO_DOOR_SX_PIN);
}
void Motors_Class::servo_detach()
{
	servo_door_dx.detach();
	servo_door_sx.detach();
}
