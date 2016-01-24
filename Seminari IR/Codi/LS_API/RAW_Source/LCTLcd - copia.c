//
// TAD per a manipular un display alfanumèric basat en el 
// controlador HD44780. Aquest és el controlador que porten la
// immensa majoria de displays. Tamany màxim, 4 files per 40 columnes
//
// Durant el procés d'inicialització, es demanarà un timer al tad timer
// que no s'usarà per a res més. El constructor alliberarà aquest timer.

// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar
// després de fer un Clear, amb independència del que digui el Busy. 
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, rertardo amb timer.

// Vcli V2.0, Sisco, 3 de març de 2010. L'adapto per a funcionar sobre un PIC24F16KA102
// Vcli V2.1, Sisco, JULIOL de 2010. L'adapto per a funcionar sobre un PIc24fj64ga006



#include "..\LSApi.h"

//
//--------------------------------CONSTANTS---AREA-----------
//
#define FUNCTION_SET	0x20
#define BITS_8			0x10
#define DISPLAY_CONTROL	0x08
#define DISPLAY_ON		0x04
#define CURSOR_ON		0x02
#define DISPLAY_CLEAR	0x01
#define ENTRY_MODE		0x04
#define SET_DDRAM		0x80
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
static int Files, Columnes; 
static int FilaAct, ColumnaAct; 
static int Timer; 
//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
void Espera(int Timer, int ms); 
void CantaIR(char IR);
void CantaData(char Data);
void WaitForBusy(void);
void EscriuPrimeraOrdre(char);
void InicialitzaMCP2317(void);
//
//---------------------------End--PROTOTIPUS--AREA-----------
//


//
//--------------------------------PUBLIQUES---AREA-----------
//
void LcInit(int files, int columnes) {
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 } 
// Pre: L'Hitachi merdós necessita 40ms de tranquilitat desde  
// la pujada de Vcc fins cridar aquest constructor
// Pre: Hi ha un timer lliure
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posició 0, 0.  
	int i; 
	Timer = TiGetTimer(); // Val més que n'hi hagi
	Files = files; Columnes = columnes;
	FilaAct = ColumnaAct = 0;
	InicialitzaMCP2317();
	for (i = 0; i < 2; i++) { 
		Espera(Timer, 40);
		// El manual de l'hitachi diu que s'ha de fer aquesta 
		// seqüència...
		EscriuPrimeraOrdre(FUNCTION_SET | BITS_8); 
		Espera(Timer, 4); 
		EscriuPrimeraOrdre(FUNCTION_SET | BITS_8);	
		Espera(Timer, 4); 
		EscriuPrimeraOrdre(FUNCTION_SET | BITS_8);	
		// .. tres vegades. Potser és una penitència?
		// Ara una a 4 bits
		EscriuPrimeraOrdre(FUNCTION_SET); 	
		Espera(Timer, 1); 
		if (files == 1) 
			CantaIR(FUNCTION_SET); 			// 4bits, 1 files, font 5x7	
		else 
			CantaIR(FUNCTION_SET | 0x08); 	// 4bits, 1 files, font 5x7	
		// Ara ja podem mirar el busy
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL);  	// Display Off
		WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   	// Tot espais
		Espera(Timer,2); // 1.64ms V1.1
		WaitForBusy(); 	CantaIR(ENTRY_MODE | 0x02); // Auto Increment i shift
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL | DISPLAY_ON); 		// Display On
	}
	// El manual dirà el que vulgui però jo he vist que després
	// d'una arrencada freda, no sempre s'inicialitza correctament 
	// tot i esperar els 40ms. Per això, he posat un bucle de dues
	// inicialitzacions i a partir d'aquí, la inicialització m'ha funcionat 
	// correctament tant si faig un reset com si apago i engego l'alimentació.
}

void LcEnd(void) {
// El Destructor
	TiCloseTimer (Timer); // Ja no ens farà cap falta
}

void LcClear(void) {
// Post: Esborra el display i posa el cursor a la posició zero en 
// l'estat en el que estava. 
// Post: La propera ordre pot trigar fins a 1.6ms
	WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   // Tot espais
	Espera(Timer, 2); // V1.1
}

void LcCursorOn(void) {
// Post: Encén el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy(); 	
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON); 
}

void LcCursorOff(void) {
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy(); 	
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON); 
}

void LcGotoXY(int Columna, int Fila) {
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us
	int Fisica;
	// d'entrada calculo la direcció efectiva de la ram del LCD
	switch (Files) {
		case 2:
			Fisica = Columna + (!Fila ? 0 : 0x40); break;
		case 4:
			Fisica = Columna;
			if (Fila == 1) Fisica += 0x40; else
			if (Fila == 2) Fisica += Columnes;      /* 0x14; */ else 
			if (Fila == 3) Fisica += 0x40+Columnes; /* 0x54; */
			break; 
		case 1:
		default:
			Fisica = Columna; break; 
	}
	// i l'aplico amb la comanda
	WaitForBusy(); 
	CantaIR(SET_DDRAM | Fisica);
	// Finalment, actualitzo les imatges locals
	FilaAct    = Fila;
	ColumnaAct = Columna;
}

