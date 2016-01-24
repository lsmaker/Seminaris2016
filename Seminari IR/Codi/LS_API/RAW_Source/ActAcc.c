//-----------------------------------------------------------------
//TAD         :Acceleròmetre MMA75660
//Autor       :FEC
//Descripció  :Programa i recull dades de l'acceleròmetre. Requereix 
// el tad I2C.
//Prefix      :Ac
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//

#ifdef	__PIC24FJ64GA006__
#define LSMAKER_BASE	// Es la placa base, si no, és el control remot (hi ha codi compartit entre tots dos.

// Aquest define serveix per a poder usar una placa que tingui l'Acceleròmetre
// avariat, ja que si no, el software es queda encallat
// #define ACC_AVARIAT	

#endif 


#ifdef	LSMAKER_BASE	
#include "../LSApi.h"
#else 
#include "I2Ti2c.h"
#include "TiTTImer.h"
#include <p24fxxxx.h>
void USBInfo(char *s);
#endif

//
//-------------------------------CONSTANTS-------------------------
//
// Registres
#define XOUT 	0x00 
#define YOUT 	0x01 
#define ZOUT 	0x02 
#define MODE_STANDBY	0x00 
#define MODE_ACTIVE	    0x01 
#define SHAKE_ACTIVE	0xe0
#define TAP_ACTIVE		0x04
#define TILT_REGISTER	0x03
#define MODE_REGISTER	0x07
#define INTSU_REGISTER	0x06
#define SR_REGISTER 	0x08
#define SPCNT_REGISTER	0x05
#define PDET_REGISTER	0x09
#define PD_REGISTER		0x0a


#define ACC_ADDRESS	0x4c

// De moment no faig cap virgueria, totes les opcions per defecte.

// Mascares
#define ALERT	(1<<6) // Cal tornar a llegir el registres X, Y o Z out
#define BIT5    (1<<5) // El bit de signe
#define MAGNITUD	(0x1f)
#define SHAKE   (1<<7)
#define TAP		(1<<5)
// Bits del registre mode
#define ACTIU		1			// Active mode
#define MAX_PROMIG 			16
#define PERIODE_MOSTREIG	10		// En ms
#define RATIO 	(90.0/(21.0*MAX_PROMIG))

//
//--------------------------END--CONSTANTS-------------------------
//
//
//-------------------------------VARIABLES-------------------------
//
static int PromigX[MAX_PROMIG];
static int PromigY[MAX_PROMIG];
static int PromigZ[MAX_PROMIG];
static int IniciX, IniciY, IniciZ;
static int FiX, FiY, FiZ;
static int PromitjaZ, PromitjaX, PromitjaY;
static int XAnt, YAnt, ZAnt; 
static int T;
static int Shake, Tap;
//
//--------------------------END--VARIABLES-------------------------
//
//-------------------------------PROTOTIPUS--LOCALS----------------
static int GetXYZ(int *X, int *Y, int *Z);
int AmitjanaX(int e);
int AmitjanaY(int e);
int AmitjanaZ(int e);
static void PosaX(int v);
static void PosaY(int v);
static void PosaZ(int v);
static void GetShake(void);
//-------------------------------PROTOTIPUS--LOCALS----------------

/*  Demo
 i2c_start();
   i2c_write(DIR_MMA7660|I2C_ESCRIBIR);
   i2c_write(RegMode);
   i2c_write(1);
   i2c_stop();
   i2c_start();
   i2c_write(DIR_MMA7660|I2C_ESCRIBIR);
   i2c_write(RegSR);
   i2c_write(0b011);
   i2c_stop();
 */
//
//-------------------------------PUBLIQUES-------------------------
//

void  AcInit(void) { int i;
	// unsigned char Tmp[1]; 
	// Activo el transistor de polarització
#ifdef LSMAKER_BASE
	_TRISB8 = 0; _LATB8 = 0;
#endif

#ifndef ACC_AVARIAT
	// Poso l'accelerometre en mode repos
	i2WriteData(ACC_ADDRESS, MODE_REGISTER, 0);
	// Activo la detecció de Shake i el Tap en els tres eixos
	i2WriteData(ACC_ADDRESS, INTSU_REGISTER, SHAKE_ACTIVE | TAP_ACTIVE);
	// I Poso el màxim de mostres per segon (120)
	i2WriteData(ACC_ADDRESS, SR_REGISTER, 0x00);	
	// Només miro el TAP en el sentit de l'eix Z i 8 comptes de llindar
	i2WriteData(ACC_ADDRESS, PDET_REGISTER, 0x68);	
	// filtre anti rebots de 9
	i2WriteData(ACC_ADDRESS, PD_REGISTER, 0x8);	
	// Poso l'accelerometre en mode actiu
	i2WriteData(ACC_ADDRESS, MODE_REGISTER, MODE_ACTIVE);
#endif
	// De moment, res més, el Joan ho ha de millorar molt més en el control remot
	T = TiGetTimer();

//	IniciZ = FiZ = IniciY = FiY = 0;
	IniciZ = FiZ = IniciY = FiY = IniciX = FiX = 0;
	PromitjaZ = PromitjaY = 0;
	XAnt = YAnt = ZAnt = 0;
	GetXYZ(&XAnt, &YAnt, &ZAnt); 
	for (i = 0; i < MAX_PROMIG; i++) {
		PosaY(YAnt); PosaZ(ZAnt); PosaX(XAnt);
	}
	// per poder llegir RB15
	AD1PCFGbits.PCFG9  = 1;    // per poder consultar l'estat del MISO
	Shake = Tap = 0;
}

