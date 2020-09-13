#include <stdint.h>
#include "arduino.h"
#include "../pins.h"
#include "../config.h"
#include "../sd/cardrw.h"
#include "../planner/manage.h"

#include "errors.h"
#include "buzzer.h"
#include "debug.h"

bool Error_Class::system_status(uint16_t status_code)
{

	switch (status_code)
	{

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_INIZIALIZZAZONE				(x0xx)
	case FATAL_ERROR_9000:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9000");
		card_rw.error_log("FATAL_ERROR_9000");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_SERBATOIO					(x1xx)
	case MAINTENANCE_2100:
	{
		DEBUG_PRINTLN("MAINTENANCE_2100");
		card_rw.error_log("MAINTENANCE_2100");
		manage.low_food_warning = true;
		return true; //ritorna vero perch� non � qualcoosa di vincolante
	}
	case FATAL_ERROR_9101:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9101");
		card_rw.error_log("FATAL_ERROR_9101");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}
	case FATAL_ERROR_9102:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9102");
		card_rw.error_log("FATAL_ERROR_9102");
		return true; //ritorna comunque vero per erogare quello che � rimasto nel tank
	}
	case FATAL_ERROR_9103:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9103");
		card_rw.error_log("FATAL_ERROR_9103");
		manage.tank_food_left = 0;
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_DOSATORE					(x2xx)
	case FATAL_ERROR_9200:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9200");
		card_rw.error_log("FATAL_ERROR_9200");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}
	case ERROR_0201:
	{
		DEBUG_PRINTLN("ERROR_0201");
		card_rw.error_log("ERROR_0201");
		return false;
	}
	case CRITICAL_ERROR_8202:
	{
		DEBUG_PRINTLN("CRITICAL_ERROR_8202");
		card_rw.error_log("CRITICAL_ERROR_8202");
		return false;
	}
	case ERROR_0203:
	{
		DEBUG_PRINTLN("ERROR_0203");
		card_rw.error_log("ERROR_0203");
		return false;
	}
	case ERROR_1204:
	{
		DEBUG_PRINTLN("ERROR_1204");
		card_rw.error_log("ERROR_1204");
		return false;
	}
	case FATAL_ERROR_9205:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9205");
		card_rw.error_log("FATAL_ERROR_9205");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_SISTEMA_DI_SCARICO			(x3xx)
	case CRITICAL_ERROR_8301:
	{
		DEBUG_PRINTLN("CRITICAL_ERROR_8301");
		card_rw.error_log("CRITICAL_ERROR_8301");
		manage.error_occur_in_feed = true; //blocca il ciclo di erogazione food()
		return false;
	}
	case ERROR_0302:
	{
		DEBUG_PRINTLN("ERROR_0302");
		card_rw.error_log("ERROR_0302");
		return false;
	}
	case ERROR_0303:
	{
		DEBUG_PRINTLN("ERROR_0303");
		card_rw.error_log("ERROR_0303");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_ELETTRONICA					(x4xx)
	case CRITICAL_ERROR_8400:
	{
		DEBUG_PRINTLN("CRITICAL_ERROR_8400");
		card_rw.error_log("CRITICAL_ERROR_8400");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_GESTIONE_CICLI				(x5xx)
	case ERROR_1500:
	{
		DEBUG_PRINTLN("ERROR_1500");
		card_rw.error_log("ERROR_1500");
		return false;
	}
	case ERROR_1501:
	{
		DEBUG_PRINTLN("ERROR_1501");
		card_rw.error_log("ERROR_1501");
		return false;
	}
	case ERROR_0502:
	{
		DEBUG_PRINTLN("ERROR_0502");
		card_rw.error_log("ERROR_0502");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	//ERRORE_DI_SALVATAGGIO SU SD			(x6xx)
	case CRITICAL_ERROR_8601:
	{
		DEBUG_PRINTLN("CRITICAL_ERROR_8601");
		card_rw.error_log("CRITICAL_ERROR_8601");
		return false;
	}
	case FATAL_ERROR_9602:
	{
		DEBUG_PRINTLN("FATAL_ERROR_9602");
		DEBUG_PRINTLN("Failed to read file baseconf.j, using default configuration");
		card_rw.error_log("FATAL_ERROR_9602");
		return false;
	}

	//-------------------------------------------------------------------------------------------------
	default:
	{
		DEBUG_PRINTLN("some_unknown_error_has_occurred");
		DEBUG_PRINTLN(status_code);
		card_rw.error_log("some_unknown_error_has_occurred");
		card_rw.error_log(String(status_code));
		return false;
	}
	}

	if (status_code > 7999 && status_code < 9000) //errore 8xxx
		buzzer.play_melody(CRITICAL_ERROR_MELODY_SELECT);

	if (status_code > 8999 && status_code < 10000) //errore 9xxx
		buzzer.play_melody(FATAL_ERROR_MELODY_SELECT);
}
