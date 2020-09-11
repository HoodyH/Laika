// errors.h

#ifndef _ERRORS_h
#define _ERRORS_h

#include <stdint.h>

//definizione errori

//gli errrori sono composti da 4 cifre
//se iniziano con 9 rappresentano un FATAL_ERROR (assolutamente necessario l'intervello) RIAVVIO RICHIESTO
//se iniziano per 8 rappresentano un CRITICAL_ERROR (rappresenta un problema meccanico potenzialmente grave) RIAVVIO RICHIESTO
//se iniziano per 0 sono normali errori ed il software � in grado sempre di risolverli autonomamente
//se iniziano per 1 sono errori che potrebbero richiedere l'intervento
//se iniziano per 2 sono manutanzioni di routine (intervenire entro qualche giorno)

//ERRORE_DI_INIZIALIZZAZONE				(x0xx)
//ERRORE_DI_SERBATOIO					(x1xx)
//ERRORE_DI_DOSATORE					(x2xx)
//ERRORE_DI_SISTEMA_DI_SCARICO			(x3xx)
//ERRORE_DI_ELETTRONICA					(x4xx)
//ERRORE_DI_GESTIONE_CICLI				(x5xx)
//ERRORE_DI_SALVATAGGIO_SU_SD			(x6xx)

//-------------------------------------------------------------------------------------------------
//ERRORE_DI_INIZIALIZZAZONE				(x0xx)

//9000 inizializzazione SD non riuscita, SD mancante
#define FATAL_ERROR_9000 9000

//-------------------------------------------------------------------------------------------------
//ERRORE_DI_SERBATOIO					(x1xx)

//2100 il serbatoio � in esaurimento (ricarica le crocchette)
#define MAINTENANCE_2100 2100
//9101 il serbatoio delle crocchette � ostruito e non � possibile nutrire il cane
#define FATAL_ERROR_9101 9101
//9102 il serbatoio non contiene abbastanza cibo per erogare interamente questo pasto
#define FATAL_ERROR_9102 9102
//9103 il serbatorio � completamente vuoto
#define FATAL_ERROR_9103 9103

//-------------------------------------------------------------------------------------------------
//ERRORE_DI_DOSATORE					(x2xx)

//9200 FATAL_ERROR ruota tramoggia bloccata
#define FATAL_ERROR_9200 9200
//0201 durante il pesaggio si aprono le portelle, la molla � troppo debole
#define ERROR_0201 0201
//8202 durante il pesaggio il cibo esce dalle portelle a causa della molla troppo debole.
#define CRITICAL_ERROR_8202 8202
//0203 il motore ha trovato difficolt� nel'erogare e si � bloccato, inversione di marcia. Questo � un errore molto frequente
#define ERROR_0203 0203
//1204 l'encoder � immobile, il motore probabilemte si � bloccato
#define ERROR_1204 1204
//9205 l'encoder � immobile, il motore si � bloccato ed � stato bloccato il ciclo di erogazione
#define FATAL_ERROR_9205 9205
//-------------------------------------------------------------------------------------------------
//ERRORE_DI_SISTEMA_DI_SCARICO			(x3xx)

//8301 servomotore non risponde in aperuta
#define CRITICAL_ERROR_8301 8301
//0302 richiusura trapdoor non riuscita
#define ERROR_0302 0302
//0303 chiusura iniziale trapdoor non riuscita
#define ERROR_0303 0303

//-------------------------------------------------------------------------------------------------
//ERRORE_DI_ELETTRONICA					(x4xx)

//8400 l'RTC non funziona, probabilmente la batteria � scarica
#define CRITICAL_ERROR_8400 8400

//-------------------------------------------------------------------------------------------------
//ERRORE_DI_GESTIONE_CICLI				(x5xx)

//1500 timeout ciclo feed(), l'operazione ha impiegato troppo tempo
#define ERROR_1500 1500
//1501 timeout ciclo drop_and_weigh(), l'operazione ha impiegato troppo tempo
#define ERROR_1501 1501
//0502 ????
#define ERROR_0502 0502

//-------------------------------------------------------------------------------------------------
//ERRORE_DI_SALVATAGGIO SU SD			(x6xx)

//8601 salvataggio su file non riuscito, SD mancante
#define CRITICAL_ERROR_8601 8601
//9602 il file di configurazione di base non � stato trovato
#define FATAL_ERROR_9602 9602

class Error_Class
{
private:
public:
	bool system_status(uint16_t status_code);
};
extern Error_Class error;

#endif
