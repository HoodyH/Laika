/* Sketch che permette di aprire un file numeri.txt su SD Card
 * scrivere un numero casuale tra 0 e 999, chiudere il file
 * e rileggerne il contenuto
 */
#include <SPI.h>
#include <SD.h>
File file;
void setup()
{
  Serial.begin(9600);
  Serial.print("Inizializzazione Card: ");
  if (!SD.begin(53)) //il Pin 4 è collegato a CS
  {
    Serial.println("FALLITA!");
    return;
  }
  Serial.println("ESEGUITO!");
  
}
void loop()
{
  int c = random(0,1000); //Estraggo numero casuale tra 0 e 999
  
  file = SD.open("numeri.txt", FILE_WRITE); //File in scrittura
  if (file) //Se il file è stato aperto correttamente
  { 
    Serial.print("Numero casuale scelto: ");
    Serial.println(c);
    Serial.println("Scrittura su file: ");
    file.println(c); //Scrivo su file il numero
    
    file.close(); //Chiusura file
    Serial.println("ESEGUITO!");
  } 
  else 
  {
    Serial.println("ERRORE: apertura file numeri.txt");
  }
  file = SD.open("numeri.txt"); //Riapro il file
  if (file) //Se il file è stato aperto correttamente
  {
    Serial.println("Contenuto file numeri.txt: ");
    //Continua fino a che c'è qualcosa
    while (file.available()) 
    {
      Serial.write(file.read());
    }
    
    file.close(); //Chiudo file
  } 
  else 
  {
    Serial.println("ERRORE: apertura file numeri.txt");
  }
  
  delay(5000); //Attendi 5 secondi prima di ricominciare
}