void AcEnd(void) {
}

int AcGetSwitchMode(void) {
// Post: Retorna cert si el siwtch està en la posició ACC i fals si està en la posició USB
	return _RB15;
}

int AcGetTap(void) {
// Post: retorna cert si hi ha hagut una condició de tap. 
// Lectura destructiva
	int t;
	t = Tap; Tap = 0;
	return t;
}


int AcGetShake(void) {
// Post: retorna cert si hi ha hagut una condició de shake. 
// Lectura destructiva
	int t;
	t = Shake; Shake = 0;
	return t;
}

void AcGetXYZInt(int *x, int *y, int *z) {
	*x = XAnt; 
	*y = YAnt;
	*z = ZAnt;
}


#ifdef	LSMAKER_BASE
void AcGetXYZ(float *X, float *Y, float *Z) {
#else 
void AcGetXYZ(int *X, int *Y, int *Z) {
#endif
// Post: XYZ valen de 90 a -90, però fent una regla de tres entre la projecció del vector 
// d'atracció gravitatoria estàtica (que l'acceleròmetre dona entre 21 i -21) i 90 graus.
// És a dir X = AcceleròmetreX * 90 / 21. Obviament, això NO es l'angle, si no el cosinus
// de l'angle. La conversió seria acos( X / 90.0) i ull amb el signe. Però l'arccosinus és
// costós des del punt de vista computacional i com que l'acceleròmetre no es gaire precís,
// creiem que l'aproximació de l'arccosinus a una recta ja és suficient per als nostres propòsits.
// Cal tenir en compte que el que més és interessa és la mesura relativa de l'angle i els canvis
// en l'angle, sobretot per detectat canvis de pendent.
#ifdef	LSMAKER_BASE
	*X = XAnt * RATIO; 
	*Y = YAnt * RATIO; 
	*Z = ZAnt * RATIO;
#else 
	*X = XAnt / MAX_PROMIG; 
	*Y = YAnt / MAX_PROMIG;
	*Z = ZAnt / MAX_PROMIG;
#endif

}

void AcMotor(void) { int X, Y, Z;
	static int Estat = 0;
	switch(Estat) {
		case 0:
			TiResetTics(T); Estat++;
			break; 
		case 1:
			if (TiGetTics(T) >= PERIODE_MOSTREIG) {
				// Han passat 10 ms, cal adquirir i amitjanar
				if (GetXYZ(&X, &Y, &Z))  {
					YAnt = AmitjanaY(Y); ZAnt = AmitjanaZ(Z); XAnt = AmitjanaX(X);
				} else {
#ifndef	LSMAKER_BASE
					USBInfo("Merda\n\r");
#endif 
				}
				GetShake();
				TiResetTics(T);
			}
			break;
	}
}

int GetXYZ(int *X, int *Y, int *Z) {
// Post: XYZ valen de 32 a -32
	unsigned char Tmp[3]; int i; 
	for (i = 0; i < 10; i++) {
#ifndef ACC_AVARIAT
		i2ReadData(ACC_ADDRESS, XOUT,3, Tmp);
#else 
		Tmp[0] = Tmp[1] = Tmp[2] = 10;
#endif
		if (((Tmp[0] & ALERT) == 0) && ((Tmp[1] & ALERT) == 0) && ((Tmp[2] & ALERT) == 0)) {
			if (Tmp[0] & BIT5) *X =(Tmp[0]&MAGNITUD)-32; else *X = Tmp[0];
			if (Tmp[1] & BIT5) *Y =(Tmp[1]&MAGNITUD)-32; else *Y = Tmp[1];
			if (Tmp[2] & BIT5) *Z =(Tmp[2]&MAGNITUD)-32; else *Z = Tmp[2];
			return 1;
		}
	}
	return 0;
}

static void GetShake(void) {
	unsigned char Tmp[3];
	i2ReadData(ACC_ADDRESS, TILT_REGISTER, 1, Tmp);
	if ((Tmp[0] & ALERT) == 0) {
		if (Tmp[0] & SHAKE) Shake = 1;
		if (Tmp[0] & TAP) Tap = 1;
	}
}


static void PosaX(int v) {
	PromigX[IniciX++] = v; 
	if (IniciX == MAX_PROMIG) IniciX = 0;
}

static void PosaY(int v) {
	PromigY[IniciY++] = v; 
	if (IniciY == MAX_PROMIG) IniciY = 0;
}

static void PosaZ(int v) {
	PromigZ[IniciZ++] = v; 
	if (IniciZ == MAX_PROMIG) IniciZ = 0;
}

static int TreuX(void) {
	int t; 
	t = PromigX[FiX++];
	if (FiX == MAX_PROMIG) FiX = 0;
	return t;
}

static int TreuY(void) {
	int t; 
	t = PromigY[FiY++];
	if (FiY == MAX_PROMIG) FiY = 0;
	return t;
}

static int TreuZ(void) {
	int t; 
	t = PromigZ[FiZ++];
	if (FiZ == MAX_PROMIG) FiZ = 0;
	return t;
}

int AmitjanaX(int e) {
	PromitjaX = PromitjaX-TreuX();
	PromitjaX = PromitjaX+e;
	PosaX(e);
	return PromitjaX;
}

int AmitjanaY(int e) {
	PromitjaY = PromitjaY-TreuY();
	PromitjaY = PromitjaY+e;
	PosaY(e);
	return PromitjaY;
}

int AmitjanaZ(int e) {
	PromitjaZ = PromitjaZ-TreuZ();
	PromitjaZ = PromitjaZ+e;
	PosaZ(e);
	return PromitjaZ;
}

