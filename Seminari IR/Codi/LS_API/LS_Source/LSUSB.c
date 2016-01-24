//-----------------------------------------------------------------------------------------
// Descripció: Gestió de la consoloa
// Prefix    : LS_USB
//  Aquestes rutines simulen el comportament de les rutine estàndards d'entrada-sortida, com
// ara getc, scanf, printf, putc, etc. La consola del LSMaker es el programa LSLoader, el qual,
// si es connecta l'LSMaker a un PC mitjançant el cable USB, envia el que s'escriu pel teclat cap
// a les rutines LS_USB_sscanf i LS_USB_getc i el resultat de les rutines LS_USB_printf i 
// LS_USB_putc apareix a la finestra central del programa LSLoader
// 
#include "../LSApi.h"
#include <stdarg.h>


int LS_USB_CharAvailable(void){
// Post: retorna el nombre de caracters que ha rebut l'LSMaker i que encara no han estat
// processats per cap rutina
	LS_Executiu();
	return SiCharAvail();
}

char LS_USB_getc(void) {
// Post: retorna un caràcter que ha rebut l'LSMaker. Si no hi ha cap caràcter, aquesta rutina 
// s'espera fins que arribi. 
	while (!SiCharAvail()) {
		LS_Executiu();
	}
	return SiGetChar();
}

void LS_USB_putc(char c) {
// Post: Envia aquest caràcter cap al PC
	LS_Executiu();
	SiSendChar(c);
}

unsigned int LS_USB_gets(char *buffer, ...) {
// Post: Processa el flux de caràcters d'entrada fins que trobi un '\r' (tecla enter) o un '\n' 
// (final de linia oficial) i ho diposita en l'array buffer. Llavors, podeu utilitzar la funció
// sscanf(bufer, ...) per a formatejar la informació d'entrad a la vostra voluntat, exactament igual 
// que ho fa la rutina estandard scanf. Vegeu la documentació d'aquesta rutina en els llibres
// de text del llenguatge C o en les innumerables referències que trobareu per internet i vigila
// de no oblidar el & al davant de les variables que no siguin arrays.
	int i; char c;
	i = 0;
	do {
		c = LS_USB_getc(); LS_USB_putc(c);
		if (c == '\n') c = '\r';
		if (c != '\r') buffer [i++] = c;
	} while (c != '\r');
	buffer[i] = 0;
	return i;
}


unsigned int LS_USB_printf(const char *fmt, ...) {
// Post: Formateja els paràmetres exactament igual que ho fa la rutina estandard printf i envia el 
// resultat cap el PC. Vegeu les immmenses posibilitats que ofereix la funció printf en els llibres
// de text del llenguatge C o en les innumerables referències que trobareu per internet.
	char s[255]; int r;
	va_list args;
	va_start( args, fmt );
	r = vsprintf( s, fmt, args );
	SiPuts(s);
    va_end( args );
	return r;
}
