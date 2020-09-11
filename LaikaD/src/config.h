#ifndef _CONFIG_H
#define _CONFIG_H

//configurazione motore

#define MAIN_MOTOR_CURRENT 1000          //mA
#define MAIN_MOTOR_STEP_PER_ROTATION 200 //steps
#define MAIN_MOTOR_MICROSTEP 16          //1/x in questo caso --> ogni 16 step software equivale uno reale del motore

#define LOAD_CELL_ADJ_FULL_SCALE 850 //grams  , aggiungere qualche grammo in pi� per una pesata pi� generosa (tener conto imprecisione cella)

#define ENCODER_MAIN_PULSE_PER_ROTATION 2400 //pulse ,   resolution of the encoder

//doser config

//funzione drop
#define MAIN_MOTOR_MAX_ROTATION_PER_MIN 30  //rotation*min
#define MAIN_MOTOR_MIN_ROTATION_PER_MIN 4   //rotation*min
#define MAIN_MOTOR_FINAL_ROTATION_PER_MIN 1 //rotation*min

//valore in grammi, ultimi grami in cui il motore gira alla velocit� minima
#define WEIGHT_FOR_FINAL_SPEED 50 //grams

//ogni quanti milliseconsd viene controllata la cella di carico durante drop()
#define LOAD_CELL_TIME_CECK 400 //milliseconds
//valore entro il quale la cella di carico pu� misurare un valore inferiore all'ultimo misurato. Per evitare il rimbalzo.
//#define LOAD_CELL_OSCILLATION_VALUE			6		//grams  attualmente non utilizzato

//in step reali del motore  (se il motore � da 200 step allora 50 = 1/4 di giro)
//quanto il motore pu� tornare in dietro se si blocca
#define MAIN_MOTOR_STD_BACKWARDS_ROTATION 10 //steps
//differenza massima tra encoder e steps contati per avviare il ritorno indietro
#define MAX_DIF_ENCODER_STEPPER 3 //steps
//in gram, peso massimo che pu� contenere il WEIGHING_COLLECTOR oltre il quale bisogna dividere la pesata
#define MAX_WEIGHT_IN_WEIGHING_COLLECTOR 125 //grams
//peso sopra al peso massimo che permette di eseguire una nuova pesata.
//se ci sono pochi grammi sopra MAX_WEIGHT_IN_WEIGHING_COLLECTOR la pesata multipla non pu� essere precisa
#define OVER_WEIGHT_IN_WEIGHING_COLLECTOR 20 //grams

//definisce lo stato aperto o chiuso delle porte, sulla base della logica della scheda NO/NC
#define SWITCH_DOOR_OPEN LOW
#define SWITCH_DOOR_CLOSE HIGH

//SERVO
#define SERVO_MOTOR_MAX_ANGLE 180 //grade
#define SERVO_MOTOR_MIN_ANGLE 0   //grade

#define SERVO_MOTOR_SLOW_OPEN_SPEED 25 //deg*sec
#define SERVO_MOTOR_FAST_OPEN_SPEED 50 //deg*sec
//range in gradi dentro il quale il motore si muove a SERVO_MOTOR_SLOW_OPEN_SPEED
//fuori dal range invece si muove a SERVO_MOTOR_FAST_OPEN_SPEED
#define SERVO_MOTOR_IN_GRAD_SLOW_SPEED 30  //deg
#define SERVO_MOTOR_OUT_GRAD_SLOW_SPEED 70 //deg

#define SERVO_MOTOR_CLOSE_SPEED 80 //deg*sec
//tempo per chiudere in ceck_trapdoor_closed()
#define TIME_TO_INITIAL_CLOSE_TRAPDOOR 200 //milliseconds

//la somma di tutti i timeout deve essere inferiore o uguale a FEED_TIMEOUT
//tempo complessivo per dare da mangiare
#define FEED_TIMEOUT 80000     //milliseconds
#define WEIGHING_TIMEOUT 30000 //milliseconds
//soglia sotto al quale parte l'avvertimento per il quasi esaurimento del cibo
#define STATIC_WARNING_THRESHOLD_LOW_FOOD 5000 //grams

#define DEBUG_SERIAL_PRINT_ON true

#endif
