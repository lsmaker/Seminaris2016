//-----------------------------------------------------------------
//TAD         :Transceptor de radiofreqüència
//Autor       :F. Escudero 
//Descripció  :Gestió de la comunicació amb el CC2500
//Prefix      :Rf
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//

#ifdef	__PIC24FJ64GA006__
#define LSMAKER_BASE	// Es la placa base, si no, és el control remot (hi ha codi compartit entre tots dos.
#endif 

#ifdef	LSMAKER_BASE	
#include "../LSApi.h"
#include "TI_CC_CC1100-CC2500.h"
#else 
#include "TI_CC_CC1100-CC2500.h"
#include "RfTRadio.h"
#include "p24FJ32GA004.h"
#include "eeteprom.h"
#include "SiTSio.h"
#include "../../SOFT_REMOT/Source/PTProto.h"
#endif
#include <stdio.h>
#include <string.h>

//#define _DEPURANT_RADIO
#ifdef  _DEPURANT_RADIO
// char AdressaRemotaFalsa[3] = { 0x26, 0xe8, 0x2a }; // Mangel 
// char AdressaRemotaFalsa[3] = { 0x26, 0xe7, 0x07 }; // Jordi Albó & Consul
// char AdressaRemotaFalsa[3] = { 0x21, 0x57, 0x8a }; // versió antiga, casa
char AdressaRemotaFalsa[3] = { 0x69, 0x69, 0x00 }; // 

#endif

//
//--------------------------------CONSTANTS---AREA-----------
//


#define LengthDATA  (RF_LEN_ADDRESS+RF_LEN_ADDRESS+RF_FRAME_SIZE)   // Longitud de les dades

#define IDLE_Mode    	0
#define RX_Mode      	1
#define TX_Mode       	2
#define MAX_TRAMES 		2
#define MAX_CUA_USUARIS	2


#define GDO0Bit 						(_RF6)

//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//


static unsigned char NET_ADDRESS_Sinc=0x0f;		// Adreça de xarxa, usada com a preambu
static unsigned char NET_ADDRESS_Device=0x32;	// Aquests valors són constants i no depenen del protocol del LSMaker

// Bufers temporals
static char txBuffer1[LengthDATA+2]; // Guarden les dades a enviar i rebre
static char rxBuffer1[LengthDATA+2];

// Estat del Tranciever    
static unsigned char RadioMode;

static int Canal;    
// PATABLE (0 dBm output power)
//extern  char paTable[] = {0xFB};
//extern  char paTableLen = 1;
static char paTable[] = {0xFF}; // 1dBm
static char paTableLen = 1;

// Cua de trames d'usuari
struct TramaUsuari {
	unsigned char Origen[RF_LEN_ADDRESS];
	unsigned char Desti[RF_LEN_ADDRESS];
	unsigned char Dades[RF_FRAME_SIZE];
	int Rssi;
} BufferU[MAX_CUA_USUARIS];
static int IniciU, FiU, QuantsU; 

// Cua de trames del control remot.
static int Inici, Fi, Quants; 
static struct Trama Trames[MAX_TRAMES];


static unsigned char Estat2500;
static unsigned char Promis;
static unsigned char RssiAct;
static unsigned char LQIAct;
static unsigned char ModeLite;

static unsigned char LocalAddress[RF_LEN_ADDRESS];
static unsigned char RemoteAddress[RF_LEN_ADDRESS];
static unsigned char BroadAddress[RF_LEN_ADDRESS] = { 0xff, 0xff, 0xff };

//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
void EngegaLed(void);
void ApagaLed (void);
void IoWait(unsigned int Micros); // En el tad eeprom
// Pre : Micros és positiu
// Post: Espera el nombre de microsegons especificat. NO ES COOPERATIVA
// i per tant s'ha d'usar amb molta precaució. Esta afinada per a un clock de 8M
unsigned char ToAscii(unsigned char c);
unsigned char ToHexa(unsigned char high, unsigned char low);
void Posa(struct Trama c);
struct Trama Treu(void);
void RFSendPacket(char *txBuffer, char size);
static void RadioInit(void);
static void MiraTrama(void);
static void ModeRecepcio(void);
void TransmetTrama(void);
void CCInici(char addr);
void CC2500_Send_DUMMYDATA(void);
void TI_CC_SPISetup(void);
void TI_CC_SPIWriteReg(char addr, char value);
char TI_CC_SPIReadReg(char addr);
void TI_CC_SPIWriteBurstReg(char addr,  char *buffer, char count);
void TI_CC_SPIReadBurstReg(char addr, char *buffer, char count);
char TI_CC_SPIReadStatus(char addr);
void TI_CC_SPIStrobe(char strobe);
void TI_CC_PowerupResetCCxxxx(void);
//
//---------------------------End--PROTOTIPUS--AREA-----------
//

//
//--------------------------------PUBLIQUES---AREA-----------
//
void RfInit(void) {
#ifdef LSMAKER_BASE
	Canal = 0; // De moment...
	// Preparo els ports del SP1
	_TRISF1 = 0;	// El chip select, de sortida
	_TRISF0 = 0;	// El de l'eeprom també, i tots dos a 1
	_LATF0 = 1;	
	_LATF1 = 1;
	// Inicialitzo variables.
	Inici = Fi = Quants = Promis = IniciU = FiU = QuantsU = 0;
	// Inicialitzo el transceptor
	RadioInit();
	// Activo Interrupcio INT0
	IFS0bits.INT0IF = 0;		// Esborro el flag	
 	INTCON2bits.INT0EP 	= 1;	// Programo interrupció per flanc de baixada.
	IEC0bits.INT0IE	= 1;		// Activo l'entrada d'interrupció.
#else 
	// Canal = canal;
	Canal = 0; // De moment...
	// Preparo els ports del SP1
	_TRISB10 = 0;	// El chip select, de sortida
	_LATB10  = 1;	
	// EL bit RB12 no pot ser analògic
	AD1PCFGbits.PCFG12 = 1;

	// Inicialitzo variables.
	Inici = Fi = Quants = Promis = IniciU = FiU = QuantsU = 0;
	// Inicialitzo el transceptor
	RadioInit();
	// Activo Interrupcio INT0
	IFS0bits.INT0IF = 0;		// Esborro el flag	
 	INTCON2bits.INT0EP 	= 1;	// Programo interrupció per flanc de baixada.
	IEC0bits.INT0IE	= 1;		// Activo l'entrada d'interrupció.
#endif
}

