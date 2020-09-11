/*
Arduino mega interupt pin
2, 3, 18, 19, 20, 21
*/

#ifndef _PINS_H
#define _PINS_H

//TANK--------------------------------------------------------------------------
#define PROX_SENSOR_1_PIN 11 //SERVOS 1 sensore organico serbatorio

//HOPPER------------------------------------------------------------------------
//motor (X soket on ramps 1.4)
#define MOTOR_MAIN_STEP_PIN 54
#define MOTOR_MAIN_DIR_PIN 55
#define MOTOR_MAIN_ENABLE_PIN 38
#define MOTOR_MAIN_CS_PIN 53
//encoder
#define ENCODER_MAIN_ENA_PIN 2 //X- ENDSTOP  interupt
#define ENCODER_MAIN_ENB_PIN 3 //X+ ENDSTOP  interupt

//WEIGHTS AND RELEASES SYSTEM---------------------------------------------------
//load cell AUX2
#define LOAD_CELL_DT_DX_PIN 63  //A9 AUX2
#define LOAD_CELL_SCK_DX_PIN 59 //A5 AUX2
#define LOAD_CELL_DT_SX_PIN 65  //A11 AUX2
#define LOAD_CELL_SCK_SX_PIN 66 //A12 AUX2
//servo
#define SERVO_DOOR_DX_PIN 5 //SERVOS 3
#define SERVO_DOOR_SX_PIN 4 //SERVOS 4
//failsafe switch
#define SWITCH_DOOR_DX_PIN 14 //Y- ENDSTOP
#define SWITCH_DOOR_SX_PIN 15 //Y+ ENDSTOP

//VIBRATION SYSTEM--------------------------------------------------------------
#define MOTOR_VIB_DX_PIN 8 //BIG MOSFET D8

//FULL GRAPHIC SMART CONTROLLER-------------------------------------------------
//display
#define LCD_RS_PIN 16
#define LCD_RW_PIN 17
#define LCD_ENABLE_PIN 23
#define LCD_BACKLIGHT_PIN 39
//sd
#define SD_DETECT_PIN 49
#define SD_MISO_PIN 50
#define SD_MOSI_PIN 51
#define SD_SCK_PIN 52
#define SD_CS_PIN 53
//encoder
#define BTN_EN1_PIN 31
#define BTN_EN2_PIN 33
#define BTN_ENC_PIN 35
//buzzer
#define BEEPER_PIN 37
//killswitch
#define KILL_PIN 41

//MAINRBOARD--------------------------------------------------------------------
#define LED_BOARD_PIN 13
//rtc
#define RTC_SDA_PIN 20
#define RTC_SCL_PIN 21
//led_out
#define LED_RGB_PIN
//Power Supply
#define PS_ON 12

//DAILY DOSER-------------------------------------------------------------------
//motor
#define MOTOR_DOSER_STEP_PIN 60
#define MOTOR_DOSER_DIR_PIN 61
#define MOTOR_DOSER_ENABLE_PIN 56
#define MOTOR_DOSER_CS_PIN 49
//actuator
#define ACTUATOR_PIN 9 //BIG MOSFET D9

//another motor pinout
//motor (Z soket on ramps 1.4)
#define MOTOR_OTHER_STEP_PIN 26
#define MOTOR_OTHER_DIR_PIN 28
#define MOTOR_OTHER_ENABLE_PIN 24
//#define MOTOR_OTHER_CS_PIN        ??

//END CONFIG--------------------------------------------------------------------

#endif
