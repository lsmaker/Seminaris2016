//------------------------------------------------------------------------------------------

// Aquestes rutines permeten enviar i rebre trames entre diferens LS. No serveix
// per a comunicar-se amb el control remot, ja que aix� ho fa de forma autom�tica
// l'executiu de la API LS de forma transparent
// 
// El nivell d'intensitat de senyal que retorna la rutina LS_HiHaTrama d�na
// una idea APROXIMADA de la dist�ncia a la que es troba el LS que ha enviat
// la trama. Aquesta dist�ncia varia en funci� de la humitat relativa, del
// nombre de persones o altres b�sties que es trobin entre els dos cotxes o 
// del nombre de rebots que a patiti el senyal
// 
// El format de les trames que s'envien (i aix� �s valid per a les trames
// ocultes entre el LS i el seu control remot) segueix el seg�ent patro
// 
// [ADRE�A DESTI, 3 bytes][ADRE�A ORIGEN, 3 bytes][Dades, 9 bytes]
//
// El hardware s'encarrega de verificar el CRC i no deixar� passar una
// trama si no es pot garantir la seva correcci�.
// No hi ha cap mena de garantia de que en enviar una trama, aquesta
// arribi al seu dest�, ni hi ha cap informaci� sobre si realment ha arribat
// 
//-----------------------------------------------------------------------------------------
#include "../LSApi.h"

void LS_COM_GetLargeMAC(unsigned char MAC[COM_LARGE_MAC_SIZE]) {
// Post: Posa en MAC el valor de l'adre�a mac en format llarg
	EEGetLargeMAC(MAC);
	LS_Executiu();
}

void LS_COM_GetShortMAC(unsigned char MAC[COM_SHORT_MAC_SIZE]) {
// Post: Posa en MAC el valor abreujat de l'adre�a mac en format llarg
	EEGetShortMAC(MAC);
	LS_Executiu();
}

void LS_COM_SetPromiscuousMode(void) {
// Post: Activa el mode promiscu. Totes les trames coherents que es detectin
// seran informades a les rutines LS
	RfSetPromiscuousMode();
	LS_Executiu();
}

void LS_COM_UnsetPromiscuousMode(void) {
// Post: Desactiva el mode promiscu. Nom�s les trames adre�ades al LS 
// seran informades a les rutines LS
	RfUnSetPromiscuousMode();
	LS_Executiu();
}

int LS_COM_FrameAvailable(void) {
// Post: Retorna cert si s'ha rebut una trama adre�ada al LS amb el primer Byte igual a 0xff
// (si el mode promiscu �s fals) o b� s'ha rebut una trama adre�ada a qualsevol
// (per� nom�s si el mode promiscu �s cert)
	LS_Executiu();
	return RfHiHaTramaUsuari();
}

int  LS_COM_GetFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Origin[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]){
// Pre : LS_COM_FrameAvailable �s cert
// Post: Omple els camps Desti (que en mode no promiscu coincideix amb l'adre�a local), Origen i Dades
// Retorna el nivell de RSSI (for�a del senyal rebut) expressat en dBm, de -10 (m�xim nivell rebut)
// fins a -100 (m�nim nivell rebut)
	LS_Executiu();
	return RfGetTramaLliure(Origin, Destination, Data);
}

void LS_COM_SendFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]){
// Post: munta una trama amb l'adre�a dest�, l'adre�a propia del robot i les dades.
// i l'envia. No es garanteix que el dest� la rebi.
	RfSendFrame( Destination, Data);
	LS_Executiu();
}

//Adre�a broadcast \377\377\377
int LS_COM_IsBroadcastFrame(unsigned char Destination[COM_SHORT_MAC_SIZE]) { 
// Post: Retorna cert si s'ha rebut una trama adre�ada al LS
// (si el mode promiscu �s fals) o b� s'ha rebut una trama adre�ada a qualsevol
// (per� nom�s si el mode promiscu �s cert)
	LS_Executiu();
	return RfEsBroadcast(Destination);
}


//Adre�a broadcast \377\377\377
void LS_COM_SendBroadcastFrame(unsigned char Data[COM_FRAME_SIZE]) {
// Post: munta una trama amb l'adre�a propia del robot i les dades.
// i l'envia per broadcast. No es garanteix que cap dest� la rebi.
	RfSendBroadCastFrame(Data);
	LS_Executiu();
}
