//-----------------------------------------------------------------
//TAD         :Protocol de comunicacions de RF
//Autor       :FEC
//Descripció  :Rep i executa les trames entrants, NO inicialitza res
//Prefix      :Pt
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//
//----------------------------------------PROTOTIPUS---------------
//
#include "../LSApi.h"

//
//------------------------------END-------PROTOTIPUS---------------
//
void EnviaAck(void);
void ExecutaTrama(void);
void ProcessaTramaLliure(void);
void WatchDogCC2500(void);

//
//----------------------------------------CONSTANTS----------------
//
#define T_RESPOSTA		300	// En ms, temps que triguem a enviar l'ACK
//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------VARIABLES----------------
//
static int T;
static struct Trama LaTrama;
//static unsigned char Origen[RF_LEN_ADDRESS];
//static unsigned char Desti[RF_LEN_ADDRESS];
//static unsigned char Dades[RF_FRAME_SIZE];
static int EstemAturats;
//static int Rssi;
//
//------------------------------END-------VARIABLES----------------
//


//
//----------------------------------------PÚBLIQUES----------------
//
void PtInit(void) {
	T = TiGetTimer();
	EstemAturats = 0;
}

void PtMotor(void) {
	static int Estat = 0;
	WatchDogCC2500();
	switch (Estat) {
		case 0:
			if (RfHiHaTrama()) {
				LaTrama = RfGetTrama();
				ExecutaTrama(); EnviaAck(); TiResetTics(T); IoSetLedBeat(20);
				++Estat;
			} 
			break;
		case 1:
			if (TiGetTics(T) >= 1200) {
				// Un segon sense cap trama, paro motors
				MtSetPWM(M_ESQUERRA, 0, 0); MtSetPWM(M_DRET, 0, 0); MtSetPWM(M_2, 0, 0); MtSetPWM(M_3, 0, 0); 
				NvSetDireccio(0, 0, 0, 0);
				// I faig un reset al transceptor
				// if (TiGetTics(T) > 3000) {
				//	RfEnd(); RfInit(); // Quin peligro!
					TiResetTics(T);
					IoSetLedBeat(5);
				//}
				Estat = 0;
			}
			if (RfHiHaTrama()) {
				LaTrama = RfGetTrama();
				ExecutaTrama(); EnviaAck(); TiResetTics(T); IoSetLedBeat(20); 
			}
			break;
		case 2:
			if (TiGetTics(T) >= T_RESPOSTA) {
				EnviaAck();
				--Estat; 
			}
			break;

	}
}

//
//------------------------------END-------PRIVADES-----------------
//

void EnviaAck(void) {
	RfSendTrama((unsigned char *)LaTrama.Origen, P_ACK, 0, 0, 0, 0);
}

#include <stdio.h>


void ExecutaTrama(void) { 
	int Data1, Data2, Data3, Data4;
	Data1 = LaTrama.Data1;
	Data2 = LaTrama.Data2;
	Data3 = LaTrama.Data3;
	Data4 = LaTrama.Data4;
	LS_USB_printf("OP=%d D1=%d D2=%d D3=%d D4=%d        \r\n", LaTrama.OpCode, LaTrama.Data1, LaTrama.Data2, LaTrama.Data3,LaTrama.Data4);
	if (EstemAturats) {
		if (LaTrama.OpCode == P_RESTART) EstemAturats = 0;
		else return;
	}
	switch (LaTrama.OpCode) {
		case P_MOTOR_E:
			MtSetPWM(M_ESQUERRA, Data2, Data1) ; break;
		case P_MOTOR_D:
			MtSetPWM(M_DRET, Data2, Data1) ; break;
		case P_MOTOR_2:
			MtSetPWM(M_2, Data2, Data1) ; break;
		case P_MOTOR_3:
			MtSetPWM(M_3, Data2, Data1) ; break;
		case P_LS_MT_Lineal: 
//			ResponTest();	// Per provar
			MtLineal(Data1, Data2, Data3); 
//			LS_USB_printf("Lineal: Temps=%d Speed =%d Stop = %d\r\n", Data1, Data2, Data3, Data4);
			break;
		case P_LS_MT_TurnRight: 
//			ResponTest();	// Per provar
			MtTurnRight(Data1, Data2, Data3, Data4); 
//			LS_USB_printf("Right: Temps=%d Speed =%d Radi=%d, Stop = %d\r\n", Data1, Data2, Data3, Data4);
			break;
		case P_HBEAT:
			if (Data1 == P_SET_BEAT) IoSetLedBeat(Data2);
			break;
		case P_STOP:
			MtSetPWM(M_ESQUERRA, 0, 0);
			MtSetPWM(M_DRET, 0, 0); 
			MtSetPWM(M_2, 0, 0); 
			MtSetPWM(M_3, 0, 0); 
			EstemAturats = 1; 
			NvSetDireccio(0, 0, 0, 0);
			break;
		case P_ME_A_MD_A: // Motor esquerra endavant, motor dret endavant
			MtSetPWM(M_DRET,     Data1, M_DIR_ENDAVANT);
			MtSetPWM(M_ESQUERRA, Data2, M_DIR_ENDAVANT);
			NvSetDireccio(Data2, M_DIR_ENDAVANT, Data1, M_DIR_ENDAVANT);
			break;
		case P_ME_A_MD_R: // Motor esquerra endavant, motor dret recula
			MtSetPWM(M_DRET,     Data1, M_DIR_ENDARRERA);
			MtSetPWM(M_ESQUERRA, Data2, M_DIR_ENDAVANT);
			NvSetDireccio(Data2, M_DIR_ENDAVANT, Data1, M_DIR_ENDARRERA);
			break;
		case P_ME_R_MD_A: // Motor esquerra recula, motor dret endavant
			MtSetPWM(M_DRET,     Data1, M_DIR_ENDAVANT);
			MtSetPWM(M_ESQUERRA, Data2, M_DIR_ENDARRERA);
			NvSetDireccio(Data2, M_DIR_ENDARRERA, Data1, M_DIR_ENDAVANT);
			break;
		case P_ME_R_MD_R: // Motor esquerra recula, motor dret recula
			MtSetPWM(M_DRET,     Data1, M_DIR_ENDARRERA);
			MtSetPWM(M_ESQUERRA, Data2, M_DIR_ENDARRERA);
			NvSetDireccio(Data2, M_DIR_ENDARRERA, Data1, M_DIR_ENDARRERA);
			break; 
		case P_INIT_TRACK: 
			MtInitTrack(); 
			LS_USB_printf("\n\rINIT TRACK\n\r");
			break;
		case P_END_TRACK : 
			MtEndTrack();  
			LS_USB_printf("\n\rEND TRACK\n\r");
			break;
	}
}


#define PIN_INT_2500 	(_RF6)
void WatchDogCC2500(void) {
	static int Estat = 0;
	switch (Estat) {
		case 0: if (PIN_INT_2500 == 1) ++Estat; break; 	
		case 1: if (PIN_INT_2500 == 1) ++Estat; else --Estat; break; 	
		case 2: if (PIN_INT_2500 == 1) ++Estat; else --Estat; break; 	
		case 3: if (PIN_INT_2500 == 1) ++Estat; else --Estat; break; 	
		case 4:
			// El pobre 2500 s'ha saturat per un excés de trames
			// Solució radical
			RfEnd(); RfInit(); // Quin peligro!
			Estat = 0;
			break; 
	}
}