void RfMotor(void) {
	MiraTrama();
}

void RfEnd(void) {
}

int RfHiHaTrama(void) {
// Post: retorna el nombre de trames rebudes  a la cua de recepció
// No és per a us de la API, si no del Tad protocol
	return Quants;
}

struct Trama RfGetTrama(void){
// Pre: HiHaTrama és cert
// Post: Retorna i extrau la primera trama de la cua de recepció
// No és per a us de la API, si no del Tad protocol
	return Treu();
}

int RfHiHaTramaUsuari(void) {
// Post: Retorna el nombre de trames rebudes que tenen un 0xff en el camp
// opcode
	return QuantsU;
}

int  RfGetTramaLliure(unsigned char Origin[COM_SHORT_MAC_SIZE], unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]) {
// Pre: RfHiHaTramaUsuari és cert
// Post: omple el camp origi, destination i data de la trama d'usuari 
// Retorna el nivell de RSSI (força del senyal rebut) expressat en dBm, de -10 (màxim nivell rebut)
// fins a -100 (mínim nivell rebut)
	int Rssi;
	Rssi = BufferU[FiU].Rssi;
	memcpy(Origin, BufferU[FiU].Origen, COM_SHORT_MAC_SIZE);
	memcpy(Destination, BufferU[FiU].Desti, COM_SHORT_MAC_SIZE);
	memcpy(Data, BufferU[FiU].Dades, COM_FRAME_SIZE);
	FiU++;
	if (FiU == MAX_CUA_USUARIS) FiU = 0;
	QuantsU--;
	return Rssi;
}

void RfSetPromiscuousMode(void) { Promis = 1; }
// Post: posa el modul de RF en mode promiscu (per a les trames lliures)

void RfUnSetPromiscuousMode(void) { Promis = 0; }
// Post: posa el modul de RF en mode promiscu (per a les trames lliures)

int RfEsBroadcast(unsigned char Destination[COM_SHORT_MAC_SIZE]) {
// Post: retorna cert si aquesta adreça (destination) es una adreça de broadcast
	if (Destination[0] != BroadAddress[0]) return 0;
	if (Destination[1] != BroadAddress[1]) return 0;
	if (Destination[2] != BroadAddress[2]) return 0;
	return 1;
}



void RfSendTrama(unsigned char *Desti ,unsigned char OP_Code, unsigned int Data1, unsigned int Data2, unsigned int Data3, unsigned int Data4) {
// Post: Envia una trama a l'adreça desti amb OpCode, Data1, Data2, Data3 i Data4  com les 9 (COM_FRAME_SIZE) dades de la trama
// Es una rutina d'us intern, que només fa servir el tad protocol
	// Carrego la trama a enviar
	int i, Index;
	Index = 0;
	txBuffer1[Index++] = LengthDATA+1;			// 0
	txBuffer1[Index++] = NET_ADDRESS_Device;    // 1, Addreça de xarxa   
	// Adreça Destí
	for (i = 0; i < 	RF_LEN_ADDRESS; i++) {
		txBuffer1[Index++] = Desti[i];  
	}
	// Adreça local
	for (i = 0; i < 	RF_LEN_ADDRESS; i++) {
		txBuffer1[Index++] = LocalAddress[i];  
	}
	txBuffer1[Index++] = OP_Code;
	txBuffer1[Index++] = Data1;		
	txBuffer1[Index++] = Data1>>8;		
	txBuffer1[Index++] = Data2;		
	txBuffer1[Index++] = Data2>>8;		
	txBuffer1[Index++] = Data3;		
	txBuffer1[Index++] = Data3>>8;		
	txBuffer1[Index++] = Data4;		
	txBuffer1[Index++] = Data4>>8;	
	TransmetTrama();
#ifndef LSMAKER_BASE
	PtTramaEnviada(); // Només les trames privades porten control de retransmissió
#endif
}

void RfReSendTrama(void) {
// Post: Retransmet la última trama
	TransmetTrama();
}

void RfSendFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]) {
// Post: Envia les dades de Data a l'adreça destí. 
	int i, Index;
	Index = 0;
	txBuffer1[Index++] = LengthDATA+1;			// 0
	txBuffer1[Index++] = NET_ADDRESS_Device;    // 1, Addreça de xarxa   
	// Adreça Destí
	for (i = 0; i < 	RF_LEN_ADDRESS; i++) {
		txBuffer1[Index++] = Destination[i];  
	}
	// Adreça local
	for (i = 0; i < 	RF_LEN_ADDRESS; i++) {
		txBuffer1[Index++] = LocalAddress[i];  
	}
	// Les dades
	for (i = 0; i < RF_FRAME_SIZE; i++) {
		txBuffer1[Index++] = Data[i];  
	}
	TransmetTrama();
}

void RfSendBroadCastFrame(unsigned char Data[COM_FRAME_SIZE]) {
// Post: munta una trama amb l'adreça propia del robot i les dades.
// i l'envia per broadcast. No es garanteix que cap destí la rebi.
	RfSendFrame(BroadAddress, Data);
}
//
//------------------------END-----PUBLIQUES---AREA-----------
//
//
//--------------------------------PRIVADES----AREA-----------
//


