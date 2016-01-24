
#include "LSApi.h"

// Prototipus generics que no es poden encabir en cap altre fitxer iq eu no mereixen
// estar a la API
void SysInit(void);
void SysMotor(void);

//#define CANAL		200	// Cotxe 2

#define CANAL		0	// Cotxe oruga
unsigned char LOCAL[6] = { 4,0,0,0,0,CANAL};

#define PROPAGANDA1		" LS Maker. v"
#define ERROR_EEPROM 	"ATENCIO: ERROR EN EEPROM\n\r"

// Vectors per al Bootloader. 
	unsigned int userReset  __attribute__ ((space(prog),section(".BLreset"))) = 0xC00 ;  
	unsigned char timeout  __attribute__ ((space(prog),section(".BLreset"))) = 0xA ;

void LS_Executiu(void) {
	// Quan no hi ha activitat, aquest bucle dura entre 100 i 200us
	// La interrupció de l'ADC, cada 2.5ms, pot afegir 100us
	IoMotor();	// Entrades i sortides
	PtMotor();	// El protocol de comunicació	ULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
	RfMotor();	// L'enllaç de radio
	AcMotor();	// L'acceleròmetre
	LcMotor();	// L'LCD
	SysMotor();	// El control de tasques
	MtMotor();  // El control dels motors
}

int LS_OpenCom(int Com) {
// Pre : Com entre 1 i 15. Només te sentit en mode remot, no fa res en mode local
// Post: Obre aquest canal de comunicació per parlar amb el control remot
// Retorna 0 si ha pogut obrir el com, això no vol dir que en aquest COM
// hi hagi un control remot
	return 0;
}

int LS_CloseCom(int Com) {
// Pre : Com entre 1 i 15. Només te sentit en mode remot, no fa res en mode local
// Post: Tanca aquest canal de comunicació 
// Retorna 0 si ha pogut tancar el com
	return 0;
}

void LS_Init(void) { int i; char NOM[100];
	// Inicialitzacions genèriques
	// Clock intern a 8Mhz
	CLKDIVbits.RCDIV2 = 0;
	CLKDIVbits.RCDIV1 = 0;
	CLKDIVbits.RCDIV0 = 0;
	// Aturo els motors
	TRISEbits.TRISE5 = 0; TRISEbits.TRISE6 = 0;
	_LATE6 = 0; _LATE5 = 0;
	// Interrupcions
	INTCON1bits.NSTDIS = 1; // NO es permeten aniuaments
	// NO modifico els IPCs, per tant, totes les fonts interruptives a nivell 4.
	// TADS de baix nivell
	TiInit();			// Timer
	// SiInit(B19200);		// El Canal Serie
	SiInit(B57600);		// El Canal Serie
	IoInit();			// Els ports digitals i analògics
	i2Init();			// El bus I2C
	RfInit();			// El transceptor de radio
	PtInit();			// El protocol de comunicacio
	EEInit();			// L'eeprom
	MtInit();			// EL Control dels motors
	NvInit();			// Memoria flash
	AcInit();			// Acceleròmetre
	LcInit(4, 16);		// El display LCD de 4 x 16
	SysInit();			// El control de tasques
	EEGetPropietari(NOM);
	// Pinto a l'LCD
	LS_LCD_Clear();
	LS_LCD_Printf(0,0, " LS Maker v%s", VERSIO_LIB);
	LS_LCD_Printf(0,1, NOM);
	LS_LCD_Printf(10,3, EEGetMACString());
	for (i = 0; i < 100; i++) LcMotor();
	// Canto pel canal serie, propaganda, nom de l'usuari i errors si s'escau.
	SiPuts(PROPAGANDA1); SiPuts(VERSIO_LIB); SiPuts("\n\r");
	SiPuts(NOM); SiPuts("\n\r");
	SiPuts("  MAC="); SiPuts(EEGetMACString()); SiPuts("\n\r");
	if (!EEVerifica()) SiPuts(ERROR_EEPROM);
	LS_IO_SetLedBeat(5);
}




