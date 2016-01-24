//-----------------------------------------------------------------
//TAD         :Motors del robot LS MAKER
//Autor       :FEC
//Descripció  :Gestiona els 4 PWM del PIC24FJ64GA006
//Prefix      :Mt
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//
//----------------------------------------PROTOTIPUS---------------
//
struct Ordre {
	char Opcode; // M_AV, M_RE, M_GD, M_GE	(en honor al Logo)
	char Velocitat;
	char StopBits;
	int Radi;
	int Temps;
};

#include "../LSApi.h"
#include "math.h"

void ExecutaOrdre(void);
void ParaMotors(void);
void GetNivellBateria(void);
float Interpola(int b, int a, int x);
float InterpolaW(int b, int a, int x);
float GetV(int speed);
float GetW(int speed);

void EnviaRespostaMotors( int Durada, int StopReasons);
//
//------------------------------END-------PROTOTIPUS---------------
//

//
//----------------------------------------CONSTANTS----------------
//
#define RADI_COTXE	14	// en cm
#define RADI_MAXIM  300	// en cm, més que això, ja es considera radi infinit.
#define DELTA_T		10	// Milisegons de cada quantum de mesura de posició
#define DELTA_T_SEGONS	1.E-2	// Igual que l'anterior però expressat en segons
#define PAS_ADQUISICIO	500	// En milisegons, periode d'adquisició de posició.

#define ENABLE0  (_LATE6)
#define ENABLE1  (_LATE5)
#define ENABLE2  (_LATE7)
#define ENABLE3  (_LATB9)

#define DIR0 	(_LATB10)
#define DIR1 	(_LATB11)
#define DIR2 	(_LATB12)
#define DIR3 	(_LATB13)

// Taula de velocitats lineals
// En dècimes de centímetres per a una mesura de 724 en el convesors (7.1V a bateria) 
const unsigned int Lineal724[5] = {
	0,
	145, // A velocitat 40, 14.5cm per segon
	275, // A velocitat 60, 27,5 cm per segon
	390, // A velocitat 80, 39,0 cm per segon
	525  // A velocitat 100, 52.5 cm per segon
};

const unsigned int Omega724[5] = {
	0,
	750, // A velocitat 40, 75 graus per segon
	1300, // A velocitat 60, 130 graus per segon
	1900,// A velocitat 80, 190 graus per segon
	2400 // A velocitat 100, 240 graus per segon
};

float Interpola(int b, int a, int x);
float InterpolaW(int b, int a, int x);
float GetV(int speed);
float GetW(int speed);
void MotorPosicio(void);

//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------VARIABLES----------------
//

// Valors per defecte de les variables de calibració si no hi ha res a l'EEPROM
static char CalEsquerra = 100;
static char CalDreta = 100;
static char CalTraccio = 50;
static unsigned char PosServo2 , PosServo3;
static float CorreccioTraccio = 1.0;
static unsigned char Adquirint = 0;
static int NivellBateria;
static struct Ordre OrdreAct;
static int T; 
static int EstatMoviment, Durada;
// Només per a test!
static int RN0, RN1, RN2, RN3;
static int RDir0, RDir1, RDir2, RDir3;
static int HiHaOrdre;
// Variables de posicionament
static float V,W,Rx,Ry,Dx,Dy; // Noms ben curts, com a l'any 1981 amb Fortran IV
static char NivellStops; 
//
//------------------------------END-------VARIABLES----------------
//