void WriteRFSettings(void){
// Configuro els registres del Transceiver, rutina copiada de la nota d'aplicació de
// texas instruments
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   SMARTRF_SETTING_IOCFG2);  // GDO2 output pin config. 			SMARTRF_SETTING_IOCFG2 = 0x29
    													// GDO2is CHIP_RDYn

    TI_CC_SPIWriteReg(TI_CCxxx0_PKTLEN,   SMARTRF_SETTING_PKTLEN);  // Packet length. 					SMARTRF_SETTING_PKTLEN = 0xFF
   														// Indicates the packet length when fixed packet length is enabled.
   														// If variable length packets are used, this value indicates the maximum length packets allowed.
 
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL1, SMARTRF_SETTING_PKTCTRL1);  // Packet automation control. 	SMARTRF_SETTING_PKTCTRL1 = 0x07
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7:5 		PQT[2:0] 			0 (000) 		R/W P			reamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is
//																received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit.
//
//																A threshold of 4-PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
//
// 4 		Reserved 			0 				R0
//
// 3 		CRC_AUTOFLUSH 		0 				R/W 			Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RX FIFO and that packet length is limited to the RX FIFO size.
//
// 																PKTCTRL0.CC2400_EN must be 0 (default) for the CRC autoflush function to work correctly.
//
// 2 		APPEND_STATUS 		1 				R/W 			When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as the CRC OK flag.
//
// 1:0 		ADR_CHK[1:0] 		0 (00) 			R/W 			Controls address check configuration of received packages.
// 																	Setting 	Address check configuration
// 																	0 (00)	 		No address check
// 																	1 (01) 			Address check, no broadcast
// 																	2 (10) 			Address check and 0 (0x00) broadcast
// 																	3 (11) 			Address check and 0 (0x00) and 255 (0xFF) broadcast				

	TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL0, SMARTRF_SETTING_PKTCTRL0);  // Packet automation control. 	SMARTRF_SETTING_PKTCTRL0 = 0x05
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7 		Reserved 							R0
//
// 6 		WHITE_DATA 			1 				R/W 			Turn data whitening on / off
//																	0: Whitening off
//																	1: Whitening on
//
//																Data whitening can only be used when PKTCTRL0.CC2400_EN=0 (default).
//
// 5:4 		PKT_FORMAT[1:0] 	0 				(00) 			R/W Format of RX and TX data
//																	Setting 	Packet format
//																	0 (00) 		Normal mode, use FIFOs for RX and TX
//																	1 (01) 		Synchronous serial mode, used for backwards compatibility. Data in on GDO0
//																	2 (10)		Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX.
//																	3 (11) 		Asynchronous serial mode. Data in on GDO0 and data out on either of the GDO0 pins
//
// 3 		CC2400_EN 			0 				R/W 			Enable CC2400 support. Use same CRC implementation as CC2400.
//
//																PKTCTRL1.CRC_AUTOFLUSH must be 0 if PKTCTRL0.CC2400_EN=1.
//
//																PKTCTRL0.WHITE_DATA must be 0 if PKTCTRL0.CC2400_EN=1.
//
// 2 		CRC_EN 				1 				R/W 			1: CRC calculation in TX and CRC check in RX enabled
//																0: CRC disabled for TX and RX
//
// 1:0 		LENGTH_CONFIG[1:0] 	1 (01) 			R/W 			Configure the packet length
//																	Setting 	Packet length configuration
//																	0 (00) 			Fixed packet length mode. Length configured in PKTLEN register
//																	1 (01) 			Variable packet length mode. Packet length configured by the first byte after sync word
//																	2 (10) 			Infinite packet length mode
//																	3 (11) 			Reserved

    TI_CC_SPIWriteReg(TI_CCxxx0_ADDR,     SMARTRF_SETTING_ADDR);  // Device address.					SMARTRF_SETTING_ADDR = 0x00
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7:0 		DEVICE_ADDR[7:0] 	0 (0x00) 		R/W 			Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).

//    TI_CC_SPIWriteReg(TI_CCxxx0_CHANNR,   SMARTRF_SETTING_CHANNR); // Channel number.
    TI_CC_SPIWriteReg(TI_CCxxx0_CHANNR,   Canal); // El numero de canal
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7:0 		CHAN[7:0] 			0 (0x00) 		R/W 			The 8-bit unsigned channel number, which is multiplied by the channel spacing setting and added to the base frequency.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL1,  SMARTRF_SETTING_FSCTRL1); // Freq synthesizer control.		SMARTRF_SETTING_FSCTRL1 = 0x0A
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7:5 		Reserved 							R0
// 4:0 		FREQ_IF[4:0] 		15 (0x0F) 		R/W 			The desired IF frequency to employ in RX. Subtracted from FS base frequency in RX and controls the digital complex mixer in the demodulator.
//																	f(if) = (F(xosc)/2^10)*Freq_if
//																The default value gives an IF frequency of 381 kHz, assuming a 26.0 MHz crystal.
//

    TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL0,  SMARTRF_SETTING_FSCTRL0); // Freq synthesizer control. 		SMARTRF_SETTING_FSCTRL0 = 0x00
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7:0 		FREQOFF[7:0] 		0 (0x00) 		R/W 			Frequency offset added to the base frequency before being used by the FS. (2’s complement). 
//																Resolution is FXTAL/214 (1.59 - 1.65 kHz); range is ±202 kHz to ±210 kHz, dependent of XTAL frequency.

    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ2,    SMARTRF_SETTING_FREQ2); // Freq control word, high byte		SMARTRF_SETTING_FREQ2 = 0x5A
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7:6 		FREQ[23:22]		 	1 (01) 			R 				FREQ[23:22] is always binary 01 (the FREQ2 register is in the range 85 to 95 with 26-27 MHz crystal)
//
// 5:0 		FREQ[21:16] 		30 (0x1E)		R/W 			FREQ[23:0] is the base frequency for the frequency synthesiser in increments of Fxosc/2^16.
//																	Fcarrier = Fxosc/2^16*Freq[23:0]

    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ1,    SMARTRF_SETTING_FREQ1); // Freq control word, mid byte.		SMARTRF_SETTING_FREQ1 = 0x1C
