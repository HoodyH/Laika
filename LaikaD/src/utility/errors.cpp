#include <stdint.h>
#include "arduino.h"
#include "../pins.h"
#include "../config.h"
#include "../sd/cardrw.h"
#include "../planner/manage.h"

#include "errors.h"
#include "buzzer.h"

bool Error_Class::system_status(uint16_t status_code)
{

	switch (status_code)
	{

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_INIZIALIZZAZONE				(x0xx)
	case FATAL_ERROR_9000:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("FATAL_ERROR_9000");
		card_rw.error_log("FATAL_ERROR_9000");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_SERBATOIO					(x1xx)
	case MAINTENANCE_2100:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("MAINTENANCE_2100");
		card_rw.error_log("MAINTENANCE_2100");
		manage.low_food_warning = true;
		return true; //ritorna vero perch� non � qualcoosa di vincolante
	}
	case FATAL_ERROR_9101:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("FATAL_ERROR_9101");
		card_rw.error_log("FATAL_ERROR_9101");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}
	case FATAL_ERROR_9102:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("FATAL_ERROR_9102");
		card_rw.error_log("FATAL_ERROR_9102");
		return true; //ritorna comunque vero per erogare quello che � rimasto nel tank
	}
	case FATAL_ERROR_9103:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("FATAL_ERROR_9103");
		card_rw.error_log("FATAL_ERROR_9103");
		manage.tank_food_left = 0;
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_DOSATORE					(x2xx)
	case FATAL_ERROR_9200:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("FATAL_ERROR_9200");
		card_rw.error_log("FATAL_ERROR_9200");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}
	case ERROR_0201:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_0201");
		card_rw.error_log("ERROR_0201");
		return false;
	}
	case CRITICAL_ERROR_8202:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("CRITICAL_ERROR_8202");
		card_rw.error_log("CRITICAL_ERROR_8202");
		return false;
	}
	case ERROR_0203:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_0203");
		card_rw.error_log("ERROR_0203");
		return false;
	}
	case ERROR_1204:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_1204");
		card_rw.error_log("ERROR_1204");
		return false;
	}
	case FATAL_ERROR_9205:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("FATAL_ERROR_9205");
		card_rw.error_log("FATAL_ERROR_9205");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_SISTEMA_DI_SCARICO			(x3xx)
	case CRITICAL_ERROR_8301:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("CRITICAL_ERROR_8301");
		card_rw.error_log("CRITICAL_ERROR_8301");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}
	case ERROR_0302:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_0302");
		card_rw.error_log("ERROR_0302");
		return false;
	}
	case ERROR_0303:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_0303");
		card_rw.error_log("ERROR_0303");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_ELETTRONICA					(x4xx)
	case CRITICAL_ERROR_8400:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("CRITICAL_ERROR_8400");
		card_rw.error_log("CRITICAL_ERROR_8400");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_GESTIONE_CICLI				(x5xx)
	case ERROR_1500:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_1500");
		card_rw.error_log("ERROR_1500");
		return false;
	}
	case ERROR_1501:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_1501");
		card_rw.error_log("ERROR_1501");
		return false;
	}
	case ERROR_0502:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("ERROR_0502");
		card_rw.error_log("ERROR_0502");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_SALVATAGGIO SU SD			(x6xx)
	case CRITICAL_ERROR_8601:
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println("CRITICAL_ERROR_8601");
		card_rw.error_log("CRITICAL_ERROR_8601");
		return false;
	}
	case FATAL_ERROR_9602:
	{
		if (DEBUG_SERIAL_PRINT_ON)
		{
			Serial.println("FATAL_ERROR_9602");
			Serial.println("Failed to read file baseconf.j, using default configuration");
		}
		card_rw.error_log("FATAL_ERROR_9602");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	default:
	{
		if (DEBUG_SERIAL_PRINT_ON)
		{
			Serial.println("some_unknown_error_has_occurred");
			Serial.println(status_code);
		}
		card_rw.error_log("some_unknown_error_has_occurred");
		card_rw.error_log(String(status_code));
		return false;
	}
	}

	if (status_code > 7999 && status_code < 9000) //errore 8xxx
		buzzer.play_melody(CRITICAL_ERROR_MELODY_SELCT);

	if (status_code > 8999 && status_code < 10000) //errore 9xxx
		buzzer.play_melody(FATAL_ERROR_MELODY_SELCT);
}
