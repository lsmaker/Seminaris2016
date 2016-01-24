//-----------------------------------------------------------------
// TAD:Gestió dels senyals d'entrada/sortida
// Descripció:
//   Aquestes rutines permeten accedir als quatre ports d'entrada analògics
// anomentas AN_0, AN_1, AN_2 i AN_3 en els esquemes, que trobareu en els 
// connectors d'expansió.
//   També permet definir la direcció (entrada o sortida) dels ports 
// GPIO_0, GPIO_1, GPIO_2 i GPIO_3, que teniu a disposició en els connectors
// d'expansió
//   A més, amb aquestes rutines podreu:
//   a) Saber si s'ha premut o s'ha deixat anar el polsador de propòsit general
//      etiquetat com GpButton a la placa de circuit imprès
//   b) Governar els leds de la placa
//Prefix      :IO_
//Data        :Wednesday, June 30, 2010
//-----------------------------------------------------------------
#include "../LSApi.h"
//
//----------------------------------------CONSTANTS----------------
//
/* Definides en LSAPi.h
#define GPIO_0	0
#define GPIO_1	1
#define GPIO_2	2
#define GPIO_3	3
#define AN_0	0
#define AN_1	1
#define AN_2	2
#define AN_3	3
#define DIR_OUT		0
#define DIR_IN		1
*/
#define MAX_DUTY	90
#define MIN_DUTY    10	// Tots dos han de sumar 100
#define MAX_BEAT	50

#define GPBUT		(_RB7)
#define GPBUT2		(_RC13)

#define DEBOUNCE	10		// Milisegons d'espera anti rebots.
//
//------------------------------END-------CONSTANTS----------------
//
//
//----------------------------------------VARIABLES----------------
//
static int Duty = 0;
static int NouDuty;
static int Estat = 0;
static int Beat, Periode, P_2;
static int T; 
// Les cues per a fer un promig mòbil dels quatre senyals analògics
#define MAX_CUES_AN 	8	// Un periode complert 
static struct Cua {
	unsigned char inici, fi; 
	int Valors[MAX_CUES_AN];
	long Promig;
} Cua0, Cua1, Cua2, Cua3;
static int AN0, AN1, AN2, AN3, BAT;
static unsigned char BotoPres, BotoUnPres; 
static unsigned char BotoPres2, BotoUnPres2; 
//
//------------------------------END-------VARIABLES----------------
//
//
//----------------------------------------PRIVADES-----------------
//
static int AnTreu(struct Cua *c) {
	int t; 
	t = c -> Valors[c -> fi++]; 
	if (c->fi == MAX_CUES_AN) c->fi = 0;
	return t;
}

static void AnPosa (struct Cua *c, int v) {
	c -> Valors[c->inici++] = v; 
	if (c->inici == MAX_CUES_AN) c->inici  = 0;
}

static void AnAmitjana(struct Cua *c, int v) {
	c -> Promig = c -> Promig + v - AnTreu(c);
	AnPosa(c, v);
}

static void AnInicialitza(struct Cua *c) { int i;
	c -> inici = c -> fi = 0;
	for (i = 0; i < MAX_CUES_AN; i++) AnPosa(c, 0);
}

static void ActivaLeds(void) {
	OC5RS = 100;
}

static void DesactivaLeds(void) {
	OC5RS = 0;
}

static void MotorPress(void) {
	static int Estat = 0;
	static int T;
	switch (Estat) {
		case 0: T = LS_SYS_OpenTimer(); Estat++; break;
		case 1: 
			if (GPBUT == 0) {	
				// Sembla ser que s'ha premut el botó. 
				LS_SYS_ResetTics(T); Estat++; 
			} 
			break;
		case 2:
			if (LS_SYS_GetTics(T) >= DEBOUNCE) {	
				// Han passat 10 ms, si encara està a zero, vol dir que 
				// no és un espuri i que ja estem fora de la zona de rebots
				if (GPBUT == 0) {
					++Estat; BotoPres = 1;
				} else --Estat; // NO, falsa alarma
			}
			break;
		case 3:
			// Esperarem que acabi d'apretar
			if (GPBUT == 1) {	
				// Sembla ser que s'ha despremut el botó. 
				LS_SYS_ResetTics(T); Estat++; 
			} 
			break;
		case 4:
			if (LS_SYS_GetTics(T) >= DEBOUNCE) {	
				// Han passat 10 ms, si encara està a 1, vol dir que 
				// no és un espuri i que ja estem fora de la zona de rebots
				if (GPBUT == 1) {
					Estat = 1; BotoUnPres = 1;
				} else --Estat; // NO, falsa alarma
			}
			break;
		
	}
}

