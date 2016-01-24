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
#include "SiTSio.h"
#include <xc.h>

//
//--------------------------------CONSTANTS---AREA-----------
//
#define B9600     0
#define B19200    1
#define BUFFER_SIZE   64
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
static char Buffer[BUFFER_SIZE];
static int Inici, Fi, Quants;
//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
static void Posa(char);
static char Treu(void);

//
//---------------------------End--PROTOTIPUS--AREA-----------
//


//
//--------------------------------PUBLIQUES---AREA-----------
//
void SiInit(int Baud) {
// Pre: Baud pot valer B9600 o B19200
	Inici = Fi = Quants = 0;
	// Programo a N,8,1 i activo la UART1
	U1MODE = 0x8000;
	// Fixo el baud rate amb BRGH = 0, amb Fosc = 8Mhz (per a 9600 i 19200
	switch ( Baud) {
		case B9600 : U1BRG = 25; break; 
		case B19200: U1BRG = 12; break;
		case B57600:
			// Cal posar el BRGH a 1 o si no l'error és massa gran
			U1MODE = 0x8008;
			U1BRG  = 16;	// Error del 2.1%, no se, no se...
			break; 
	}
	// U1STA ja está bé tal com vé per defecte	
	// Activo les interrupcions
	IEC0bits.U1RXIE = 1;
}

void SiEnd(void);

int SiCharAvail(void) {
// Pre: retorna el nombre de caràcters rebuts que no s'han recollit
// amb la funció GetChar encara
	return Quants;
}

char SiGetChar(void) {
// Pre: SiCharAvail() és major que zero
// Post: Treu i retorna el primer caràcter de la cua de recepció.
	return Treu();
}

void SiSendChar(char c) {
// Post: espera que el caràcter anterior s'hagi enviat i envia aquest
	while(U1STAbits.TRMT == 0) ClrWdt();
	U1TXREG = c;
	U1STAbits.UTXEN = 1;
}

void SiPuts(char *s) {
	while(*s) SiSendChar(*s++);
}	

//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//
static void Posa(char c) {
	Buffer[Inici++] = c;
	if (Inici == BUFFER_SIZE) Inici = 0;
	Quants++;
}
static char Treu(void) {
	char tmp;
	tmp = Buffer[Fi++];
	if (Fi == BUFFER_SIZE) Fi = 0;
	Quants --;
	return tmp;
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
	IFS0bits.U1RXIF = 0;
	if (U1STAbits.OERR == 1) U1STAbits.OERR = 0;
	Posa(U1RXREG);
}	
//
//---------------------------End--PRIVADES----AREA-----------
//