//
//----------------------------------------PÚBLIQUES----------------
//
void MtInit(void){
	// Programo els ports d'stop
	TRISDbits.TRISD8  = 1; 	// STOP0
	TRISDbits.TRISD9  = 1;	// STOP1
	TRISDbits.TRISD10 = 1;	// STOP2
	TRISDbits.TRISD11 = 1;	// STOP3
	TRISBbits.TRISB15 = 1;	// STOP4
	TRISGbits.TRISG9  = 1;	// STOP5
	TRISBbits.TRISB14 = 1;	// STOP6
	TRISCbits.TRISC14 = 1;	// STOP7
	// Programo els ports de control dels motors
	TRISEbits.TRISE5 = 0;
	TRISEbits.TRISE6 = 0;
	TRISEbits.TRISE7 = 0;
	TRISBbits.TRISB9 = 0;
	TRISBbits.TRISB10 = 0;	// Dir0
	TRISBbits.TRISB11 = 0;	// Dir1
	TRISBbits.TRISB12 = 0;	// Dir2 
	TRISBbits.TRISB13 = 0;  // Dir3

	// Els que poden ser analògics cal reconfigurar-los, però ja ho ha fet l'init d'IO
	// Paro els motors
	ENABLE0 = 0;
	ENABLE1 = 0;
	ENABLE2 = 0;
	ENABLE3 = 0;
	// Recullo la calibració de l'eprom
	//  en CalEsquerra, CalDreta
	// --------------------------------------PENDENT--------------------
	// Programo els PWM dels motors
	// Usaré el Timer2 com a base de temps comú
	OC1CON = 0;		    // De moment, parat
	OC1R   = 50;
	OC1RS  = 50;
	OC1CON = 0x0006;	// Acció per a OC1 (PWM0)
	OC2CON = 0;		    // De moment, parat
	OC2R   = 50;
	OC2RS  = 50;
	OC2CON = 0x0006;	// Acció per a OC2 (PWM1)

	//-----------------------------Servo en PW2 i PW3
	// Programo el timer 3 per a una frequencia de 2us. Comptant que el preescaler rep 4M (8Mhz / 2)
	T3CONbits.TCKPS0 = 1;	// Preescaler de timer3 a 1:8
	T3CONbits.TCKPS1 = 0;	// Preescaler de timer3 a 1:8
	T3CONbits.TON = 1;      // Activo timer 
	// Inicialitza la sortida de PWM2 (OC3) per ajustar la durada del pols entre 1 i 2 ms
	OC3CON = 0;		    // De moment, parat
	OC3R   = 500;
	OC3RS  = 500;
	OC3CON = 0x0006 | 8 ;	// Que depengui del timer 3, en mode PMW
	// El port RD2 de sortida (OC3)
	TRISDbits.TRISD2 = 0;
	// Amb 10us de tic a timer 3, tenim 1ms si posem 500 a OC3RS i de 2ms si posem 1000 a OC3RS
	// el periode total el fixem amb PR2 a 25 ms (12500)
	PR3 = 12500;
	// Si es vol, es pot habilitat un altre servo a OC4 compartint el timer 3
	// El port RD3 de sortida (OC4)
	TRISDbits.TRISD3 = 0;
	OC4CON = 0;		    // De moment, parat
	OC4R   = 500;
	OC4RS  = 500;
	OC4CON = 0x0006 | 8 ;	// Que depengui del timer 3, en mode PMW
	// Els dos servos a la posicio neutre (50)
	OC3RS = 500+50*5;
	OC4RS = 500+50*5;
	PosServo2 = PosServo3 = 50;
	//-----------------------------End Servo en PW2 i PW3
	

	// El timer 2 genera una freqüencia de 39680Hz per a un valor de 100 en PR2
	// si treballem a 8MHz i fixem el preescaler a 64
	PR2 = 100;
	// Preescaler a 1
	T2CONbits.TCKPS0 = 0;	// Preescaler de timer2 a 1
	T2CONbits.TCKPS1 = 0;	// Preescaler de timer2 a 1
	T2CONbits.TON = 1;      // Activo timer 
	HiHaOrdre = 0;
	T = TiGetTimer();
	// Agafo les dades de calibració
	if (EEVerge()) {
		// Els valors per defecte estan a dalt de tot del fitxer
	} else {
		EERead(EE_CALIBRA_ESQUERRA, (unsigned char*)&CalEsquerra, 1);		
		EERead(EE_CALIBRA_DRETA,    (unsigned char*)&CalDreta, 1);		
		EERead(EE_CALIBRA_TRACCIO,  (unsigned char*)&CalTraccio, 1);		
		CorreccioTraccio = (50 - CalTraccio); // De +50 a -50
		CorreccioTraccio = 1 + CorreccioTraccio / 100.0; // de 0.5 a 1.5
		// LS_USB_printf("Correcio Traccio %f\n", CorreccioTraccio);
	}
	V = W = Rx = Ry = Dy = 0;
	Dx = 1;
	NivellStops = 1;
}

void MtEnd(void){
}

int MtGetNivell(int quin) {
	switch (quin) {
		case 0: return RN0; 
		case 1: return RN1; 
		case 2: return RN2; 
		case 3: return RN3; 
	}
	return 0;
}

