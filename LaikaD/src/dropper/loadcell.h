#ifndef _LOADCELL_h
#define _LOADCELL_h


class Load_Cell_Class
{
protected:
	
	int16_t weigth = 0;
	int	pesoprec = 0; 

public:
	void setup();
	int16_t get_weight();
	void power_down();
	void power_up();

}; extern Load_Cell_Class load_cell;

#endif