//
// Bit 		Field Name 			Reset 			R/W 			Description
//  
// 7:0 		FREQ[15:8] 			196 (0xC4) 		R/W 			Ref. FREQ2 register

    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ0,    SMARTRF_SETTING_FREQ0); // Freq control word, low byte.		SMARTRF_SETTING_FREQ0 = 0x71
//
// Bit 		Field Name 			Reset 			R/W 			Description
//     
// 7:0 		FREQ[7:0] 			236 (0xEC) 		R/W 			Ref. FREQ2 register
    
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  SMARTRF_SETTING_MDMCFG4); // Modem configuration.				SMARTRF_SETTING_MDMCFG4 = 0x2D 
//
// Bit 		Field Name 			Reset 			R/W 			Description
//  
// 7:6 		CHANBW_E[1:0] 		2 (10) 			R/W
// 5:4 		CHANBW_M[1:0] 		0 (00) 			R/W 			Sets the decimation ratio for the delta-sigma ADC input stream and thus the channel bandwidth.
//																		BWchannel=  (Fxosc)/(8(4+CHANBW_M)2^(CHANBW E))
//																The default values give 203 kHz channel filter bandwidth, assuming a 26.0 MHz crystal.
// 3:0 		DRATE_E[3:0] 		12 (1100) 		R/W 			The exponent of the user specified symbol rate

    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  SMARTRF_SETTING_MDMCFG3); // Modem configuration.				SMARTRF_SETTING_MDMCFG3 = 0x2F
//
// Bit 		Field Name 			Reset 			R/W 			Description
// 
// 7:0 		DRATE_M[7:0] 		34 (0x22) 		R/W 			The mantissa of the user specified symbol rate. The symbol rate is configured using an unsigned, floating-point number
//																with 9-bit mantissa and 4-bit exponent. The 9th bit is a hidden ‘1’. The resulting data rate is:
//																		Rdata = ((256+Drate_M)2^(Drate_E))/(2^(28))*Fxosc
//																The default values give a data rate of 115.051 kBaud (closest setting to 115.2 kBaud), assuming a 26.0 MHz crystal.

    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG2,  SMARTRF_SETTING_MDMCFG2); // Modem configuration.				SMARTRF_SETTING_MDMCFG2 = 0x72
//
// Bit 		Field Name 			Reset 			R/W 			Description
//  
// 7 		DEM_DCFILT_OFF 		0 				R/W 			Disable digital DC blocking filter before demodulator.
//																	0 = Enable (better sensitivity)
//																	1 = Disable (current optimized). Only for data rates < 250 kBaud
//																The recommended IF frequency changes when the DC blocking is disabled. Please use SmartRF Studio [5] to calculate correct register setting.
//
// 6:4 		MOD_FORMAT[2:0] 	0 (000) 		R/W 			The modulation format of the radio signal 
//																	Setting 		Modulation format
//																	0 (000) 			2-FSK
//																	1 (001) 			GFSK
//																	2 (010) 			-
//																	3 (011) 			OOK
//																	4 (100) 			-
//																	5 (101) 			-
//																	6 (110) 			-
//																	7 (111) 			MSK
//
// 3 		MANCHESTER_EN 		0 				R/W 			Enables Manchester encoding/decoding.
//																	0 = Disable
//																	1 = Enable
//
// 2:0 		SYNC_MODE[2:0] 		2 (010) 		R/W 			Combined sync-word qualifier mode.
//																The values 0 (000) and 4 (100) disables preamble and sync word transmission in TX and preamble and sync word detection in RX.
//																The values 1 (001), 2 (010), 5 (101) and 6 (110) enables 16-bit sync word transmission in TX and 16- bits sync word detection in RX. 
// 																Only 15 of 16 bits need to match in RX when using setting 1 (001) or 5 (101).
//
// 																The values 3 (011) and 7 (111) enables repeated sync word transmission in TX and 32-bits sync word detection in RX (only 30 of 32 bits need to match).
//																	Setting 		Sync-word qualifier mode
//																	0 (000) 			No preamble/sync
//																	1 (001) 			15/16 sync word bits detected
//																	2 (010) 			16/16 sync word bits detected
//																	3 (011) 			30/32 sync word bits detected
//																	4 (100) 			No preamble/sync, carrier-sense above threshold
//																	5 (101) 			15/16 + carrier-sense above threshold
//																	6 (110) 			16/16 + carrier-sense above threshold
//																	7 (111) 			30/32 + carrier-sense above threshold

    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG1,  SMARTRF_SETTING_MDMCFG1); // Modem configuration.				SMARTRF_SETTING_MDMCFG1 = 0x22
