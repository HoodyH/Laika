
#ifndef _DEBUG_h
#define _DEBUG_h

#include "arduino.h"
#include "../config.h"

#if DEBUG_SERIAL_PRINT_ON
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#else
#define DEBUG_PRINTLN(...)
#define DEBUG_PRINT(...)
#endif

#endif