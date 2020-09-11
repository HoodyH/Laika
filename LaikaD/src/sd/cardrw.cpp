#include <SD.h>
#include "../config.h"
#include "../rtc/datatime.h"
#include "../planner/manage.h"

#include "cardrw.h"

bool Card_rw_Class::setup()
{

	pinMode(SD_CS_PIN, OUTPUT);

	if (!SD.begin(SD_CS_PIN))
	{
		error.system_status(FATAL_ERROR_9000);
		delay(2000);
		setup();
	}
	return true;
}

void Card_rw_Class::save_base_data()
{
	SD.remove("baseconf.j");

	File file = SD.open("baseconf.j", FILE_WRITE);

	if (!file)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to create file baseconf.j"));
		return;
	}

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject(); //crea l'oggetto root

	//DOGINFO
	JsonObject &DOG_INFO = root.createNestedObject("DOG_INFO");
	JsonArray &date_of_birth_dog = DOG_INFO.createNestedArray("date_of_birth_dog");
	date_of_birth_dog.add(manage.date_of_birth_dog[0]);
	date_of_birth_dog.add(manage.date_of_birth_dog[1]);
	date_of_birth_dog.add(manage.date_of_birth_dog[2]);

	DOG_INFO["n_of_meals"] = manage.n_of_meals;

	JsonArray &timetable = DOG_INFO.createNestedArray("timetable");
	JsonObject &meal_1 = timetable.createNestedObject();
	meal_1["hour1"] = manage.timetable[0];
	meal_1["quarter1"] = manage.timetable[1];
	JsonObject &meal_2 = timetable.createNestedObject();
	meal_2["hour2"] = manage.timetable[2];
	meal_2["quarter2"] = manage.timetable[3];
	JsonObject &meal_3 = timetable.createNestedObject();
	meal_3["hour3"] = manage.timetable[4];
	meal_3["quarter3"] = manage.timetable[5];
	JsonObject &meal_4 = timetable.createNestedObject();
	meal_4["hour4"] = manage.timetable[6];
	meal_4["quarter4"] = manage.timetable[7];

	//JUNIOR
	JsonObject &JUNIOR = root.createNestedObject("JUNIOR");
	JsonArray &value_gr_food_15_months = JUNIOR.createNestedArray("value_gr_food_15_months");
	value_gr_food_15_months.add(manage.value_gr_food_15_months[0]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[1]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[2]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[3]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[4]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[5]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[6]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[7]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[8]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[9]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[10]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[11]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[12]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[13]);
	value_gr_food_15_months.add(manage.value_gr_food_15_months[14]);

	//ADULT
	JsonObject &ADULT = root.createNestedObject("ADULT");
	ADULT["months_for_adult"] = manage.months_for_adult;
	ADULT["value_gr_food_adult"] = manage.value_gr_food_adult;

	//se questo valore � vero vengono utilizzate le crocchette 6+/8+/10+/12+ e quindi vengono attivate le variabili sottostanti
	root["food_for_oldness"] = manage.food_for_oldness;

	//MATURE
	JsonObject &MATURE = root.createNestedObject("MATURE");
	MATURE["age_for_mature"] = manage.age_for_mature;
	MATURE["value_gr_food_mature"] = manage.value_gr_food_mature;
	//AGEING
	JsonObject &AGEING = root.createNestedObject("AGEING");
	AGEING["age_for_ageing"] = manage.age_for_ageing;
	AGEING["value_gr_food_ageing"] = manage.value_gr_food_ageing;

	if (root.printTo(file) == 0)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to write to file baseconf.j"));
	}

	file.close();
}

