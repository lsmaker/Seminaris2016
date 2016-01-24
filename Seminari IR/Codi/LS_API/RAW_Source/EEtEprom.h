//-----------------------------------------------------------------
//TAD         :Gestió de l'eeprom
//Autor       :FEC
//Descripció  :Grava i llegeix dades de l'eeprom i llegeix la MAC
//Prefix      :EE
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//
//----------------------------------------PÚBLIQUES----------------
//
// Posicions interessant a dins de l'eeprom
#define EE_MAC					0xFA		// Posició de la MAC de 6 bytes
#define EE_NOM_PROPIETARI		0x80		// 25 caselles
#define EE_CALIBRA_ESQUERRA		(EE_NOM_PROPIETARI+25)	// 1 byte
#define EE_CALIBRA_DRETA		(EE_CALIBRA_ESQUERRA+1)	// 1 byte
#define EE_CALIBRA_TRACCIO		(EE_CALIBRA_DRETA+1)	// 1 byte
#define EE_PUNTER_A				(EE_CALIBRA_TRACCIO+1)	// 2 bytes
#define EE_PUNTER_B				(EE_PUNTER_A+2)			// 2 bytes
#define EE_PUNTER_C				(EE_PUNTER_B+2)			// 2 bytes
#define EE_MAC_REMOT			(EE_PUNTER_C+2)			// 3 Bytes
#define EE_VERGE				(EE_MAC_REMOT+3)		// 1 byte
#define EE_TEST					(EE_VERGE+1)				// 1 byte

void EEInit(void);

void EERead(unsigned int Pos, unsigned char *buffer, unsigned int Length);
// Pre : Pos + Lenght < 128
// Pre : Pos i Length positius
// Post: Llegeix el nombre de caracters indicats a Lenght a partir
// de l'adreça Pos de l'EEPROM i els posa a l'array buffer

void EEWrite(unsigned int Pos, unsigned char c);
// Pre : Pos < 128
// Pre : Pos i positius
// Post: Escriu c a l'adreça Pos de l'EEPROM. Cal esperar 5ms per escriure 
// una nova dada 

void EEGetLargeMAC(unsigned char MAC[COM_LARGE_MAC_SIZE]);
// Post: Posa en MAC el valor de l'adreça mac en format llarg

void EEGetShortMAC(unsigned char MAC[COM_SHORT_MAC_SIZE]);
// Post: Posa en MAC el valor abreujat de l'adreça mac en format llarg

char *EEGetMACString(void);
// Post: retorna un string (null termianted) amb l'adreça mac reduida,
// en hexa i apte per a imprimir

unsigned char EEVerge(void);
// Post: Retorna cert si l'eeprom encara no ha estat configurada per l'LSLoader

void EEGetPropietari(char *buffer);
// Post: Posa en buffer el nom del propieatari del LSMaker o be "La Salle" si encara
// no hi ha cap nom enregistrat 

int EEVerifica(void);
// Post: retorna cert si es pot escriure a l'EEPROM
