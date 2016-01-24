//
// Rutines per a manipular el display alfanum�ric del LS MAker, que t�
// quatre files per setze columnes
//

#include "../LSApi.h"
#include <stdarg.h>

void LS_LCD_Clear(void) {
// Post: Esborra el display i posa el cursor a la posici� zero en 
// l'estat en el que estava. 
	LcClear();
}

void LS_LCD_CursorOn(void){
// Post: Enc�n el cursor
// Post: La propera ordre pot trigar fins a 40us
	LcCursorOn();
}

void LS_LCD_CursorOff(void){
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us
	LcCursorOff();
}

void LS_LCD_GotoXY(unsigned int Columna, unsigned int Fila){
// Pre : Columna entre 0 i 15, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
	LcGotoXY(Columna, Fila);
}

void LS_LCD_PutChar(char C) {
// Post: Pinta C en l'actual poscici� del cursor i incrementa 
// la seva posici�. Si se supera la columna 15, el cursor salta a la fila seg�ent, 
// en cas que sigui la �ltima, el cursor salta a la posici� 0,0
	LcPutChar(C);
}

void LS_LCD_PutString(char *s) {
// Post: Pinta l'string s (null terminated apartir de la posici� actual del cursor.
// El criteri de coordenades �s el mateix que a LS_LCD_PutChar. Si se supera la columna 15, el cursor salta a la fila seg�ent, 
// en cas que sigui la �ltima, el cursor salta a la posici� 0,0
	LcPutString(s);
}

void LS_LCD_Printf(int X, int Y, char *fmt, ... ) {
// Pre : Columna entre 0 i 15, Fila entre 0 i 3
// Post: rutina equivalent a la printf, per� amb la difer�ncia que el 
// resultat el pinta en el display a la fila i columna especificada. Si se supera la columna 15, el cursor salta a la fila seg�ent, 
// en cas que sigui la �ltima, el cursor salta a la posici� 0,0
	char s[255]; 
	va_list args;
	va_start( args, fmt );
	vsprintf( s, fmt, args );
	LcGotoXY(X, Y); LcPutString(s);
    va_end( args );
}