int MtGetDir(int quin) {
	switch (quin) {
		case 0: return RDir0; 
		case 1: return RDir1; 
		case 2: return RDir2; 
		case 3: return RDir3; 
	}
	return 0;
}
// 
// Rutines primitives
// 

void MtSetServo(int NumServo, int Posicio) {
// Pre: Num Servo val 2 o 3 (
// Pre: Posicio  es un numero entre 0 i 100
// Post: Genera un pols repetitiu que val 1ms si Pos val 0 i 2 ms si pols val 1
// i entremig, pren valors de forma proporcional
// Aquests valors (0,100) és corresponen amb les posicions extremes del servo (en teoria), les
// quals depenen de cada servo concret, ja que no tots tenen el mateix angle d'obertura
// Segurament pots anar més enllà d'aquests límits
	if (NumServo == 2) { OC3RS = 500+Posicio*5; PosServo2 = Posicio; }
	if (NumServo == 3) { OC4RS = 500+Posicio*5; PosServo3 = Posicio; }
}

unsigned int MtGetPosServo(int NumServo) {
// Pre: Num Servo val 2 o 3 
// Post: Retorina la posició actual, fixada per MtSetServo
	if (NumServo == 2) return PosServo2;
	if (NumServo == 3) return PosServo3;
	return 0;
}

void MtSetPWM(int Motor, int Nivell, int Direccio) {
  // Pre: Motor pot valer M_ESQUERRA, M_DRET, M_2, M_3
  // M_ESQUERRA i M_DRET fan referència al control dels motors nadius de la placa
  // Omnia i M_2 es correspon als senyals PWM2, DIR2 i !Enable2, mentre que M_3
  // fa el mateix per a PWM3, DIR3, i !Enable3  
  // Nivell va des de 1 fins a 100, 0 vol dir que l'enable és fals (aturat)
  // Direcció pot ser M_DIR_ENDAVANT o M_DIR_ENDARRERA
  // Pre: No es poden encuar més de MAX_ORDRES
  // Post: Fixa el valor del duty del PWM corresponent, així com el valor dels 
  // senyals d'enable i direcció respectius.
	char Dir;
	if (Nivell > 100) Nivell = 100;
	if (Nivell < 0) Nivell = 0;
	if (Nivell == 0){
		// Cal Aturar els motors
		switch (Motor) {
			case M_ESQUERRA: ENABLE0 = 0; RN0 = 0; break;
			case M_DRET:     ENABLE1 = 0; RN1 = 0; break;
			case M_2: 		 ENABLE2 = 0; RN2 = 0; break;
			case M_3: 		 ENABLE3 = 0; RN3 = 0; break;
		}
		return;
	} else {
		// Cal engegar els motors
		switch (Motor) {
			case M_ESQUERRA: ENABLE0 = 1; break;
			case M_DRET:     ENABLE1 = 1; break;
			case M_2: 		 ENABLE2 = 1; break;
			case M_3: 		 ENABLE3 = 1; break;
		}
	}
	// Reprogramo el PWM
	switch (Motor) {
		case M_ESQUERRA: OC1RS = RN0 = Nivell; break;
		case M_DRET:     OC2RS = RN1 = Nivell; break;
		case M_2: 		 OC3RS = RN2 = Nivell; break;
		case M_3: 		 OC4RS = RN3 = Nivell; break;
	}
	// Ajusto la direcció 
	if (Direccio == M_DIR_ENDAVANT) Dir = 1; else Dir = 0;
	DIR2 = Dir;
	switch (Motor) {
		case M_ESQUERRA: DIR0 = RDir0 = !Dir; break;
		case M_DRET:     DIR1 = RDir1 = Dir; break;
		case M_2: 		 DIR2 = RDir2 = Dir; break;
		case M_3: 		 DIR3 = RDir3 = Dir; break;
	}
}

