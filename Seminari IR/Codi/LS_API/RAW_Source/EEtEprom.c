//-----------------------------------------------------------------
//TAD         :Gestió de l'eeprom
//Autor       :FEC
//Descripció  :Grava i llegeix dades de l'eeprom i llegeix la MAC
//Prefix      :EE
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//
//----------------------------------------PROTOTIPUS---------------
//
#include "../LSApi.h"
#include <string.h>

void EEError();
//
//------------------------------END-------PROTOTIPUS---------------
//
//
//----------------------------------------CONSTANTS----------------
//
#define TX_SPIReady				while(SPI2STATbits.SPITBF==0);
#define WaitSPIReady			while(SPI2STATbits.SPIRBF==0);
#define Reset_SPIRBF			tonto=SPI2BUF;
#define Send_DUMMYDATA			SPI2BUF=0x00;
#define CS_Actiu_EEPROM			(_LATF0 = 0)
#define CS_Inactiu_EEPROM		(_LATF0 = 1)
#define ORDRE_LECTURA			0x03
#define ORDRE_LECTURA_STATUS	0x05
#define ORDRE_ESCRIPTURA		0x02
#define ORDRE_WRITE_ENABLE		0x06
#define ORDRE_WRITE_DISABLE		0x04
#define WIP						0x01		// Write in progress bit

//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------VARIABLES----------------
//
//
//------------------------------END-------VARIABLES----------------
//
static unsigned char tonto;	// Per provocar una lectura
static unsigned char GetStatus(void);
//
//----------------------------------------PÚBLIQUES----------------
//
void EEInit(void) {
	// EL CS EEprom ja està programat com a sortida en el tad radio
}


void EERead(unsigned int Pos, unsigned char *buffer, unsigned int Length) {
// Pre : Pos + Lenght < 128
// Pre : Pos i Length positius
// Post: Llegeix el nombre de caracters indicats a Lenght a partir
// de l'adreça Pos de l'EEPROM i els posa a l'array buffer
	int i; char c;
	// Comprovo que no hi hagi una operació en marxa
	int Intents = 100; 
	do {
		IoWait(100);
		if ((GetStatus() & WIP) == 0) break;
	 	if (--Intents == 0) {
			EEError(); 
			break; 
		}
	} while (Intents) ;
	CS_Actiu_EEPROM;    			// CS enable
  	Reset_SPIRBF;					// Esborro el flag
	SPI2BUF = ORDRE_LECTURA;		// Ordre de lectura
	WaitSPIReady;             		// Esperem que acabi de transmetre
	Reset_SPIRBF;
	SPI2BUF = Pos;					// L'adreça
	WaitSPIReady;             		// Esperem que acabi de transmetre
	Reset_SPIRBF;
	for (i = 0; i <	 (Length); i++)   {
    	Send_DUMMYDATA;             // Byte tonto
		WaitSPIReady;             	// Esperem que acabi de transmetre
    	buffer[i] = SPI2BUF;        // Guarda la dada rebuda
		c = buffer[i];
	}
	CS_Inactiu_EEPROM;         		// CS disable
}

void EEWrite(unsigned int Pos, unsigned char c) { // 70 microsegons per byte
// Pre : Pos < 128
// Pre : Pos i positius
// Post: Escriu c a l'adreça Pos de l'EEPROM. Cal esperar 5ms per escriure 
// una nova dada 
	// Comprovo que no hi hagi una operació en marxa
	int Intents = 100; 
	do {
		IoWait(100);
		if ((GetStatus() & WIP) == 0) break;
	 	if (--Intents == 0) {
			EEError(); 
			break; 
		}
	} while (Intents) ;
	//------------------------------HABILITO L'ESCRIPTURA
	CS_Actiu_EEPROM;    			// CS enable
  	Reset_SPIRBF;					// Esborro el flag
	SPI2BUF = ORDRE_WRITE_ENABLE;	// Ordre de WRITE ENABLE
	WaitSPIReady;             		// Esperem que acabi de transmetre
  	Reset_SPIRBF;					// Esborro el flag
	CS_Inactiu_EEPROM;         		// CS disable
	IoWait(10);
	//------------------------------ESCRIC------------------
	CS_Actiu_EEPROM;    			// CS enable
	Reset_SPIRBF;					// Esborro el flag
	SPI2BUF = ORDRE_ESCRIPTURA;		// Ordre de WRITE
	WaitSPIReady;             		// Esperem que acabi de transmetre
	Reset_SPIRBF;					// Esborro el flag
	SPI2BUF = Pos;					// adreça
	WaitSPIReady;             	// Esperem que acabi de transmetre
  	Reset_SPIRBF;				// Esborro el flag
    SPI2BUF = c;				// La dada
    WaitSPIReady;               // Esperem que acabi de transmetre
  	Reset_SPIRBF;				// Esborro el flag
	CS_Inactiu_EEPROM;         	// CS disable
}

void EEGetLargeMAC(unsigned char MAC[COM_LARGE_MAC_SIZE]) {
// Post: Posa en MAC el valor de l'adreça mac en format llarg
	EERead(EE_MAC, MAC, 6); 
}

void EEGetShortMAC(unsigned char MAC[COM_SHORT_MAC_SIZE]) {
// Post: Posa en MAC el valor abreujat de l'adreça mac en format llarg
	EERead(EE_MAC+3, MAC, 6-3); 
}

static char tempo[7];
static char ToHexa[16] ={ '0','1','2','3','4','5','6','7','8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static void OmpleHexa(char *s, unsigned char v) {
	s[0] = ToHexa[v >> 4];
	s[1] = ToHexa[v & 0x0f];
	s[2] = 0;
}

char *EEGetMACString(void) {
	unsigned char MAC[COM_SHORT_MAC_SIZE]; int i;
	EEGetShortMAC(MAC);
	for (i = 0; i < 3; i++) {
		OmpleHexa(&tempo[i*2], MAC[i]);
	}
	return tempo;
}

static unsigned char GetStatus(void) {
	unsigned char s;
	CS_Actiu_EEPROM;    			// CS enable
  	Reset_SPIRBF;					// Esborro el flag
	SPI2BUF = ORDRE_LECTURA_STATUS;	// Ordre de lectura
	WaitSPIReady;             		// Esperem que acabi de transmetre
	Reset_SPIRBF;
   	Send_DUMMYDATA;             	// Byte tonto
	WaitSPIReady;             		// Esperem que acabi de transmetre
	s = SPI2BUF;					// Capturo el byte
	CS_Inactiu_EEPROM;         		// CS disable
	return s;
}


void EEError(void) {
	LS_USB_printf("Error greu en EEPROM\n\r");
}

unsigned char EEVerge(void) {
// Post: Retorna cert si l'eeprom encara no ha estat configurada per l'LSLoader
	unsigned char c;
	EERead(EE_VERGE, &c, 1); 
	return c != 0x69; // Un numero qualsevol
}

void EEGetPropietari(char *buffer) {
// Post: Posa en buffer el nom del propieatari del LSMaker o be "La Salle" si encara
// no hi ha cap nom enregistrat 
	if (EEVerge()) strcpy(buffer, "    La Salle");
	else EERead(EE_NOM_PROPIETARI, (unsigned char *) buffer, 25);
}

int EEVerifica(void) { unsigned char c;
// Post: retorna cert si es pot escriure a l'EEPROM
	EEWrite(EE_TEST, 69);
	EERead(EE_TEST, &c, 1);
	return c == 69;
}
	