void Card_rw_Class::load_base_data()
{
	File file = SD.open("baseconf.j", FILE_READ);

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(file);

	if (!root.success())
		if (DEBUG_SERIAL_PRINT_ON)
			error.system_status(FATAL_ERROR_9602);

	//DOG_INFO
	JsonArray &DOG_INFO_date_of_birth_dog = root["DOG_INFO"]["date_of_birth_dog"];
	manage.date_of_birth_dog[0] = DOG_INFO_date_of_birth_dog[0];
	manage.date_of_birth_dog[1] = DOG_INFO_date_of_birth_dog[1];
	manage.date_of_birth_dog[2] = DOG_INFO_date_of_birth_dog[2];

	manage.n_of_meals = root["DOG_INFO"]["n_of_meals"];

	JsonArray &timetable = root["DOG_INFO"]["timetable"];
	JsonObject &meal_1 = timetable[0];
	manage.timetable[0] = meal_1["hour1"];
	manage.timetable[1] = meal_1["quarter1"];
	JsonObject &meal_2 = timetable[0];
	manage.timetable[2] = meal_2["hour2"];
	manage.timetable[3] = meal_2["quarter2"];
	JsonObject &meal_3 = timetable[0];
	manage.timetable[4] = meal_3["hour3"];
	manage.timetable[5] = meal_3["quarter3"];
	JsonObject &meal_4 = timetable[0];
	manage.timetable[6] = meal_4["hour4"];
	manage.timetable[7] = meal_4["quarter4"];

	//JUNIOR
	JsonArray &JUNIOR_value_gr_food_15_months = root["JUNIOR"]["value_gr_food_15_months"];
	manage.value_gr_food_15_months[0] = JUNIOR_value_gr_food_15_months[0];
	manage.value_gr_food_15_months[1] = JUNIOR_value_gr_food_15_months[1];
	manage.value_gr_food_15_months[2] = JUNIOR_value_gr_food_15_months[2];
	manage.value_gr_food_15_months[3] = JUNIOR_value_gr_food_15_months[3];
	manage.value_gr_food_15_months[4] = JUNIOR_value_gr_food_15_months[4];
	manage.value_gr_food_15_months[5] = JUNIOR_value_gr_food_15_months[5];
	manage.value_gr_food_15_months[6] = JUNIOR_value_gr_food_15_months[6];
	manage.value_gr_food_15_months[7] = JUNIOR_value_gr_food_15_months[7];
	manage.value_gr_food_15_months[8] = JUNIOR_value_gr_food_15_months[8];
	manage.value_gr_food_15_months[9] = JUNIOR_value_gr_food_15_months[9];
	manage.value_gr_food_15_months[10] = JUNIOR_value_gr_food_15_months[10];
	manage.value_gr_food_15_months[11] = JUNIOR_value_gr_food_15_months[11];
	manage.value_gr_food_15_months[12] = JUNIOR_value_gr_food_15_months[12];
	manage.value_gr_food_15_months[13] = JUNIOR_value_gr_food_15_months[13];
	manage.value_gr_food_15_months[14] = JUNIOR_value_gr_food_15_months[14];

	//ADULT
	manage.months_for_adult = root["ADULT"]["months_for_adult"];
	manage.value_gr_food_adult = root["ADULT"]["value_gr_food_adult"];

	manage.food_for_oldness = root["food_for_oldness"];

	//MATURE
	manage.age_for_mature = root["MATURE"]["age_for_mature"];
	manage.value_gr_food_mature = root["MATURE"]["value_gr_food_mature"];

	//AGEING
	manage.age_for_ageing = root["AGEING"]["age_for_ageing"];
	manage.value_gr_food_ageing = root["AGEING"]["value_gr_food_ageing"];

	file.close();
}

bool Card_rw_Class::save_daily_data()
{

	SD.remove("daily.dat");

	File DailyData = SD.open("daily.dat", FILE_WRITE);

	if (!DailyData)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to create file daily.dat"));
		return false;
	}

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject(); //crea l'oggetto root

	//UPTIME
	JsonObject &UP_TIME = root.createNestedObject("UP_TIME");
	UP_TIME["uptime_days"] = manage.uptime_days;
	UP_TIME["uptime_higher"] = manage.uptime_days;

	root["its_adult"] = manage.its_adult;
	root["its_mature"] = manage.its_mature;
	root["its_ageing"] = manage.its_ageing;

	root["happy_birthday"] = manage.happy_birthday;
	root["daily_ceck_to_do"] = manage.daily_ceck_to_do;
	root["adult_portion_to_calculate"] = manage.adult_portion_to_calculate;

	if (root.printTo(DailyData) == 0)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to write to file daily.dat"));
	}

	DailyData.close();
}