void MtMotor(void) {
  // Post: motor per a qestionar el moviment del robot. Es cridat pel LS_Exectutiu()  
	static int Estat = 0;
	MotorPosicio();
	switch (Estat) {
		case 0:
			++Estat; 
			break; 
		case 1:
			if (HiHaOrdre) {
				HiHaOrdre = 0; ExecutaOrdre(); ++Estat; EstatMoviment = M_MOVIMENT; TiResetTics(T);
			} else {
				// Motors parats, aprofito per obtenir la tensió de bateria
				GetNivellBateria();
			}
			break;
		case 2:
			// Hi ha una ordre en marxa, espero que s'acabi o que hi hagi un senyal d'stop
			if ( MtGetStop (OrdreAct.StopBits)) {
				// Aturada d'emergència
				ParaMotors(); EstatMoviment = M_ATURAT; Estat = 1; Durada = TiGetTics(T);
				EnviaRespostaMotors(Durada, MtGetStop (OrdreAct.StopBits));
			} else 
			if (TiGetTics(T) >= OrdreAct.Temps) {
				// L'ordre actual s'ha acabat
				if (OrdreAct.Temps != 0) {	// Temps indefinit
					ParaMotors(); EstatMoviment = M_QUIET; // No hi ha més ordres de moment
					Estat = 1; Durada = TiGetTics(T);
					EnviaRespostaMotors(Durada, MtGetStop (OrdreAct.StopBits));
				}
			}
			break;
	}
}
// 
// Rutines de moviment del robot, s'assumeix que el MtMotor és cridat per algú
// 
void MtLineal(unsigned int Time, int Speed, int StopBits) {
  // Pre : Time, en milisegons >= 0. Speed: [-100,100]
  // StopBits: Màscara amb MT_STOP_0, MT_STOP_1, ..., MT_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker es mogui cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // MotiuStop retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
	HiHaOrdre = 1;
	OrdreAct.Temps = Time;
	OrdreAct.Velocitat = Speed;
	OrdreAct.StopBits = StopBits;
	OrdreAct.Opcode = M_LIN;
}

void MtTurnRight(unsigned int Time, int Speed, unsigned int Radius, int StopBits) {
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: Màscara amb MT_STOP_0, MT_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a dreta cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores
	HiHaOrdre = 1;
	OrdreAct.Temps = Time;
	OrdreAct.Velocitat = Speed;
	OrdreAct.StopBits = StopBits;
	OrdreAct.Radi = Radius;
	OrdreAct.Opcode = M_GD;
}

void MtTurnLeft(unsigned int Time, int Speed, unsigned int Radius, int StopBits) {
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: Màscara amb M_STOP_0, M_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a esquerra cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores
	HiHaOrdre = 1;
	OrdreAct.Temps = Time;
	OrdreAct.Velocitat = Speed;
	OrdreAct.StopBits = StopBits;
	OrdreAct.Radi = Radius;
	OrdreAct.Opcode = M_GE;
}

void MtSetNivellStopBits(unsigned char Nivell) {
// Pre : Nivell pot valer 1 o 0
// Post: Defineix el valor dels senyals d'stop que fan aturar les rutines de moviment. 
// Si nivell val 1, els motors s'aturen quan els senyals d'STOPi valen 1 (3.3V) o bé zero vols
// si Nivell és zero
	NivellStops = Nivell;
}

unsigned int MtGetTimeFromDistance(int Distance, int Speed) {
  // Pre: Distance en centímetres i positiu.
  // Pre: Speed entre -100 i 100, però valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Lineal per 
  // tal que el robot avançi el nombre de centímetres expressats a Distance, a la velocitat
  // expressada per Speed. El punt òptim és dona quan la tensió de bateria està entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
	double t = GetV(Speed);	// en centímetres per segon
	if (t == 0) return 0;
	t = Distance / t;		// de centímetres a segons
	return t * 1000.0;		// de segons a milisegons
}

unsigned int MtGetTimeFromAngle(int Angle, int Speed) {
  // Pre: Angle en graus, de 0 a 360.
  // Pre: Speed entre -100 i 100, però valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Turn Left o Right per 
  // tal que el robot giri el nombre de graus expressats a Angle, a la velocitat
  // expressada per Speed. El punt òptim és dona quan la tensió de bateria està entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
  // Només es correcte si el radi de gir és menor de 10
	double t, radians;
	t = GetW(Speed);    // En radians per segon
	if (t == 0) return 0;
	radians = Angle;
	radians = radians * 3.14159 / 180.0; // De graus a radians
	t =  radians / t;	// de radians a segons
	return t * 1000.0;  // de segons a milisegons
}


void MtInitTrack () {
// Post: A partir d'aquesta crida, l'exectiu comença a calcular la posició actual en funció de les ordres rebudes
// (LS_MT_Lineal, LS_MT_TurnLeft, LS_MT_TurnRight)
// La posició actual es memoritza cada segon i es pot baixar al PC amb el cable USB i el programa LSLoader
	Rx = Ry = 0;	// Coordenades a zero
	Dx = 1; Dy = 0; // Direccio est
	W  = 0; V  = 0;
	Adquirint = 1;
	NvInitTrack();	// Inicialitzo l'enregistrament.
	NvSetTrack(Rx, Ry);
}