//
// Bit 		Field Name 			Reset 			R/W 			Description
//
// 7 		FEC_EN 				0 				R/W 			Enable Forward Error Correction (FEC) with interleaving for packet payload
//																	0 = Disable
//																	1 = Enable (Only supported for fixed packet length mode, i.e. PKTCTRL0.LENGTH_CONFIG=0)
//
// 6:4 		NUM_PREAMBLE[2:0] 	2 (010) 		R/W 			Sets the minimum number of preamble bytes to be transmitted
//																	Setting 		Number of preamble bytes
//																	0 (000)				2
//																	1 (001) 			3
//																	2 (010) 			4
//																	3 (011) 			6
//																	4 (100) 			8
//																	5 (101) 			12
//																	6 (110) 			16
//																	7 (111) 			24
//
// 3:2 		Reserved 							R0
// 1:0 		CHANSPC_E[1:0] 		2 (10) 			R/W 			2 bit exponent of channel spacing

    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG0,  SMARTRF_SETTING_MDMCFG0); // Modem configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_DEVIATN,  SMARTRF_SETTING_DEVIATN); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM0 ,   SMARTRF_SETTING_MCSM0); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_CCxxx0_FOCCFG,   SMARTRF_SETTING_FOCCFG); // Freq Offset Compens. Config
    TI_CC_SPIWriteReg(TI_CCxxx0_BSCFG,    SMARTRF_SETTING_BSCFG); //  Bit synchronization config.
    TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL2, SMARTRF_SETTING_AGCCTRL2); // AGC control.
    TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL1, SMARTRF_SETTING_AGCCTRL1); // AGC control.
    TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL0, SMARTRF_SETTING_AGCCTRL0); // AGC control.
    TI_CC_SPIWriteReg(TI_CCxxx0_FREND1,   SMARTRF_SETTING_FREND1); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_FREND0,   SMARTRF_SETTING_FREND0); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL3,   SMARTRF_SETTING_FSCAL3); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL2,   SMARTRF_SETTING_FSCAL2); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL1,   SMARTRF_SETTING_FSCAL1); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL0,   SMARTRF_SETTING_FSCAL0); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSTEST,   SMARTRF_SETTING_FSTEST); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST2,    SMARTRF_SETTING_TEST2); // Various test settings.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST1,    SMARTRF_SETTING_TEST1); // Various test settings.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST0,    SMARTRF_SETTING_TEST0);  // Various test settings.
    
   // Registres que toco manualment 
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   0x0B);  // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG0,   0x06);  // GDO0 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTLEN,   0xFF);  // Packet length.
//    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL1, 0x06); // 0x05);  // Packet automation control.
                                                // Addres Broadcast 0x00, 0xFF enable,  APPEND_STATUS
// V62
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL1, 0xE);  // Packet automation control. 	Activo el CRC autoflusss
// End V62  
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL0, 0x05);// Packet automation control.
                                                // Variable Pk length , CRC ON
    
    TI_CC_SPIWriteReg(TI_CCxxx0_ADDR,     NET_ADDRESS_Device);  // Device address.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM1 ,   0x3e); //MainRadio Cntrl State Machine
                                                 // TX_OFF--> Seguim en TX
    
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
                                                 // OOOOJJJJOOOO  PODEM REDUIR EL CONSUM 
    
    
    TI_CC_SPIWriteReg(TI_CCxxx0_SYNC1 ,   NET_ADDRESS_Sinc);// Defineixo la adreça de xarxa
    TI_CC_SPIWriteReg(TI_CCxxx0_SYNC0 ,   NET_ADDRESS_Sinc);
    
    
}

void ModeRecepcio(void) {
   	TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    TI_CC_SPIStrobe(TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mode.
    IoWait(900); 
	RadioMode=RX_Mode;
}

#define TX_MODE 0x13

void ModeTransmissio(void) { 
  	TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    TI_CC_SPIStrobe(TI_CCxxx0_STX);           // Initialize CCxxxx in TX mode.
    IoWait(900); 
	// Comprovo que hagi entrat en mode de transmissió, ja que tenim el CCA activat
	RadioMode=TX_Mode;
}

void TransmetTrama(void) {
#ifndef LSMAKER_BASE
	EngegaLed();
#endif
	ModeTransmissio();
	TI_CC_SPIWriteBurstReg(TI_CCxxx0_TXFIFO, txBuffer1, LengthDATA+2); // Write TX data
//	ModeTransmissio();
	IoWait(1000);	// Per assegurar que hem enviat la trama (falta comprovar la durada de la trama)
	ModeRecepcio();	// I tornem al mode recepció.
#ifndef LSMAKER_BASE
//	PtTramaEnviada(); Només les trames privades porten control de retransmissió
	ApagaLed();
#endif

}

void RadioInit(void){ 
	TI_CC_SPISetup();      		// Initialize SPI port del PIC
 	TI_CC_PowerupResetCCxxxx(); // Reset CCxxxx
    IoWait(200);
  	WriteRFSettings();                        // Write RF settings to config reg
  	TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen);//Write PATABLE
	ModeRecepcio(); 
	// Capturo l'adreça local i l'adreça del control remot des de l'eeprom.
	EEGetShortMAC(LocalAddress);
#ifdef _DEPURANT_RADIO
	RemoteAddress[0] = AdressaRemotaFalsa[0];
	RemoteAddress[1] = AdressaRemotaFalsa[1];
	RemoteAddress[2] = AdressaRemotaFalsa[2];
#else
	EERead(EE_MAC_REMOT,RemoteAddress, 	RF_LEN_ADDRESS);
	// Versió 0.2
	if ((RemoteAddress[0] == 0xFF) && (RemoteAddress[1] == 0xff)) ModeLite = 1;
	else ModeLite = 0;
#endif
}


//----------------------------------------------------------------------+++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char TramaPerMi(unsigned char *RXFrametmp){
	unsigned char a0, a1, a2;
	a0 = RXFrametmp[0];
	a1 = RXFrametmp[1];
	a2 = RXFrametmp[2];
	if (a0 != LocalAddress[0]) return 0;
	if (a1 != LocalAddress[1]) return 0;
	if (a2 != LocalAddress[2]) return 0;
  	return 1;
}

unsigned char EsLliure(unsigned char *RXFrametmp){ return RXFrametmp[6] == 0xFF; }