bool Card_rw_Class::load_daily_data()
{
	File file = SD.open("daily.dat", FILE_READ);

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(file);

	if (!root.success())
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to read file daily.dat, using default configuration"));

	manage.uptime_days = root["UPTIME"]["uptime_days"];
	manage.uptime_higher = root["UPTIME"]["uptime_higher"];

	manage.its_adult = root["its_adult"];
	manage.its_mature = root["its_mature"];
	manage.its_ageing = root["its_ageing"];

	manage.happy_birthday = root["happy_birthday"];
	manage.daily_ceck_to_do = root["daily_ceck_to_do"];
	manage.adult_portion_to_calculate = root["adult_portion_to_calculate"];

	file.close();
}

bool Card_rw_Class::save_cycle_data()
{
	SD.remove("cycle.dat");

	File CycleData = SD.open("cycle.dat", FILE_WRITE);

	if (!CycleData)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to create file cycle.dat"));
		return false;
	}

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject(); //crea l'oggetto root

	root["gr_today_food"] = manage.gr_today_food;
	root["gr_today_food_left"] = manage.gr_today_food_left;

	JsonArray &original_gr_meal = root.createNestedArray("original_gr_meal");
	original_gr_meal.add(manage.original_gr_meal[0]);
	original_gr_meal.add(manage.original_gr_meal[1]);
	original_gr_meal.add(manage.original_gr_meal[2]);
	original_gr_meal.add(manage.original_gr_meal[3]);

	JsonArray &adj_gr_meal = root.createNestedArray("adj_gr_meal");
	adj_gr_meal.add(manage.adj_gr_meal[0]);
	adj_gr_meal.add(manage.adj_gr_meal[1]);
	adj_gr_meal.add(manage.adj_gr_meal[2]);
	adj_gr_meal.add(manage.adj_gr_meal[3]);

	JsonArray &done_meal = root.createNestedArray("done_meal");
	done_meal.add(manage.done_meal[0]);
	done_meal.add(manage.done_meal[1]);
	done_meal.add(manage.done_meal[2]);
	done_meal.add(manage.done_meal[3]);

	//variabili di controllo
	root["status_start_cicle"] = manage.status_start_cicle;
	root["status_unload_sequence"] = manage.status_unload_sequence;
	root["status_end_cicle"] = manage.status_end_cicle;

	//variabili deep cicle
	root["currently_weight"] = manage.currently_weight;

	if (root.printTo(CycleData) == 0)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to write to file"));
	}

	CycleData.close();
}

bool Card_rw_Class::load_cycle_data()
{
	File file = SD.open("cycle.dat", FILE_READ);

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(file);

	if (!root.success())
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to read file cycle.dat, using default configuration"));

	manage.gr_today_food = root["gr_today_food"];
	manage.gr_today_food_left = root["gr_today_food_left"];

	JsonArray &original_gr_meal = root["original_gr_meal"];
	manage.original_gr_meal[0] = original_gr_meal[0];
	manage.original_gr_meal[1] = original_gr_meal[1];
	manage.original_gr_meal[2] = original_gr_meal[2];
	manage.original_gr_meal[3] = original_gr_meal[3];

	JsonArray &adj_gr_meal = root["adj_gr_meal"];
	manage.adj_gr_meal[0] = adj_gr_meal[0];
	manage.adj_gr_meal[1] = adj_gr_meal[1];
	manage.adj_gr_meal[2] = adj_gr_meal[2];
	manage.adj_gr_meal[3] = adj_gr_meal[3];

	JsonArray &done_meal = root["done_meal"];
	manage.done_meal[0] = done_meal[0];
	manage.done_meal[1] = done_meal[1];
	manage.done_meal[2] = done_meal[2];
	manage.done_meal[3] = done_meal[3];

	//variabili di controllo
	manage.status_start_cicle = root["status_start_cicle"];
	manage.status_unload_sequence = root["status_unload_sequence"];
	manage.status_end_cicle = root["status_end_cicle"];

	//variabili deep cicle
	manage.currently_weight = root["currently_weight"];

	file.close();
}