static void MotorPress2(void) {
	static int Estat = 0;
	static int T;
	switch (Estat) {
		case 0: T = LS_SYS_OpenTimer(); Estat++; break;
		case 1: 
			if (GPBUT2 == 0) {	
				// Sembla ser que s'ha premut el botó. 
				LS_SYS_ResetTics(T); Estat++; 
			} 
			break;
		case 2:
			if (LS_SYS_GetTics(T) >= DEBOUNCE) {	
				// Han passat 10 ms, si encara està a zero, vol dir que 
				// no és un espuri i que ja estem fora de la zona de rebots
				if (GPBUT2 == 0) {
					++Estat; BotoPres2 = 1;
				} else --Estat; // NO, falsa alarma
			}
			break;
		case 3:
			// Esperarem que acabi d'apretar
			if (GPBUT2 == 1) {	
				// Sembla ser que s'ha despremut el botó. 
				LS_SYS_ResetTics(T); Estat++; 
			} 
			break;
		case 4:
			if (LS_SYS_GetTics(T) >= DEBOUNCE) {	
				// Han passat 10 ms, si encara està a 1, vol dir que 
				// no és un espuri i que ja estem fora de la zona de rebots
				if (GPBUT2 == 1) {
					Estat = 1; BotoUnPres2 = 1;
				} else --Estat; // NO, falsa alarma
			}
			break;
	}
}
//
//--------------------------------END-----PRIVADES-----------------
//
//
//----------------------------------------PÚBLIQUES----------------
//
#include "SiTSio.h"
void IoInit(void) {
	T = TiGetTimer();
	AnInicialitza(&Cua0);	AnInicialitza(&Cua1);	AnInicialitza(&Cua2);	AnInicialitza(&Cua3);
	// El port RD4 de sortida
	TRISDbits.TRISD4 = 0;
	Duty = 25;
	OC5CON = 0;		    // De moment, parat
	OC5R   = 50;
	OC5RS  = 50;
	OC5CON = 0x0006;	// Acció per a OC1 (PWM0)
	// El timer 2 genera una freqüencia de 625Hz per a un valor de 100 en PR2
	// si treballem a 8MHz i fixem el preescaler a 64
	PR2 = 100;
	// Programo el sentit els ports digitals
	_LATC12 = 0; _TRISC12 = 0; // AutoOff, de sortida i a zero
	
	// GPIO0	RD7
	// GPIO1	RD6
	// GPIO2	RD5
	// GPIO0	RC15
	// De moment, tots d'entrada per defecte

	// Els ports d'stop els programa el modul de motors
	// STOP0	RD8
	// STOP1	RD9
	// STOP2	RD10
	// STOP3	RD11
	// STOP4	RB15
	// STOP5	RG9
	// STOP6	RB14
	// STOP7	RC14

	// Tots els senyals d'stop son sempre d'entrada
	_TRISD8  = 1; _TRISD9 = 1; _TRISD10 = 1; _TRISD11 = 1; 
	_TRISB15 = 1; _TRISG9 = 1; _TRISB14 = 1; _TRISC14 = 1;
	_TRISC13 = 1;

	// Programació dels ports analògics
	// Selecció la tensió de 3.3V com a tensió de referència. No és la millor 
	// referència del món, però ja estarà bé.
	AD1CON2 = 0x0400; 
	// Els ports que no s'ón analògics s'han de marcar en aquest registre
	AD1PCFG =  0xFF83;
	// La relació entre els noms externs i els noms interns analògics és la següent
	// AN0			AN2
	// AN1			AN3
	// AN2			AN4
	// AN3			AN5
	// BatSample	AN6
	// Els ports que el conversor ha d'escanejar es configuren en aquest registre
	AD1CSSL = 0x007C; //AN2 fins a AN6
	// Activo interrupció cada 5 scanners (AN2..AN6)
	AD1CON2 |= (4 << 2);	// Bits SMPI0:SMPI3
	// Tempo de conversió
	// Per convertir necessita 12 TADs (clock del AD) i per a mostrejar, el que li
	// posem en els bits SAMC. Volem que faci una conversió cada 0.5ms i que interrompi cada
	// 5, amb el que tindrem un periode d'interrupció de 2.5ms, per poder filtrar 50Hz amb 8 mostres
	// TAD = TCY(ADCS+1) , amb TCY = 2/8Mhz = 250ns, si fem ADCS = 49, tenim TAD = 12.5us
	// Com que el temps de conversió és 12TAD, fem que el temps de mostreig sigui 28TAD (SAMC = 28)
	// i el temps de cada mostra serà (12+28) x 12.5us = 500us
	AD1CON3 = (49);
	AD1CON3 |= (28 << 8);
	// Esborro el flag d'interrupció
	IFS0bits.AD1IF = 0;
	// Activo la interrupció del Conversor 
	IEC0bits.AD1IE = 1;
	// Activo el conversor, integer, autoconvert i ASAM
	AD1CON1 = 0x80E4;
}