unsigned char EsElMeuRemot(unsigned char *RXFrametmp){
	unsigned char a0, a1, a2;
	if (ModeLite) {
		// Si estem en mode lite (V0.2) cal mirar si l'adreça destí
		// val 0xff, 0xff (en els primers dos bytes i comparar el tercer byte
		// amb el tercer byte de l'adreça remota
		a0 = RXFrametmp[0];
		a1 = RXFrametmp[1];
		a2 = RXFrametmp[2];
	} else {
		// En mode normal, comprovem que el qui envia és el nostre remot
		a0 = RXFrametmp[3];
		a1 = RXFrametmp[4];
		a2 = RXFrametmp[5];
	}
	if (a0 != RemoteAddress[0]) return 0;
	if (a1 != RemoteAddress[1]) return 0;
	if (a2 != RemoteAddress[2]) return 0;
  	return 1;
}

unsigned char EsBroadCast(unsigned char *RXFrametmp){
	unsigned char a0, a1, a2;
	a0 = RXFrametmp[0];
	a1 = RXFrametmp[1];
	a2 = RXFrametmp[2];
	if (a0 != BroadAddress[0]) return 0;
	if (a1 != BroadAddress[1]) return 0;
	if (a2 != BroadAddress[2]) return 0;
  	return 1;
}

//-----------------------------------------------------------------------------
char RFReceivePacket(char *rxBuffer, char *length)
{
  char pktLen;
 	if ((TI_CC_SPIReadStatus(TI_CCxxx0_RXBYTES) & TI_CCxxx0_NUM_RXBYTES)) {
    	pktLen = TI_CC_SPIReadReg(TI_CCxxx0_RXFIFO); // Quantes dades hi ha?
#ifdef LSMAKER_BASE
//			LS_USB_printf("\r\nLenR %d LenE %d\r\n", pktLen, *length);
#endif
	    if ((pktLen <= *length) && (pktLen > 0)) {   // Suficients=
    		TI_CC_SPIReadBurstReg(TI_CCxxx0_RXFIFO, rxBuffer, pktLen+2); // Llegeix les dades + 2 d'estat
      		*length = pktLen;                     // Returna el tamany real
 			RssiAct = rxBuffer[LengthDATA];	// El primer byte indica el RSSI
			LQIAct  = rxBuffer[LengthDATA+TI_CCxxx0_LQI_RX] & 0x7f;	// I el segon l'indicador de qualitat de l'enllaç		
			ModeRecepcio();
			return (char)(rxBuffer[LengthDATA+TI_CCxxx0_LQI_RX]&TI_CCxxx0_CRC_OK); // Return CRC_OK bit

    	} else {
			*length = pktLen;                   // Return the large size
      		TI_CC_SPIStrobe(TI_CCxxx0_SFRX);   	// Flush RXFIFO
			IoWait(2);
			ModeRecepcio();
      		return 0;                           // Error
    	}
	} else {
		ModeRecepcio();
      	return 0;                             	// Error
  	}
	return 0;
}

int AjustadB(unsigned char Rssi) {
	int RssiOff  = 72; // Pagina 35 del manual del CC2500
	int RssiSigned = Rssi;
	if (Rssi > 128) return (RssiSigned -256)/2-RssiOff;
	return RssiSigned /2 - RssiOff;
}

void ProcessaTrama(unsigned char * RXFrame){
	int i;
 	struct Trama T; 
//	char tmp[16];
//	for (i = 0; i < 16; i++) {
//		sprintf(tmp, "%x ", RXFrame[i]); SiPuts(tmp);
//	}
//	SiPuts("\n\r");
	if (EsLliure(RXFrame)) {	// Trama d'usuari.
		if (TramaPerMi(RXFrame) || Promis || EsBroadCast(RXFrame)) {
			// Ens interessa com a trama d'usuari
			memcpy(BufferU[IniciU].Desti, 	&RXFrame[0],						COM_SHORT_MAC_SIZE );
			memcpy(BufferU[IniciU].Origen, 	&RXFrame[0+COM_SHORT_MAC_SIZE], 	COM_SHORT_MAC_SIZE);
			memcpy(BufferU[IniciU].Dades, 	&RXFrame[0+2*COM_SHORT_MAC_SIZE], 	COM_FRAME_SIZE);
			BufferU[IniciU].Rssi = AjustadB(RssiAct); 
			if (QuantsU < MAX_CUA_USUARIS) {
				IniciU++;	QuantsU++;
				if (IniciU == MAX_CUA_USUARIS) IniciU = 0;
			}
		}
	} else {	// Trama de control remot
		if (EsElMeuRemot(RXFrame)) {
#ifdef LSMAKER_BASE
//			LS_USB_printf("\r\nRssi %d LQI %x\r\n", AjustadB(RssiAct), LQIAct);
#endif
			// Trama de telecontrol del nostre remot
			// Omplo l'estructura i la poso a la cua
			memcpy(T.Origen, 	&RXFrame[0+COM_SHORT_MAC_SIZE], 	COM_SHORT_MAC_SIZE);
			i = 6;
			T.OpCode = RXFrame[i++];
			T.Data1  = RXFrame[i++]; 
			T.Data1  = T.Data1 | (RXFrame[i++]<<8); 
			T.Data2  = RXFrame[i++]; 
			T.Data2  = T.Data2 | (RXFrame[i++]<<8); 
			T.Data3  = RXFrame[i++]; 
			T.Data3  = T.Data3 | (RXFrame[i++]<<8); 
			T.Data4  = RXFrame[i++]; 
			T.Data4  = T.Data4 | (RXFrame[i++]<<8); 
			if (Quants < MAX_TRAMES) Posa(T); // Else, que te den.
		}
#ifndef LSMAKER_BASE
		if (TramaPerMi(RXFrame)) {
			memcpy(T.Origen, 	&RXFrame[0+COM_SHORT_MAC_SIZE], 	COM_SHORT_MAC_SIZE);
			i = 6;
			T.OpCode = RXFrame[i++];
			T.Data1  = RXFrame[i++]; 
			T.Data1  = T.Data1 | (RXFrame[i++]<<8); 
			T.Data2  = RXFrame[i++]; 
			T.Data2  = T.Data2 | (RXFrame[i++]<<8); 
			T.Data3  = RXFrame[i++]; 
			T.Data3  = T.Data3 | (RXFrame[i++]<<8); 
			T.Data4  = RXFrame[i++]; 
			T.Data4  = T.Data4 | (RXFrame[i++]<<8); 
			if (Quants < MAX_TRAMES) Posa(T); // Else, que te den.
		}
#endif
	}	
}
//----------------------------------------------------------------------+++++++++++++++++++++++++++++++++++++++++++++++++++


