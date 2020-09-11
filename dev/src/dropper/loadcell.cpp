#include "../../base.h"

HX711 load_cell_dx(LOAD_CELL_DT_DX_PIN, LOAD_CELL_SCK_DX_PIN);
HX711 load_cell_sx(LOAD_CELL_DT_SX_PIN, LOAD_CELL_SCK_SX_PIN);

void Load_Cell_Class::setup()
{
	
	load_cell_dx.set_scale(LOAD_CELL_ADJ_FULL_SCALE); //fondo scala cella di carico
	load_cell_dx.tare(10);

	//load_cell_sx.set_scale(LOAD_CELL_ADJ_FULL_SCALE); /fondo scala cella di carico
	//load_cell_sx.tare(10);
	//load_cell_sx.set_offset(LOAD_CELL_ADJ_MESURE);

	Serial.println("load cell ready");
}
//
int16_t Load_Cell_Class::get_weight()
{
	weigth = load_cell_dx.get_units();

	return weigth;
}

void Load_Cell_Class::power_up() {
	load_cell_dx.power_up();
	load_cell_sx.power_up();
}
void Load_Cell_Class::power_down() {
	load_cell_dx.power_down();
	load_cell_sx.power_down();
}
