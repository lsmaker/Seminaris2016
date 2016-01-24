//-----------------------------------------------------------------
//TAD         :Transceptor de radiofreq��ncia
//Autor       :Xavier Tarrag�
//Descripci�  :Gesti� de la comunicaci� amb el CC2500
//Prefix      :Rf
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//

#define RF_FRAME_SIZE 		9
#define RF_LEN_ADDRESS		3
#define COM_LARGE_MAC_SIZE		(RF_LEN_ADDRESS+3)
#define COM_SHORT_MAC_SIZE		RF_LEN_ADDRESS
#define COM_FRAME_SIZE			RF_FRAME_SIZE 

struct Trama {
	char Broadcast; // Cert si la trama que hem rebut era un broadcasr
	char Origen[RF_LEN_ADDRESS]; // Adre�a origen
	char OpCode;
	unsigned int Data1, Data2, Data3, Data4;
};
//
//--------------------------------PUBLIQUES---AREA-----------
//

void RfInit();
// Cal que el Tad EEPROM estigui inicialitzat, ja que necessitem llegir l'adre�a local i 
// la del control remot. En principi, n'hi ha prou a inicialitzar l'SPI

void RfEnd(void);

void RfMotor(void);

int RfHiHaTrama(void);
// Post: retorna el nombre de trames rebudes  a la cua de recepci�
// No �s per a us de la API, si no del Tad protocol

struct Trama RfGetTrama(void);
// Pre: HiHaTrama �s cert
// Post: Retorna i extrau la primera trama de la cua de recepci�
// No �s per a us de la API, si no del Tad protocol

int RfHiHaTramaUsuari(void);
// Post: Retorna el nombre de trames rebudes que tenen un 0xff en el camp
// opcode

int  RfGetTramaLliure(unsigned char Origin[COM_SHORT_MAC_SIZE], unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]);
// Pre: RfHiHaTramaUsuari �s cert
// Post: omple el camp origi, destination i data de la trama d'usuari 
// Retorna el nivell de RSSI (for�a del senyal rebut) expressat en dBm, de -10 (m�xim nivell rebut)
// fins a -100 (m�nim nivell rebut)

void RfSetPromiscuousMode(void);
// Post: posa el modul de RF en mode promiscu (per a les trames lliures)

void RfUnSetPromiscuousMode(void);
// Post: posa el modul de RF en mode promiscu (per a les trames lliures)

int RfEsBroadcast(unsigned char Destination[COM_SHORT_MAC_SIZE]);
// Post: retorna cert si aquesta adre�a (destination) es una adre�a de broadcast

void RfSendTrama(unsigned char *Desti ,unsigned char OP_Code, unsigned int Data1, unsigned int Data2, unsigned int Data3, unsigned int Data4);
// Post: Envia una trama a l'adre�a desti amb OpCode, Data1, Data2, Data3 i Data4  com les 9 (COM_FRAME_SIZE) dades de la trama
// Es una rutina d'us intern, que nom�s fa servir el tad protocol

void RfReSendTrama(void);
// Post: Retransmet la �ltima trama

void RfSendFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]);
// Post: Envia les dades de Data a l'adre�a dest�. 

void RfSendBroadCastFrame(unsigned char Data[COM_FRAME_SIZE]);
// Post: munta una trama amb l'adre�a propia del robot i les dades.
// i l'envia per broadcast. No es garanteix que cap dest� la rebi.