void Posa(struct Trama c) {
	Trames[Inici++] = c;
	if (Inici == MAX_TRAMES) Inici = 0;
	Quants++;
}

struct Trama Treu(void) {
	struct Trama tmp; 
	tmp = Trames[Fi++] ;
	if (Fi == MAX_TRAMES) Fi = 0;
	Quants --;
	return tmp;
}


static int TramaRebuda = 0;
void MiraTrama(void) {
	char len;
	if (TramaRebuda) {
		TramaRebuda = 0;
    	len=LengthDATA+1;                 				// Longitud de les dades mçes l'adreça de xarxa
		if(RadioMode == RX_Mode){
			if (RFReceivePacket(rxBuffer1,&len))  {     // Fetch packet from CCxxxx
				ProcessaTrama((unsigned char *)rxBuffer1);
			}	
		}	
	}
}

void __attribute__((interrupt, shadow, no_auto_psv)) _INT0Interrupt(void){ 
	if(IFS0bits.INT0IF == 1){
		TramaRebuda = 1;
		IFS0bits.INT0IF = 0;		// CLR flag		
  	}  	
}

//-----------------------------------------------------------------------------------------
// Rutines relacionades amb l'SPI i el CC2500
//-----------------------------------------------------------------------------------------
   
#ifdef LSMAKER_BASE

char CC2500_tmp;

#define CC2500_WaitSPIReady				while(SPI2STATbits.SPIRBF==0);
#define CC2500_Reset_SPIRBF				CC2500_tmp=SPI2BUF;
#define CC2500_WaitReady				while(_RG7==1) { Nop(); Nop(); Nop(); }
#define CS_Actiu_CC2500					_LATF1 = 0
#define CS_Inactiu_CC2500				_LATF1 = 1


void CCInici(char addr) {
  	CS_Actiu_CC2500;       	// Baixo el CS
	CC2500_WaitReady;		// Espero que el 2500 estigui a punt
  	CC2500_Reset_SPIRBF;	// Esborro el flag de lectura
	SPI2BUF = addr;        	// Envio adreça
  	CC2500_WaitSPIReady;	// Espero que SPI acabi
}

void CC2500_Send_DUMMYDATA(void) {
  	CC2500_Reset_SPIRBF;	// Esborro el flag de lectura
	SPI2BUF=0x00;			// envio un byte qualsevol
  	CC2500_WaitSPIReady;	// i Espero que SPI acabi
}

void TI_CC_SPISetup(void) {

	CS_Inactiu_CC2500;
	// Configurem el SPI a 500KHz
//  	SPI2CON1= 0b0000000001100011;			// Aquest era l'original
  	SPI2CON1= 0b0000000100100011;			// Inverteixo clock
  	SPI2CON2= 0x0000;
  	SPI2STAT= 0b0010000000010100;	 			//0x2014;
  	SPI2STATbits.SPIEN = 1;					// SPI2 ENABLE
}

void TI_CC_SPIWriteReg(char addr, char value)
{
	CCInici(addr);
  	CC2500_Reset_SPIRBF;		// Esborro el flag de lectura
	SPI2BUF = value;  			// Escric el valor
  	CC2500_WaitSPIReady;		// Espero que SPI acabi
	CS_Inactiu_CC2500;      	// Pujo el CS
}

void TI_CC_SPIWriteBurstReg(char addr,  char *buffer, char count) {
  	int i;
	CCInici(addr | TI_CCxxx0_WRITE_BURST);
	for (i = 0; i < count; i++) {
	  	CC2500_Reset_SPIRBF;	// Esborro el flag de lectura
	   	SPI2BUF = buffer[i];    // Send data
		CC2500_WaitSPIReady;	// Espero que SPI acabi
	}
	CS_Inactiu_CC2500;      	// Pujo el CS
}

char TI_CC_SPIReadReg(char addr) {
	char x;
	CCInici(addr | TI_CCxxx0_READ_SINGLE); 	
	CC2500_Send_DUMMYDATA();	// Write tonto                             
	x = SPI2BUF;            	// Llegeixo i guardo
	CS_Inactiu_CC2500;      	// Pujo el CS
	return x;
}

void TI_CC_SPIReadBurstReg(char addr, char *buffer, char count) {
	unsigned int i;
	CCInici(addr | TI_CCxxx0_READ_BURST);  	
	CC2500_Send_DUMMYDATA();	// Write tonto                             	
 	CC2500_Reset_SPIRBF;
 	// El primer byte ja el tenim
	for (i = 0; i <	 (count-1); i++) {
		SPI2BUF=0x00;			// envio un byte qualsevol
	  	CC2500_WaitSPIReady;	// i Espero que SPI acabi
		buffer[i] = SPI2BUF; 
  	}
  	buffer[count-1] = SPI2BUF;	// Falta l'ultim
	CS_Inactiu_CC2500;      	// Pujo el CS
}

char TI_CC_SPIReadStatus(char addr) {
	char status;
  	CCInici(addr | TI_CCxxx0_READ_BURST);  
	CC2500_Send_DUMMYDATA();	// Write tonto                             
	status = SPI2BUF;   
	CS_Inactiu_CC2500;      	// Pujo el CS
	return status;
}

void TI_CC_SPIStrobe(char strobe) { 
	CCInici(strobe);           	// Directament
	Estat2500 = SPI2BUF;
	CS_Inactiu_CC2500;      	// Pujo el CS
}