void IoMotor(void) {
	long Tics; 
	static int TAnterior;
	MotorPress(); MotorPress2();
	switch (Estat) {
		case 0:
			if ((Beat > 0) && (Beat < MAX_BEAT)) {
				TiResetTics(T); 
				++Estat; NouDuty = Duty = 0;
			}
			break;
		case 1:
			if (TiGetTics(T) >= Periode) {
				TiResetTics(T); TAnterior = 0;
			} else {	
				if (TiGetTics(T) != TAnterior) {
					Tics = TAnterior = TiGetTics(T);
					Tics = Tics * MAX_DUTY; // Compte amb no perdre bits
					Tics = Tics / P_2;
					if (TAnterior < P_2) NouDuty = Tics; 
					else NouDuty = 2*MAX_DUTY-Tics;
					OC5RS = NouDuty + MIN_DUTY;
				}			
			}
			break;
	}
}


void IoSetDirection(int Port, int Dir) {
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Dir val DIR_IN (entrada) o DIR_OUT sortida)
// Post: Defineix el sentit dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Post: Per defecte, el valor que pren un port de sortida és 0 (0V)
	// GPIO0	RD7
	// GPIO1	RD6
	// GPIO2	RD5
	// GPIO0	RC15
	unsigned char D;
	if (Dir == DIR_IN) D = 1; else D = 0;
	switch( Port) {
		case GPIO_0: _TRISD7  = D; 	break;
		case GPIO_1: _TRISD6  = D; 	break;
		case GPIO_2: _TRISD5  = D; 	break; 
		case GPIO_3: _TRISC15 = D; 	break;
	}
}

void IoSetValue(int Port, int Value) {
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Value val 1 (3.3 V) o 0 (0 V)
// Post: Defineix el valor dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a sortides
	// GPIO0	RD7
	// GPIO1	RD6
	// GPIO2	RD5
	// GPIO0	RC15
	switch( Port) {
		case GPIO_0: _LATD7  = Value; 	break;
		case GPIO_1: _LATD6  = Value; 	break;
		case GPIO_2: _LATD5  = Value; 	break; 
		case GPIO_3: _LATC15 = Value; 	break;
	}
}

int IoGetValue(int Port) {
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 o GPIO_3
// Post: Retorna el valor actual dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a entrades. Retorna 1 si la tensió en el port
// és superior a 2.3V i 0 si la tensió del port és inferior a 0.9 V
// Altrament no es pot assegurar el valor
	// GPIO0	RD7
	// GPIO1	RD6
	// GPIO2	RD5
	// GPIO0	RC15
	switch( Port) {
		case GPIO_0:	return _RD7;
		case GPIO_1:	return _RD6;
		case GPIO_2:	return _RD5;
		case GPIO_3:	return _RC15;
	}
	return 0;
}