void MtEndTrack () {
// Post: Finalitza el seguiment de la posició 
	Adquirint = 0;
	NvEndTrack();
}

void MtGetTrack (int *x, int *y) {
// Post: Retorna la posició actual de tracking, tenint en compte que després de LS_InitTrack la posició 
// es 0,0 
	*x = Rx; *y = Ry; 
}

void MtGetDireccio (int *dx, int *dy) {
// Post: Retorna les components x, y del vector direccio (unitari) multiplicats per 1000
// a posició actual de tracking, tenint en compte que després de LS_InitTrack la posició 
// es 0,0 i el vector director és 1,0
	*dx = Dx*1000; *dy = Dy*1000; 
}

unsigned char NvAdquirint(void) {
// Retorna cert si estem adquirint
    return Adquirint;
}                            

int MtGetEstat(void) {
	// Post: Retorna: M_QUIET o M_ATURAT o M_MOVIMENT, on QUIET vol dir que ha acabat amb éxit totes
	// les ordres de moviment encomanades; ATURAT vol dir que la darrera ordre s'ha interromput per culpa
	// d'algun dels sensors d'stop i MOVIMENT vol dir que encara està executant una ordre
	return EstatMoviment;
}

int MtGetDurada(void) {
	// Post: retorna el temps real que ha durat l'última ordre
	return Durada;
}

int MtGetStop(unsigned char stopBits) {
	// Pre: StopBits: Màscara amb M_STOP0, M_STOP1, M_STOP7, que indica quins senyals d'stop es vol consultar
	// Post: retorna cert si algun dels senyals d'stop especificat a la màscara StopBits és cert en l'actualitat.
	unsigned int t = 0;
	if (NivellStops == 1) {
		if (stopBits & M_STOP0) { if (PORTDbits.RD8  == 1) t |=  M_STOP0; }  
		if (stopBits & M_STOP1) { if (PORTDbits.RD9  == 1) t |=  M_STOP1; }  
		if (stopBits & M_STOP2) { if (PORTDbits.RD10 == 1) t |=  M_STOP2; }  
		if (stopBits & M_STOP3) { if (PORTDbits.RD11 == 1) t |=  M_STOP3; }  
		if (stopBits & M_STOP4) { if (PORTBbits.RB15 == 1) t |=  M_STOP4; }  
		if (stopBits & M_STOP5) { if (PORTGbits.RG9  == 1) t |=  M_STOP5; }  
		if (stopBits & M_STOP6) { if (PORTBbits.RB14 == 1) t |=  M_STOP6; }  
		if (stopBits & M_STOP7) { if (PORTCbits.RC14 == 1) t |=  M_STOP7; }  
	} else {
		if (stopBits & M_STOP0) { if (PORTDbits.RD8  == 0) t |=  M_STOP0; }  
		if (stopBits & M_STOP1) { if (PORTDbits.RD9  == 0) t |=  M_STOP1; }  
		if (stopBits & M_STOP2) { if (PORTDbits.RD10 == 0) t |=  M_STOP2; }  
		if (stopBits & M_STOP3) { if (PORTDbits.RD11 == 0) t |=  M_STOP3; }  
		if (stopBits & M_STOP4) { if (PORTBbits.RB15 == 0) t |=  M_STOP4; }  
		if (stopBits & M_STOP5) { if (PORTGbits.RG9  == 0) t |=  M_STOP5; }  
		if (stopBits & M_STOP6) { if (PORTBbits.RB14 == 0) t |=  M_STOP6; }  
		if (stopBits & M_STOP7) { if (PORTCbits.RC14 == 0) t |=  M_STOP7; }  
	}
	return t;
}

//                                                    
//
//------------------------------END-------PÚBLIQUES----------------
//

// En condicions de bateria al 90 per cent, amb una velocitat de 50, el
// robot recorre 50 cm dos segons. En aquestes condicions, el robot gira 90
// graus en 0.7 segons. Tot això, sempre, de forma molt aproximada

