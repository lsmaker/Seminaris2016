//-----------------------------------------------------------------
//TAD         :Canal sèrie, placa base del robot OMNIA
//Autor       :FEC
//Descripció  :El típic i legendari TadSio, transmissió per recepció
// amb una cua de recepció perrò, de moment, les transmissions s'esperen
// fins acabar la transmissió anterior
//Prefix      :Si
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//

#ifdef	__PIC24FJ64GA006__
#include <xc.h>
#endif
//
//--------------------------------CONSTANTS---AREA-----------
//
#define B9600     0
#define B19200    1
#define B38400	2
#define B57600  3
#define BUFFER_SIZE   64
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
//
//---------------------------End--PROTOTIPUS--AREA-----------
//


//
//--------------------------------PUBLIQUES---AREA-----------
//
void SiInit(int Baud);
// Pre: Baud pot valer B9600 o B19200

void SiEnd(void);

int SiCharAvail(void);
// Pre: retorna el nombre de caràcters rebuts que no s'han recollit
// amb la funció GetChar encara

char SiGetChar(void);
// Pre: SiCharAvail() és major que zero
// Post: Treu i retorna el primer caràcter de la cua de recepció.

void SiSendChar(char c);
// Post: espera que el caràcter anterior s'hagi enviat i envia aquest

void SiPuts(char *s);
// Post: Usa SiSendchar

//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//

//
//---------------------------End--PRIVADES----AREA-----------
//
