//------------------------------------------------------------------------------------------
//   Aquestes rutines permeten accedir a la mem�ria no vol�til del tipus EEPROM i FLASH
// que hi ha a la placa base. El que s'enregistra en aquesta mem�ria es conserva
// eternament (o quasi), encara que s'esgoti la bateria.

//   En aquesta mateixa mem�ria (la EEPROM) hi resideix una adre�a de 6 bytes 
// que �s �nica per a cada LS. Aquesta adre�a es utilitzada en molts 
// sistemes de comuncicaci� i per aix� s'anomena adre�a MAC (Media access control) 
// o adre�a de la capa de control del medi. 

//   Les comunicacions entre el control remot i l'LS o entre LS o entre
// control remots es resoldran amb nom�s tres bytes dels sis que hi ha 
// en la mem�ria. Per aix� es distingeix entre LARGE i SHORT MAC.
//
//   L'EPROM s'organitza com un array de 128 bytes, en les que es pot escriure
// i llegir a lliure criteri, sense cap mena de restricci�.
//
//   El tractament de la mem�ria flash �s una mica m�s restrictiu, d'una banda per
// que cal esborrar un sector sense abans d'escriure-hi i d'altra banda per que
// en aquesta flash TAMB� HI HA el codi del programa. Per aquesta ra�, 
// l'acc�s a la flash el resoldrem com un fitxer sequencial, en el que 
// es pot escriure nom�s al final del fitxer. Per no allunyar-nos del 
// paradigma de C, usarem les t�piques rutines fopen, fread, fwrite i fclose
// per� nom�s permetrem dos fitxers: "FitxerA" i "FitxerB", cadscun d'ells amb un l�mit de
// 1536 bytes cadascun. 
//   Atenci�: cada cop que s'obre un d'aquests fitxers en mode d'escriptura, la CPU
// pot quedar bloquejada fins a 800 ms. Conv�, doncs, obrir els fitxers que es vulguin usar
// abans de comen�ar el bucle principal ja que, en cas contrari, observarem una aturada del
// robot de gaireb� un segon que no queda gaire presentable.
// 
//   El contingut d'aquests fitxers es podr� volcar en el PC gr�cies al programa 
// LSLoader, el qual tamb� permet inicialitzar aquests fitxers amb un contingut a priori.
//
#include "../LSApi.h"
#include <string.h>
struct FitxerFlash Files[2];

void LS_NVOL_EERead(unsigned int Pos, unsigned char *buffer, unsigned int Length) {
// Pre : Pos + Lenght < 196
// Pre : Pos i Length positius
// Post: Llegeix el nombre de caracters indicats a Lenght a partir
// de l'adre�a Pos de l'EEPROM i els posa a l'array buffer
	EERead(Pos, buffer, Length);
	LS_Executiu();
}

void LS_NVOL_EEWrite(unsigned int Pos, unsigned char *buffer, unsigned int Length) {
// Pre : Pos + Lenght < 196
// Pre : Pos i Length positius
// Post: Escriu el nombre de caracters indicats a Lenght de l'array
// buffer a partir de l'adre�a Pos de l'EEPROM
	int i;
	for (i = 0; i < Length; i++) {
		EEWrite(Pos+i, buffer[i]);
		LS_SYS_SleepMiliSecs(6);
	}
}

int LS_NVOL_Open(char *Nom, char *Mode) {
// Pre: Nom pot ser NVOL_STREAM_A o NVOL_STREAM_B. Mode pot ser "a", "r", o "w"
// Si es "r", mode lectura. La lectura ser� sequencial a partir
// de l'inici del fitxer. No es permet escriure (retornaria NVOL_INVALID_MODE)
// Si �s "w", esborra el contingut del fitxer i es permet escriure a partir
// de l'inici. No es permet llegir (retornaria NVOL_INVALID_MODE)
// Si �s "a", el fitxer s'obre en mode d'escriptura a partir del final del fitxer.
// No es permet llegir (retornaria NVOL_INVALID_MODE)
// Post: El valor que retorna s'ha de fer servir com a HANDLER en la resta de la
// rutina. En principi, no s'espera cap possible error.
	LS_Executiu();
	if (strcmp(Nom, NVOL_STREAM_A) == 0) {
		NVOpen(&Files[0], *Mode, MAX_BLOCS, EE_PUNTER_A, PM_INICI_A);
		return 0;
	} else 
	if (strcmp(Nom, NVOL_STREAM_B) == 0) {
		NVOpen(&Files[1], *Mode, MAX_BLOCS, EE_PUNTER_B, PM_INICI_B);
		return 1;
	}
	return NVOL_INVALID_MODE;
}

int LS_NVOL_Close(int HANDLE) {
// Pre : HANDLE prov� de LS_NVOL_Open
// Post: Tanca el fitxer
	LS_Executiu();
	NVClose(&Files[HANDLE]);
	return 0;
}

int LS_NVOL_Read(int HANDLE, char *buffer, unsigned int length) {
// Pre : HANDLE prov� de LS_NVOL_Open
// Post: Llegeix el nombre de caracters (length) del fitxer i els posa en buffer.
// La seguent lectura es far� a partir d'on hagi acabat aquesta.
// Retorna el nombre de caracters que ha llegir realment o EOF si ha arribat 
// al final del fitxer. Retorna INVALID_MODE si el fitxer s'ha obert amb "w" o amb "a"
	LS_Executiu();
	return NVRead(&Files[HANDLE], (unsigned char *)buffer, length); 
}

void LS_NVOL_ReOpen(int HANDLE) {
// Pre : HANDLE prov� de LS_Open. El fitxer s'ha obert en mode "r"
// Post: Posiciona el punter intern a l'inici del fitxer. La seguent
// ordre de lectura comen�ar� a partir de l'inici del fitxer
	LS_Executiu();
	NVReopen(&Files[HANDLE]);
}


int LS_NVOL_Write(int HANDLE, char *buffer, unsigned int length) {
// Pre : HANDLE prov� de LS_NVOL_Open 
// Post: Escriu el nombre de caracters del buffer (length) en el fitxer
// La seguent escriptura es far� a partir d'on hagi acabat aquesta.
// Retorna el nombre de caracters que ha escriut realment o EOF si ha arribat 
// al l�mit final del fitxer (5k)
// Retorna INVALID_MODE si el fitxer s'ha obert amb "r"
	LS_Executiu();
	return NVWrite(&Files[HANDLE], buffer, length); 
}

int LS_NVOL_Eof(int HANDLE) {
// Pre : HANDLE prov� de LS_NVOL_Open.
// Post: retorna cert si el punter intern del fitxer ha arribat al final,
// tant per lectura com per escriptura.
	LS_Executiu();
	return NVEof(&Files[HANDLE]); 
}

int LS_NVOL_Size(int HANDLE) {
// Pre : HANDLE prov� de LS_NVOL_Open.
// Post: Retorna el nombre de car�cters que t� el fitxer
	LS_Executiu();
	return NVSize(&Files[HANDLE]); 
}