int GetVelocitat(void) {
// Depen de:
// CalTraccio	entre 0 i 100, amb centre a 50 (per sobre de 50 cal allargar)
// NivellBateria de 100 (tope) a 30 (puff) 
// Ha de retornar entre 100 i 60
	int t = OrdreAct.Velocitat;
//	LS_USB_printf("V=%d CE=%d, CD = %d CT =%d BAT=%d\n", t, CalEsquerra, CalDreta, CalTraccio, NivellBateria);
	if (t > 0) return t;
	return -t; // De moment
}

int Maxim(int m) { 
	if (m > 100) return 100;
	if (m < 0) return 10;
	return m;
}

int GetVelocitatEsquerra(void) {
	int t = Maxim(GetVelocitat()-(100-CalEsquerra));
//	LS_USB_printf("VE = %d\n", t);
	return t; 
}

int GetVelocitatDreta(void) {
	int t = Maxim(GetVelocitat()-(100-CalDreta));
//	LS_USB_printf("VD = %d\n", t);
	return t; 
}

int GetRadi(void) {	// Radi de gir a dins del cotxe
	// int t = Maxim(GetVelocitat()-(100-CalDreta));
	//LS_USB_printf("VR = %d\n", t);
	return GetVelocitat();
}
int GetRadiInterior(int v) {
// La velocitat de les rodes interiors és menor que l'exterior en la mesura
// que ho és el radi de gir interior respecte el radi de gir exterior. Una mica
// groller, però és el que hi ha.
// El radi màxim evita que se sobrepassi el límit d'un enter.
	float t = v;
	if (OrdreAct.Radi < RADI_MAXIM) {
		t = t * OrdreAct.Radi;		// Radi màxim: 300
		t = t / (OrdreAct.Radi+2*RADI_COTXE);
	}
	//LS_USB_printf("Radi ext=%d Radi int = %d\r\n", v, t);
	return t+0.5;
}

int GetDireccio(void) {
	return (OrdreAct.Velocitat > 0) ? M_DIR_ENDAVANT : M_DIR_ENDARRERA;
}

void ExecutaOrdre(void) {
	switch (OrdreAct.Opcode) {
		case M_LIN:
			MtSetPWM(M_ESQUERRA, GetVelocitatEsquerra(), GetDireccio());
			MtSetPWM(M_DRET, GetVelocitatDreta(), GetDireccio());
			if (GetDireccio() == M_DIR_ENDARRERA) V = -GetV(GetVelocitatDreta()); 
			else V = GetV(GetVelocitatDreta());
			W = 0;
			// LS_USB_printf("LIN V = %f, W = %f\n\r", V, W);
			break;
		case M_GD:
			// Si el radi és menor que la meitat de la distància entre rodes, cal oposar els motors
			// Si no, sempre vas endavant
			if (OrdreAct.Radi < RADI_COTXE) {
				MtSetPWM(M_ESQUERRA, GetRadi(), M_DIR_ENDAVANT);
				MtSetPWM(M_DRET, GetRadi(), M_DIR_ENDARRERA);
				W = -GetW(GetRadi()); V = 0;
				// LS_USB_printf("GD V = %f, W = %f\n\r", V, W);
			} else {
				MtSetPWM(M_ESQUERRA, GetVelocitatEsquerra(), GetDireccio());
				MtSetPWM(M_DRET, GetRadiInterior(GetVelocitatDreta()), GetDireccio());
				if (GetDireccio() == M_DIR_ENDARRERA) V = -GetV(GetVelocitatEsquerra()); 
				else V = GetV(GetVelocitatEsquerra());
				NvSetOmega(GetVelocitatEsquerra(), GetDireccio(), GetRadiInterior(GetVelocitatDreta()), GetDireccio());
			}
			break;
		case M_GE:
			// Si el radi és menor que la meitat de la distància entre rodes, cal oposar els motors
			// Si no, sempre vas endavant
			if (OrdreAct.Radi < RADI_COTXE) {
				MtSetPWM(M_ESQUERRA, GetRadi(), M_DIR_ENDARRERA);
				MtSetPWM(M_DRET, GetRadi(), M_DIR_ENDAVANT);
				W = GetW(GetRadi()); V = 0;
				// LS_USB_printf("GE V = %f, W = %f\n\r", V, W);
			} else {
				MtSetPWM(M_ESQUERRA, GetRadiInterior(GetVelocitatEsquerra()), GetDireccio());
				MtSetPWM(M_DRET, GetVelocitatDreta(), GetDireccio());
				if (GetDireccio() == M_DIR_ENDARRERA) V = -GetV(GetVelocitatDreta()); 
				else V = GetV(GetVelocitatDreta());
				NvSetOmega(GetRadiInterior(GetVelocitatEsquerra()), GetDireccio(), GetVelocitatDreta(), GetDireccio());
			}
			break;
	}
}