void LcPutChar(char c) { 
// Post: Pinta C en l'actual posició del cursor i incrementa 
// la seva posició. Si la columna arriba a 39, salta a 0 tot 
// incrementant la fila si el LCD és de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// Així mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila 
	// D'entrada escric el char
	WaitForBusy(); CantaData(c);
	// i ara recalculo la posició del cursor
	++ColumnaAct;
	if (Files == 3) {
		if (ColumnaAct >= 20) {
			ColumnaAct = 0; 
			if (++FilaAct >= 4) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else 
	if (Files == 2) {
		if (ColumnaAct >= 40) {
			ColumnaAct = 0; 
			if (++FilaAct >= 2) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		} 
	} else 
	if (FilaAct == 1) {
		if (ColumnaAct >= 40) ColumnaAct = 0; 
		LcGotoXY(ColumnaAct, FilaAct);
	}
}

void LcPutString(char *s) {
// Post: Pinta l'string a apartir de la posició actual del cursor.
// El criteri de coordenades és el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de 
// la rutina
	while(*s) LcPutChar(*s++); 
}

//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//


// Registres del MCP2317
#define I2CADDR_23	0x40>>1
#define IODIRA_23	0	
#define IODIRB_23	1
#define GPIOA_23	0x12
#define GPIOB_23	0x13
#define OLAT_A		0x14
#define OLAT_B		0x15

// Variables de mirall
static unsigned char PORTA_23 = 0;
static unsigned char PORTB_23 = 0;
#define RSBit		1
#define RWBit		2
#define EBit 		4

	
static void Escriu2317(unsigned char registre, unsigned char valor) {
	i2WriteData(I2CADDR_23, registre, 1, &valor);
}

static unsigned char Llegeix2317(unsigned char registre) {
	unsigned char dades[1];
	i2ReadData(I2CADDR_23, registre, 1, dades);
	return dades[1];
}

static void CANTA_A(void) { 
	Escriu2317(GPIOA_23, PORTA_23); 
}

static void CANTA_B(void) { Escriu2317(OLAT_B, PORTB_23); }

static void SetD4_D7Sortida(void){
	Escriu2317(IODIRA_23, 0x00);
}		

static void SetD4_D7Entrada(void) {
	Escriu2317(IODIRA_23, 0x07);
}		

static void SetControlsSortida(void){
	Escriu2317(IODIRB_23, 0x00);
}    

static void SetD4D7(unsigned char c) { 
	if (c & 1) PORTA_23 |= 0x8; else PORTA_23 &= ~(0x8);	// D0
	if (c & 2) PORTA_23 |= 0x4; else PORTA_23 &= ~(0x4);	// D1
	if (c & 4) PORTA_23 |= 0x2; else PORTA_23 &= ~(0x2);	// D2
	if (c & 8) PORTA_23 |= 0x1; else PORTA_23 &= ~(0x1);	// D3
	CANTA_A();
}

unsigned char GetBusyFlag(void){
	unsigned char v;
	v = Llegeix2317(GPIOA_23);
	if (v & 1) return 1; else return 0;
}

static void RSUp(void)   	 { PORTB_23 |=  RSBit; CANTA_B(); }	
static void RSDown(void)	 { PORTB_23 &= ~RSBit; CANTA_B(); }				
static void RWUp(void)		 { PORTB_23 |=  RWBit; CANTA_B(); }		
static void RWDown(void)	 { PORTB_23 &= ~RWBit; CANTA_B(); }				
static void EnableUp(void)	 { PORTB_23 |=  EBit; CANTA_B(); }		
static void EnableDown(void) { PORTB_23 &= ~EBit;  CANTA_B(); }				

void InicialitzaMCP2317(void) {
	// No sembla que calgui fer res més
	SetControlsSortida();
	CANTA_A(); CANTA_B();
}

void Espera(int Timer, int ms) {
	TiResetTics(Timer); 
	while(TiGetTics(Timer) < ms) ClrWdt(); 
} 

void CantaPartAlta(char c) {
	 SetD4D7((c>>4) & 0x0f);
//	 SetD7(c & 0x80 ? 1 : 0);
//	 SetD6(c & 0x40 ? 1 : 0);
//	 SetD5(c & 0x20 ? 1 : 0);
//	 SetD4(c & 0x10 ? 1 : 0);
}

void CantaPartBaixa(char c) {
	 SetD4D7(c & 0x0f);
//	 SetD7(c & 0x08 ? 1 : 0);
//	 SetD6(c & 0x04 ? 1 : 0);
//	 SetD5(c & 0x02 ? 1 : 0);
//	 SetD4(c & 0x01 ? 1 : 0);
}

void CantaIR(char IR) {
	SetD4_D7Sortida(); 
	RSDown();
	RWDown();
	EnableUp();
	CantaPartAlta(IR); 		// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();				
	CantaPartBaixa(IR); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada(); 
}

void CantaData(char Data) {
	SetD4_D7Sortida(); 
	RSUp();
	RWDown();
	EnableUp();
	CantaPartAlta(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();				
	CantaPartBaixa(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada(); 
}

void WaitForBusy(void) { char Busy; 
	SetD4_D7Entrada(); 
	RSDown();
	RWUp();
	TiResetTics(Timer);
	do {
		EnableUp();EnableUp(); // Asseguro els 500ns de durada de pols
		Busy = GetBusyFlag();
		EnableDown(); 
		EnableDown(); 
		EnableUp();EnableUp(); 
		// Aquí ve la part baixa del comptador d'adreces, que no ens interessa
		EnableDown(); 
		EnableDown(); 
		ClrWdt()
		if (TiGetTics(Timer)) break; // Més d'un ms vol dir quel LCD està boig
	} while(Busy);
}

void EscriuPrimeraOrdre(char ordre) {
	// Escric el primer com si fossin 8 bits 
	SetD4_D7Sortida();  RSDown(); RWDown();
	EnableUp(); EnableUp();	
	SetD4D7( (ordre >> 4) & 0x0f);
//	SetD7(ordre & 0x80);
//	SetD6(ordre & 0x40);
//	SetD5(ordre & 0x20);
//	SetD4(ordre & 0x10);
	EnableDown(); 
}

//
//---------------------------End--PRIVADES----AREA-----------
//
