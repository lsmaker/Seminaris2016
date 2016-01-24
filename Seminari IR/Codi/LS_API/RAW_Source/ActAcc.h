//-----------------------------------------------------------------
//TAD         :Acceleròmetre MMA75660
//Autor       :FEC
//Descripció  :Programa i recull dades de l'acceleròmetre. Requereix 
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
#define LSMAKER_BASE	// Es la placa base, si no, és el control remot (hi ha codi compartit entre tots dos.
#endif 


void AcInit(void);

void AcEnd(void);

void AcMotor(void);

int AcGetTap(void);
// Post: retorna cert si hi ha hagut una condició de tap. 
// Lectura destructiva

int AcGetShake(void);
// Post: retorna cert si hi ha hagut una condició de shake. 
// Lectura destructiva

#ifdef	LSMAKER_BASE
void AcGetXYZ(float *X, float *Y, float *Z);
#else 
void AcGetXYZ(int *X, int *Y, int *Z);
#endif
// Post: XYZ valen de 90 a -90, però fent una regla de tres entre la projecció del vector 
// d'atracció gravitatoria estàtica (que l'acceleròmetre dona entre 21 i -21) i 90 graus.
// És a dir X = AcceleròmetreX * 90 / 21. Obviament, això NO es l'angle, si no el cosinus
// de l'angle. La conversió seria acos( X / 90.0) i ull amb el signe. Però l'arccosinus és
// costós des del punt de vista computacional i com que l'acceleròmetre no es gaire precís,
// creiem que l'aproximació de l'arccosinus a una recta ja és suficient per als nostres propòsits.
// Cal tenir en compte que el que més és interessa és la mesura relativa de l'angle i els canvis
// en l'angle, sobretot per detectat canvis de pendent.

void AcGetXYZInt(int *x, int *y, int *z);
// Versió en brut de l'anterior, es retornen els valors directes de l'inclinòmetre

int AcGetSwitchMode(void);
// Post: Retorna cert si el siwtch està en la posició ACC i fals si està en la posició USB
// Només valid si està definit ACC_REMOTE