void ParaMotors(void) {
	V = W = 0;
	MtSetPWM(M_ESQUERRA, 1, M_DIR_ENDAVANT);
	MtSetPWM(M_DRET, 1, M_DIR_ENDAVANT);
}

void GetNivellBateria(void) {
	unsigned long t;
#   define R9 		470		// El que importa és la seva relació
#   define R2		1000 	// i no el seu valor real
	t = IoGetBatteryVoltage(); 
	t = 33 * t;    	// Com a molt 1023 * 33 = 33759
	t = t / 1023; 	// Entre 0 i 33 (podriem escriure t /= 1023)
	t = t * (R9+R2);// Entre 0 i 48510
	t = t / R9; 	// Entre 0 i 103
	// Per ajustar les petites imprecisons de fons d'escala, limito manualment el valor màxim
	if (t > 81) NivellBateria = 100; else 
	if (t > 75) NivellBateria = 90; else 
	if (t > 72) NivellBateria = 80; else
	if (t > 70) NivellBateria = 60; else
	if (t > 68) NivellBateria = 40; else NivellBateria = 30;
}

float Interpola(int b, int a, int x) {
	float t;
	t = (b-a); t = t *x; t = t / 20;
	return (t + a) / 10; // En centimetres
}

float InterpolaW(int b, int a, int x) {
	float t;
	t = (b-a); t = t *x; t = t / 20;
	return (t + a) * 3.14159 / 1800; // En radians
}


float GetV(int speed) {
	// Calculo la velocitat a tensió nominal de bateria
	// Pendent, ajust en funció de la tensió real de la bateria
	if (speed < 0) speed = -speed;
	if (speed <= 20) return 0; 	// Facil
	if (speed < 40) return Interpola(Lineal724[1], Lineal724[0], speed-20)*CorreccioTraccio;
	if (speed < 60) return Interpola(Lineal724[2], Lineal724[1], speed-40)*CorreccioTraccio;
	if (speed < 80) return Interpola(Lineal724[3], Lineal724[2], speed-60)*CorreccioTraccio;
	return Interpola(Lineal724[4], Lineal724[3], speed-80)*CorreccioTraccio;
}

float GetW(int speed) {
	// Calculo la velocitat a tensió nominal de bateria
	// Pendent, ajust en funció de la tensió real de la bateria
	if (speed < 0) speed = -speed;
	if (speed <= 20) return 0; 	// Facil
	if (speed < 40) return InterpolaW(Omega724[1], Omega724[0], speed-20)*CorreccioTraccio;
	if (speed < 60) return InterpolaW(Omega724[2], Omega724[1], speed-40)*CorreccioTraccio;
	if (speed < 80) return InterpolaW(Omega724[3], Omega724[2], speed-60)*CorreccioTraccio;
	return InterpolaW(Omega724[4], Omega724[3], speed-80)*CorreccioTraccio;
}

void AjustaVW(int SpeedDreta, int SpeedEsquerra) {
	float radi;
	if (SpeedDreta != SpeedEsquerra) {	// Hi ha velocitat angular
		radi = 2*RADI_COTXE*SpeedEsquerra / (float)(SpeedDreta-SpeedEsquerra);
		if (radi > RADI_COTXE) W = GetV(SpeedDreta)/radi; 
		else W = GetW(SpeedDreta)/2;
	} else W = 0;
	V = GetV(SpeedDreta);
}

