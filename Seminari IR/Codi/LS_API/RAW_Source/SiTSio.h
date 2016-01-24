//-----------------------------------------------------------------
//TAD         :Canal s�rie, placa base del robot OMNIA
//Autor       :FEC
//Descripci�  :El t�pic i legendari TadSio, transmissi� per recepci�
// amb una cua de recepci� perr�, de moment, les transmissions s'esperen
// fins acabar la transmissi� anterior
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
// Pre: retorna el nombre de car�cters rebuts que no s'han recollit
// amb la funci� GetChar encara

char SiGetChar(void);
// Pre: SiCharAvail() �s major que zero
// Post: Treu i retorna el primer car�cter de la cua de recepci�.

void SiSendChar(char c);
// Post: espera que el car�cter anterior s'hagi enviat i envia aquest

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
