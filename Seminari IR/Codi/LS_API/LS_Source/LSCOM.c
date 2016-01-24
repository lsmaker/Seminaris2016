//------------------------------------------------------------------------------------------

// Aquestes rutines permeten enviar i rebre trames entre diferens LS. No serveix
// per a comunicar-se amb el control remot, ja que això ho fa de forma automàtica
// l'executiu de la API LS de forma transparent
// 
// El nivell d'intensitat de senyal que retorna la rutina LS_HiHaTrama dóna
// una idea APROXIMADA de la distància a la que es troba el LS que ha enviat
// la trama. Aquesta distància varia en funció de la humitat relativa, del
// nombre de persones o altres bèsties que es trobin entre els dos cotxes o 
// del nombre de rebots que a patiti el senyal
// 
// El format de les trames que s'envien (i això és valid per a les trames
// ocultes entre el LS i el seu control remot) segueix el següent patro
// 
// [ADREÇA DESTI, 3 bytes][ADREÇA ORIGEN, 3 bytes][Dades, 9 bytes]
//
// El hardware s'encarrega de verificar el CRC i no deixarà passar una
// trama si no es pot garantir la seva correcció.
// No hi ha cap mena de garantia de que en enviar una trama, aquesta
// arribi al seu destí, ni hi ha cap informació sobre si realment ha arribat
// 
//-----------------------------------------------------------------------------------------
#include "../LSApi.h"

void LS_COM_GetLargeMAC(unsigned char MAC[COM_LARGE_MAC_SIZE]) {
// Post: Posa en MAC el valor de l'adreça mac en format llarg
	EEGetLargeMAC(MAC);
	LS_Executiu();
}

void LS_COM_GetShortMAC(unsigned char MAC[COM_SHORT_MAC_SIZE]) {
// Post: Posa en MAC el valor abreujat de l'adreça mac en format llarg
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
// Post: Desactiva el mode promiscu. Només les trames adreçades al LS 
// seran informades a les rutines LS
	RfUnSetPromiscuousMode();
	LS_Executiu();
}

int LS_COM_FrameAvailable(void) {
// Post: Retorna cert si s'ha rebut una trama adreçada al LS amb el primer Byte igual a 0xff
// (si el mode promiscu és fals) o bé s'ha rebut una trama adreçada a qualsevol
// (però només si el mode promiscu és cert)
	LS_Executiu();
	return RfHiHaTramaUsuari();
}

int  LS_COM_GetFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Origin[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]){
// Pre : LS_COM_FrameAvailable és cert
// Post: Omple els camps Desti (que en mode no promiscu coincideix amb l'adreça local), Origen i Dades
// Retorna el nivell de RSSI (força del senyal rebut) expressat en dBm, de -10 (màxim nivell rebut)
// fins a -100 (mínim nivell rebut)
	LS_Executiu();
	return RfGetTramaLliure(Origin, Destination, Data);
}

void LS_COM_SendFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]){
// Post: munta una trama amb l'adreça destí, l'adreça propia del robot i les dades.
// i l'envia. No es garanteix que el destí la rebi.
	RfSendFrame( Destination, Data);
	LS_Executiu();
}

//Adreça broadcast \377\377\377
int LS_COM_IsBroadcastFrame(unsigned char Destination[COM_SHORT_MAC_SIZE]) { 
// Post: Retorna cert si s'ha rebut una trama adreçada al LS
// (si el mode promiscu és fals) o bé s'ha rebut una trama adreçada a qualsevol
// (però només si el mode promiscu és cert)
	LS_Executiu();
	return RfEsBroadcast(Destination);
}


//Adreça broadcast \377\377\377
void LS_COM_SendBroadcastFrame(unsigned char Data[COM_FRAME_SIZE]) {
// Post: munta una trama amb l'adreça propia del robot i les dades.
// i l'envia per broadcast. No es garanteix que cap destí la rebi.
	RfSendBroadCastFrame(Data);
	LS_Executiu();
}
