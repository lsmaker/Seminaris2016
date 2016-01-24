//-----------------------------------------------------------------
//TAD         :Acceler�metre MMA75660
//Autor       :FEC
//Descripci�  :Programa i recull dades de l'acceler�metre. Requereix 
// el tad I2C.
//Prefix      :Ac
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//

//
//-------------------------------CONSTANTS-------------------------
//
// Registres
//
//--------------------------END--CONSTANTS-------------------------
//
//
//-------------------------------PUBLIQUES-------------------------
//
#ifdef	__PIC24FJ64GA006__
#define LSMAKER_BASE	// Es la placa base, si no, �s el control remot (hi ha codi compartit entre tots dos.
#endif 


void AcInit(void);

void AcEnd(void);

void AcMotor(void);

int AcGetTap(void);
// Post: retorna cert si hi ha hagut una condici� de tap. 
// Lectura destructiva

int AcGetShake(void);
// Post: retorna cert si hi ha hagut una condici� de shake. 
// Lectura destructiva

#ifdef	LSMAKER_BASE
void AcGetXYZ(float *X, float *Y, float *Z);
#else 
void AcGetXYZ(int *X, int *Y, int *Z);
#endif
// Post: XYZ valen de 90 a -90, per� fent una regla de tres entre la projecci� del vector 
// d'atracci� gravitatoria est�tica (que l'acceler�metre dona entre 21 i -21) i 90 graus.
// �s a dir X = Acceler�metreX * 90 / 21. Obviament, aix� NO es l'angle, si no el cosinus
// de l'angle. La conversi� seria acos( X / 90.0) i ull amb el signe. Per� l'arccosinus �s
// cost�s des del punt de vista computacional i com que l'acceler�metre no es gaire prec�s,
// creiem que l'aproximaci� de l'arccosinus a una recta ja �s suficient per als nostres prop�sits.
// Cal tenir en compte que el que m�s �s interessa �s la mesura relativa de l'angle i els canvis
// en l'angle, sobretot per detectat canvis de pendent.

void AcGetXYZInt(int *x, int *y, int *z);
// Versi� en brut de l'anterior, es retornen els valors directes de l'inclin�metre

int AcGetSwitchMode(void);
// Post: Retorna cert si el siwtch est� en la posici� ACC i fals si est� en la posici� USB
// Nom�s valid si est� definit ACC_REMOTE


