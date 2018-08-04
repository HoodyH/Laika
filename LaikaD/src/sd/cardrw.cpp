#include "cardrw.h"
#include "../../base.h"

File file;


bool Card_rw_Class::setup() {

	if (!SD.begin(53)) //il Pin 53 è collegato a CS
	{
		return false;
	}
	return true;
}

bool Card_rw_Class::write(String (*f)(), String file_name) {
	
	
	if ((file = SD.open(file_name, FILE_WRITE)) == NULL) {
		return false;
	}
		file.println( f() ); //Scrivo su file il numero
		file.close(); //Chiusura file

		return true;
}

bool Card_rw_Class::read(String file_name, String out) {

	if ((file = SD.open(file_name, FILE_READ)) == NULL) {
		return false;
	}
		char line[200];
		String TextInFile;

		//TextInFile = (file.readString());
		TextInFile = file.readStringUntil('\n');
		
		//byte data1 = TextInFile.substring(3, 6).toInt(); //assegno al byte il contenuto di quella porzione di stringa

		
		//file.read(&line, 200);
		//file.seekEnd(-50);
		file.read(line, 20); 

		//Serial.println(line);
		Serial.println(TextInFile);

		
		//TextInFile = file.read(buf, GCI_BUF_SIZE);

		file.close(); //Chiudo file
		return true;

}