void Card_rw_Class::save_record()
{

	File Record = SD.open("record.out", FILE_WRITE);

	if (!Record)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to create file record.out"));
		return false;
	}

	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject(); //crea l'oggetto root

	JsonArray &data = root.createNestedArray("data");
	data.add(manage.year);
	data.add(manage.month);
	data.add(manage.day);

	root["n_of_meals"] = manage.n_of_meals;

	JsonArray &measl = root.createNestedArray("measl");

	JsonObject &first_meal = measl.createNestedObject();
	JsonObject &time = first_meal.createNestedObject("time");
	time["hour"] = manage.real_timetable[0];
	time["minute"] = manage.real_timetable[1];
	first_meal["portion"] = manage.adj_gr_meal[0];

	if (manage.n_of_meals > 1)
	{

		JsonObject &second_meal = measl.createNestedObject();
		JsonObject &time = second_meal.createNestedObject("time");
		time["hour"] = manage.real_timetable[2];
		time["minute"] = manage.real_timetable[3];
		second_meal["portion"] = manage.adj_gr_meal[1];

		if (manage.n_of_meals > 2)
		{
			JsonObject &third_meal = measl.createNestedObject();
			JsonObject &time = third_meal.createNestedObject("time");
			time["hour"] = manage.real_timetable[4];
			time["minute"] = manage.real_timetable[5];
			third_meal["portion"] = manage.adj_gr_meal[2];

			if (manage.n_of_meals > 3)
			{
				JsonObject &fourth_meal = measl.createNestedObject();
				JsonObject &time = fourth_meal.createNestedObject("time");
				time["hour"] = manage.real_timetable[6];
				time["minute"] = manage.real_timetable[7];
				fourth_meal["portion"] = manage.adj_gr_meal[3];
			}
		}
	}
	root["gr_today_food_provided"] = manage.gr_today_food_provided;

	if (root.printTo(Record) == 0)
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to write to file"));

	Record.write("\n");

	Record.close();
}

bool Card_rw_Class::error_log(String error)
{

	File file = SD.open("error.log", FILE_WRITE);

	char buffer[55];
	int n_bit_written;
	char *err = (char *)error.c_str();

	if (!file)
	{
		if (DEBUG_SERIAL_PRINT_ON)
			Serial.println(F("Failed to create file error.log"));
		return false;
	}

	data_time.get_data_time(&manage.year, &manage.month, &manage.day, &manage.hour, &manage.minute, &manage.second);

	n_bit_written = sprintf(buffer, "Data: %d/%d/%d Time: %d:%d:%d --> %s\n", manage.year, manage.month, manage.day, manage.hour, manage.minute, manage.second, err);

	file.write(buffer, n_bit_written);

	file.close();
}

//String(*f)()
bool Card_rw_Class::write(String file_name)
{

	File file = SD.open(file_name, FILE_WRITE);

	//String temp = f();
	//char tab2[1024];
	//strcpy(tab2, temp.c_str());

	file.write("ciao"); //Scrivo su file il numero
	Serial.println(file.readString());

	file.close(); //Chiusura file

	return true;
}

bool Card_rw_Class::read(String file_name)
{

	File file = SD.open(file_name, FILE_READ);

	if (!file)
	{
		Serial.println(F("Failed to opern"));
		return;
	}

	String TextInFile;
	char buff[200];
	char waste;
	long bytecount = 0;
	long size = file.size();

	//TextInFile = (file.readString());
	//TextInFile = file.readStringUntil('\n');

	//byte data1 = TextInFile.substring(3, 6).toInt(); //assegno al byte il contenuto di quella porzione di stringa

	//file.read(&line, 200);
	//file.seekEnd(-50);
	//file.seek(2);
	Serial.println("Inizio lettura");

	//do {
	Serial.println("Inizio lettura riga");

	byte i = -1;

	do
	{
		i++;
		bytecount++;
		file.read(&buff[i], 1);
		Serial.print("i: ");
		Serial.println(i);

	} while (buff[i] != ';');

	do
	{
		bytecount++;
		file.read(&waste, 1);

	} while (waste != ';');

	Serial.println(buff);
	Serial.println(bytecount);
	Serial.println(size);
	Serial.println("letta riga");

	//for (i = 0; i <= 200; i++)
	//buff[i] = ' ';

	//} while (bytecount < size-1);

	char var[50];
	int d;
	int m;
	int y;

	sscanf(buff, "%s %d %d %d", var, &d, &m, &y);
	Serial.print("nome variabile: ");
	Serial.println(var);
	Serial.print("Data: ");
	Serial.print(d);
	Serial.print(" ");
	Serial.print(m);
	Serial.print(" ");
	Serial.println(y);
	Serial.print("\n\n\n");

	//file.seek(0);
	//file.read(line, i);

	//TextInFile = (file.readString());
	//Serial.println(TextInFile);

	//Serial.println(file.read());

	file.close(); //Chiudo file
	return true;
}
