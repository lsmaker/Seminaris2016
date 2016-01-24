#include "../LSApi.h"


void LS_IO_SetDirection(int Port, int Dir) {
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Dir val DIR_IN (entrada) o DIR_OUT sortida)
// Post: Defineix el sentit dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Post: Per defecte, el valor que pren un port de sortida és 0 (0V)
	LS_Executiu();
	IoSetDirection(Port, Dir);
}


void LS_IO_SetValue(int Port, int Value) {
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Value val 1 (3.3 V) o 0 (0 V)
// Post: Defineix el valor dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a sortides
	IoSetValue(Port, Value);
	LS_Executiu();
}

int LS_IO_GetValue(int Port){
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 o GPIO_3
// Post: Retorna el valor actual dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a entrades. Retorna 1 si la tensió en el port
// és superior a 2.3V i 0 si la tensió del port és inferior a 0.9 V
// Altrament no es pot assegurar el valor
	LS_Executiu();
	return IoGetValue(Port);
}

int LS_IO_GetAnalog(int Port) {
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor analògic actual (AN_0, AN_1, AN_2 o AN_3), sense cap mena de filtratge, de
// forma que si la tensió en el port és 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.
	LS_Executiu();
	return IoGetAnalog(Port);
}

int LS_IO_GetAnalogFiltered(int Port) {
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor analògic actual (AN_0, AN_1, AN_2 o AN_3), amb un filtratge, basat en un promig
// mòbil de quatre mostres a ritme de 10 ms per mostra.
// Igual que amb GetAnalog, si la tensió en el port és 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.
	LS_Executiu();
	return IoGetAnalogFiltered(Port);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int LS_IO_GpButtonPress(void) {
// Post: Retorna cert si s'ha premut el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.
	LS_Executiu();
	return IoGpButtonPress();
}

int LS_IO_GpButtonRelease(void) {
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.
	LS_Executiu();
	return IoGpButtonRelease();
}

int LS_IO_GpButton2Press(void) {
// Post: Retorna cert si s'ha premut el polsador GpButton 2, convenientment filtrat contra
// rebots. La lectura és destructiva.
	LS_Executiu();
	return IoGpButton2Press();
}

int LS_IO_GpButton2Release(void) {
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton 2, convenientment filtrat contra
// rebots. La lectura és destructiva.
	LS_Executiu();
	return IoGpButton2Release();
}

void LS_IO_SetLedBeat(int Beat) {
// Pre: Beat pot valer entre 0 (leds apagats) i > 40 (leds permanentment encesos)
// Post:Els valors intermedis (de 1 a 40) indiquen la frequencia de pulsació en dècimes d'Hz  
// Exemple: Beat = 20 vol dir 2Hz
	IoSetLedBeat(Beat); 
	LS_Executiu();
}


void LS_IO_Sincro(void) {
// Post: reset del ritme de batec
	IoSincro();
	LS_Executiu();
}
