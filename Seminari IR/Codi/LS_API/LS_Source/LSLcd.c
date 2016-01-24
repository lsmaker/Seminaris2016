//
// Rutines per a manipular el display alfanumèric del LS MAker, que té
// quatre files per setze columnes
//

#include "../LSApi.h"
#include <stdarg.h>

void LS_LCD_Clear(void) {
// Post: Esborra el display i posa el cursor a la posició zero en 
// l'estat en el que estava. 
	LcClear();
}

void LS_LCD_CursorOn(void){
// Post: Encén el cursor
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
// Post: Pinta C en l'actual poscició del cursor i incrementa 
// la seva posició. Si se supera la columna 15, el cursor salta a la fila següent, 
// en cas que sigui la última, el cursor salta a la posició 0,0
	LcPutChar(C);
}

void LS_LCD_PutString(char *s) {
// Post: Pinta l'string s (null terminated apartir de la posició actual del cursor.
// El criteri de coordenades és el mateix que a LS_LCD_PutChar. Si se supera la columna 15, el cursor salta a la fila següent, 
// en cas que sigui la última, el cursor salta a la posició 0,0
	LcPutString(s);
}

void LS_LCD_Printf(int X, int Y, char *fmt, ... ) {
// Pre : Columna entre 0 i 15, Fila entre 0 i 3
// Post: rutina equivalent a la printf, però amb la diferència que el 
// resultat el pinta en el display a la fila i columna especificada. Si se supera la columna 15, el cursor salta a la fila següent, 
// en cas que sigui la última, el cursor salta a la posició 0,0
	char s[255]; 
	va_list args;
	va_start( args, fmt );
	vsprintf( s, fmt, args );
	LcGotoXY(X, Y); LcPutString(s);
    va_end( args );
}