void TI_CC_PowerupResetCCxxxx(void)  {
  	CS_Actiu_CC2500;       		// Baixo el CS
	IoWait(40);
  	CS_Actiu_CC2500;
  	IoWait(40);
  	CS_Inactiu_CC2500;
  	IoWait(40);
	CS_Actiu_CC2500; 
  	IoWait(200);
	CC2500_WaitReady;
 	CC2500_Reset_SPIRBF;		// Wait for TX to finish
  	SPI2BUF = TI_CCxxx0_SRES;   // Send strobe
  	// Strobe addr is now being TX'ed
  	CC2500_WaitSPIReady;		// Espero que SPI acabi
  	CC2500_Reset_SPIRBF;		// Esborro el flag de lectura
	CS_Inactiu_CC2500;      	// Pujo el CS
}

#else 	// No és un LSMAKER_BASE

char CC2500_tmp;

#define CC2500_WaitSPIReady				while(SPI1STATbits.SPIRBF==0);
#define CC2500_Reset_SPIRBF				CC2500_tmp=SPI1BUF;
#define CC2500_WaitReady				while(_RB12==1) { Nop(); Nop(); Nop(); }
#define CS_Actiu_CC2500					_LATB10 = 0
#define CS_Inactiu_CC2500				_LATB10 = 1


void CCInici(char addr) {
  	CS_Actiu_CC2500;       	// Baixo el CS
	CC2500_WaitReady;		// Espero que el 2500 estigui a punt
  	CC2500_Reset_SPIRBF;	// Esborro el flag de lectura
	SPI1BUF = addr;        	// Envio adreça
  	CC2500_WaitSPIReady;	// Espero que SPI acabi
}

void CC2500_Send_DUMMYDATA(void) {
  	CC2500_Reset_SPIRBF;	// Esborro el flag de lectura
	SPI1BUF=0x00;			// envio un byte qualsevol
  	CC2500_WaitSPIReady;	// i Espero que SPI acabi
}

void TI_CC_SPISetup(void) {

	CS_Inactiu_CC2500;
	// Configurem el SPI a 500KHz
//  	SPI2CON1= 0b0000000001100011;			// Aquest era l'original
	SPI1CON1= 0b0000000001100011;
  	SPI1CON2= 0x0000;
 	SPI1STAT= 0b0010000000010100;	 			//0x2014;
 	SPI1STATbits.SPIEN = 1;					// SPI2 ENABLE
}

void TI_CC_SPIWriteReg(char addr, char value)
{
	CCInici(addr);
  	CC2500_Reset_SPIRBF;		// Esborro el flag de lectura
	SPI1BUF = value;  			// Escric el valor
  	CC2500_WaitSPIReady;		// Espero que SPI acabi
	CS_Inactiu_CC2500;      	// Pujo el CS
}

void TI_CC_SPIWriteBurstReg(char addr,  char *buffer, char count) {
  	int i;
	CCInici(addr | TI_CCxxx0_WRITE_BURST);
	for (i = 0; i < count; i++) {
	  	CC2500_Reset_SPIRBF;	// Esborro el flag de lectura
	   	SPI1BUF = (unsigned char)buffer[i];    // Send data
		CC2500_WaitSPIReady;	// Espero que SPI acabi
	}
	CS_Inactiu_CC2500;      	// Pujo el CS
}

char TI_CC_SPIReadReg(char addr) {
	char x;
	CCInici(addr | TI_CCxxx0_READ_SINGLE); 	
	CC2500_Send_DUMMYDATA();	// Write tonto                             
	x = SPI1BUF;            	// Llegeixo i guardo
	CS_Inactiu_CC2500;      	// Pujo el CS
	return x;
}

void TI_CC_SPIReadBurstReg(char addr, char *buffer, char count) {
	unsigned int i;
	CCInici(addr | TI_CCxxx0_READ_BURST);  	
	CC2500_Send_DUMMYDATA();	// Write tonto                             	
 	CC2500_Reset_SPIRBF;
 	// El primer byte ja el tenim
	for (i = 0; i <	 (count-1); i++) {
		SPI1BUF=0x00;			// envio un byte qualsevol
	  	CC2500_WaitSPIReady;	// i Espero que SPI acabi
		buffer[i] = SPI1BUF; 
  	}
  	buffer[count-1] = SPI1BUF;	// Falta l'ultim
	CS_Inactiu_CC2500;      	// Pujo el CS
}

char TI_CC_SPIReadStatus(char addr) {
	char status;
  	CCInici(addr | TI_CCxxx0_READ_BURST);  
	CC2500_Send_DUMMYDATA();	// Write tonto                             
	status = SPI1BUF;   
	CS_Inactiu_CC2500;      	// Pujo el CS
	return status;
}

void TI_CC_SPIStrobe(char strobe) { 
	CCInici(strobe);           	// Directament
	Estat2500 = SPI1BUF;
	CS_Inactiu_CC2500;      	// Pujo el CS
}

void TI_CC_PowerupResetCCxxxx(void)  {
  	CS_Actiu_CC2500;       		// Baixo el CS
	IoWait(40);
  	CS_Actiu_CC2500;
  	IoWait(40);
  	CS_Inactiu_CC2500;
  	IoWait(40);
	CS_Actiu_CC2500; 
  	IoWait(200);
	CC2500_WaitReady;
 	CC2500_Reset_SPIRBF;		// Wait for TX to finish
  	SPI1BUF = TI_CCxxx0_SRES;   // Send strobe
  	// Strobe addr is now being TX'ed
  	CC2500_WaitSPIReady;		// Espero que SPI acabi
  	CC2500_Reset_SPIRBF;		// Esborro el flag de lectura
	CS_Inactiu_CC2500;      	// Pujo el CS
}

#endif	// Control remot


