void IoPowerOff(void) {
// Post: Apaga l'alimentació de la placa base.
	_LATC12 = 1;
}

int IoGetAnalog(int Port) {
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor analògic actual (AN_0, AN_1, AN_2 o AN_3), sense cap mena de filtratge, de
// forma que si la tensió en el port és 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.
	switch(Port) {
		case AN_0: return AN0;
		case AN_1: return AN1;
		case AN_2: return AN2;
		case AN_3: return AN3;
	}
	return 0;
}

int IoGetAnalogFiltered(int Port) { 
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor analògic actual (AN_0, AN_1, AN_2 o AN_3), amb un filtratge, basat en un promig
// mòbil de 8 mostres a ritme de 2.5 ms per mostra.
// Igual que amb GetAnalog, si la tensió en el port és 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.
	switch(Port) {
		case AN_0: return Cua0.Promig / MAX_CUES_AN;
		case AN_1: return Cua1.Promig / MAX_CUES_AN;
		case AN_2: return Cua2.Promig / MAX_CUES_AN;
		case AN_3: return Cua3.Promig / MAX_CUES_AN;
	}
	return 0;
}

int IoGetBatteryVoltage(void) {
	return BAT;
}
// Post: retorna un numero entre 0 i 1023, que indica el valor analògic del senyal BatSample


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int IoGpButtonPress(void) {
// Post: Retorna cert si s'ha premut el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.
	unsigned char tmp;
	tmp = BotoPres; BotoPres = 0;
	return tmp;
}

int IoGpButtonRelease(void) {
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.
	unsigned char tmp;
	tmp = BotoUnPres; BotoUnPres = 0;
	return tmp;
}

int IoGpButton2Press(void) {
// Post: Retorna cert si s'ha premut el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.
	unsigned char tmp;
	tmp = BotoPres2; BotoPres2 = 0;
	return tmp;
}

int IoGpButton2Release(void) {
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.
	unsigned char tmp;
	tmp = BotoUnPres2; BotoUnPres2 = 0;
	return tmp;
}

void IoSetLedBeat(int beat) {
// Pre: Beat pot valer entre 0 (leds apagats) i > 40 (leds permanentment encesos)
// Post:Els valors intermedis (de 1 a 40) indiquen la frequencia de pulsació en dècimes d'Hz  
// Exemple: Beat = 20 vol dir 2Hz
	Beat = beat; 
	if (Beat == 0) { Estat = 0; DesactivaLeds(); return; }
	if (Beat > MAX_BEAT) { Estat = 0; ActivaLeds(); return; }
	// Cas no trivial
	Periode = 10000 / Beat; 
	P_2 = Periode / 2;
	Duty = 15;
}

void IoSincro(void) {
// Post: reset del ritme de batec
	Estat = 0;
}

void IoWait(unsigned int Micros) { int micros;
// Pre : Micros és positiu
// Post: Espera el nombre de microsegons especificat. NO ES COOPERATIVA
// i per tant s'ha d'usar amb molta precaució. Esta afinada per a un clock de 8M
	micros=Micros>>1;
	while(micros!=0) {                         
		ClrWdt();
		--micros; 
		Nop(); Nop(); Nop();
	}
}


void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
	// Rutina de servei d'interrupció del conversor AD
	// S'entra aquí cada 2.5ms, després que el conversor ha processat
	// AN2..AN6 i triga 100 us
//	IO_SetValue(GPIO_1, 1); Per comprovar la temportització
	IFS0bits.AD1IF = 0; 
	// Capturo els cinv valors de conversio
	AN0 = ADC1BUF0;
	AN1 = ADC1BUF1;
	AN2 = ADC1BUF2;
	AN3 = ADC1BUF3;
	BAT = ADC1BUF4;
	// I poso els quatre analògics a la cua del promig
	AnAmitjana(&Cua0, AN0);
	AnAmitjana(&Cua1, AN1);
	AnAmitjana(&Cua2, AN2);
	AnAmitjana(&Cua3, AN3);
	// La tensió de bateria s'ofereix directament, sense cap mena de promig.
//	IO_SetValue(GPIO_1, 0);
}