void NvSetOmega(int SpeedEsquerra, int DirEsquerra, int SpeedDreta, int DirDreta) {
// Pre : Speed esquerra i dreta de 0 a 100
// Pre : DirEsquerra i DirDreta poden valer  M_DIR_ENDAVANT o M_DIR_ENDARRERA
// Post: Calcula la nova velocitat angular del vector de direcció 
// a partir de les diferències entre la velocitat dels dos motors
	// Aquesta rutina només es crida quan els dos motors van en la mateixa direcció
	if (DirEsquerra == M_DIR_ENDAVANT) { 
		if (SpeedEsquerra > SpeedDreta) {
			AjustaVW(SpeedEsquerra, SpeedDreta);			
			W = -W;
		} else {
			AjustaVW(SpeedDreta, SpeedEsquerra);			
		}
	} else {
		if (SpeedEsquerra > SpeedDreta) {
			AjustaVW(SpeedEsquerra, SpeedDreta);			
			V = -V;
		} else {
			AjustaVW(SpeedDreta, SpeedEsquerra);			
			W = -W; V = -V;
		}
	}
//	LS_USB_printf("SD = %d SE=%d WD=%f WE=%f W=%f V=%f\n\r", SpeedDreta, SpeedEsquerra, GetW(SpeedDreta), GetW(SpeedEsquerra), W, V);
}

void NvSetDireccio(int SpeedEsquerra, int DirEsquerra, int SpeedDreta, int DirDreta) {
// Pre : Speed esquerra i dreta de 0 a 100
// Pre : DirEsquerra i DirDreta poden valer  M_DIR_ENDAVANT o M_DIR_ENDARRERA
// Post: Calcula el mòdul de la nova velocitat lineal i de la velocitat angular
// a partir de les diferències entre la velocitat dels dos motors. Es fa servir
// quan el robot escolta les ordres del comandament a distància
	// Cass trivials
	if ((SpeedEsquerra == 0) && (SpeedDreta == 0)) { V = W = 0; return; }
	// Motors invertits entre si
	if ((DirEsquerra == M_DIR_ENDARRERA) && (DirDreta == M_DIR_ENDAVANT)){
		V = 0;
		// W = -GetV(SpeedEsquerra); v0.3
		W = GetW(SpeedEsquerra); // v0.3
	} else 
	if ((DirEsquerra  ==  M_DIR_ENDAVANT) && (DirDreta == M_DIR_ENDARRERA)){
		V = 0; 
//		W = GetV(SpeedEsquerra);  // v0.3
		W = -GetW(SpeedEsquerra); // v0.3
	} else {
		// Motors en el mateix sentit
		NvSetOmega(SpeedEsquerra, DirEsquerra, SpeedDreta, DirDreta);
	}
//	LS_USB_printf("W=%f V=%f\n\r", W, V);
}

void RecalculaPosicio(void) { 
	float CosWT, SinWT, DxTMesDeltaT, DyTMesDeltaT;
	float V_DELTA_T_SEGONS = V*DELTA_T_SEGONS;
	float W_DELTA_T_SEGONS = W*DELTA_T_SEGONS;
	// ATENCIO, Durada del càlcul: dos milisegons;
	// Calculo les noves coordenades
	Rx = Rx +V_DELTA_T_SEGONS*Dx;
	Ry = Ry +V_DELTA_T_SEGONS*Dy;
	// Ajusto el nou vector de direccio
	CosWT = cos(W_DELTA_T_SEGONS);
	SinWT = sin(W_DELTA_T_SEGONS);
	// Matriu de rotacio
	DxTMesDeltaT = Dx*CosWT - Dy*SinWT;
	DyTMesDeltaT = Dx*SinWT + Dy*CosWT;
	// Actualizo components a t + increment de t
	Dx = DxTMesDeltaT; Dy = DyTMesDeltaT;
}



void MotorPosicio(void) {
	static int Estat = 0;
	static int T, UnSegon;
	switch (Estat) {
		case 0: T = TiGetTimer(); ++Estat; break;
		case 1: if (Adquirint == 1) {	
					++Estat; TiResetTics(T); UnSegon = 0;
				}
				break; 
		case 2: 
				if (TiGetTics(T) >= DELTA_T) {
					TRASSA_ON; 
					TiResetTics(T); RecalculaPosicio(); UnSegon += DELTA_T;
					TRASSA_OFF;
					if (UnSegon >= PAS_ADQUISICIO) {
						// Primer la posicio
						NvSetTrack(Rx+0.5, Ry+0.5);
						// Després el vector director multiplicat per 1000
						// NvSetTrack(Dx*1000, Dy*1000);
						// LS_USB_printf("ADQ Rx = %f, Ry = %f Dx = %f Dy = %f\n\r", Rx, Ry, Dx, Dy);
						UnSegon = 0;
					}
				}
				if (Adquirint == 0) {	
					--Estat;
				}				
				break;
	}
}
//
//------------------------------END-------PRIVADES-----------------
//

