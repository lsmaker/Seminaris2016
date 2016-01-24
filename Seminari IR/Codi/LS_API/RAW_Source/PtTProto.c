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
static int NoAck;
//static int Rssi;
static unsigned char TelecontrolRebut = 0;
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
				NoAck = 0;
				ExecutaTrama(); 
				if (NoAck == 0) EnviaAck(); // Només enviem ack a les trames que no retornen res
				TiResetTics(T); IoSetLedBeat(20);
				++Estat;
			} 
			break;
		case 1:
			if (TiGetTics(T) >= 1200) {
				if (!TelecontrolRebut) {
					// Un segon sense cap trama, paro motors
					MtSetPWM(M_ESQUERRA, 0, 0); MtSetPWM(M_DRET, 0, 0); MtSetPWM(M_2, 0, 0); MtSetPWM(M_3, 0, 0); 
					NvSetDireccio(0, 0, 0, 0);	
					TiResetTics(T);
					IoSetLedBeat(5);
					Estat = 0;
				}
			}
			if (RfHiHaTrama()) {
				NoAck = 0;
				LaTrama = RfGetTrama();
				ExecutaTrama(); 
				if (NoAck == 0) EnviaAck(); // Només enviem ack a les trames que no retornen res
				TiResetTics(T); IoSetLedBeat(20); 
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

void EnviaRespostaMotors(int Durada, int StopReasons) {
	RfSendTrama((unsigned char *)LaTrama.Origen, P_RESPOSTA_MOTORS, Durada, StopReasons, 0, 0);
}

#include <stdio.h>


void ExecutaTrama(void) { 
	int Data1, Data2, Data3, Data4, tmp, x, y, z;
	Data1 = LaTrama.Data1;
	Data2 = LaTrama.Data2;
	Data3 = LaTrama.Data3;
	Data4 = LaTrama.Data4;
//	LS_USB_printf("OP=%d D1=%d D2=%d D3=%d D4=%d        \r\n", LaTrama.OpCode, LaTrama.Data1, LaTrama.Data2, LaTrama.Data3,LaTrama.Data4);
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
			TelecontrolRebut = 1; // A partir d'ara, no hi ha protecció de motors.
			MtLineal(Data1, Data2, Data3); 
			break;
		case P_LS_MT_TurnRight: 
			TelecontrolRebut = 1; // A partir d'ara, no hi ha protecció de motors.
			MtTurnRight(Data1, Data2, Data3, Data4); 
			break;
		case P_LS_MT_TurnLeft: 
			TelecontrolRebut = 1; // A partir d'ara, no hi ha protecció de motors.
			MtTurnLeft(Data1, Data2, Data3, Data4); 
			break;
		case P_Servo:
			MtSetServo(2, Data1);
			MtSetServo(3, Data2);
			break;
		case P_HBEAT:
			if (Data1 == P_SET_BEAT) IoSetLedBeat(Data2);
			if (Data1 == P_SINCRO) IoSincro();
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
			//LS_USB_printf("\n\rINIT TRACK\n\r");
			break;
		case P_END_TRACK : 
			MtEndTrack();  
			//LS_USB_printf("\n\rEND TRACK\n\r");
			break;
		case P_SET_DIR_GPIO:
			IoSetDirection(Data1, Data2); 
			break;
		case P_SET_VALUE_GPIO:
			IoSetValue(Data1, Data2);
			break;
		case P_GET_VALUE_GPIO:
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GET_VALUE_GPIO, IoGetValue(Data1), 0, 0, 0);
			break; 
		case P_GET_ANALOG_FILTERED:
			NoAck = 1; tmp = IoGetAnalogFiltered(Data1);
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GET_ANALOG_FILTERED, tmp, tmp >> 8, 0, 0);
			break; 
		case P_GET_ANALOG:
			NoAck = 1; tmp = IoGetAnalog(Data1);
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GET_ANALOG, tmp, tmp >> 8, 0, 0);
			break; 
		case P_GET_BUTTON_PRESS_RELEASE:
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GET_BUTTON_PRESS_RELEASE, IoGpButtonRelease(), 0, 0, 0);
			break; 
		case P_GET_BUTTON_PRESS:
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GET_BUTTON_PRESS, IoGpButtonPress(), 0, 0, 0);
			break; 
		case P_GET_BATTERY:
			tmp = IoGetBatteryVoltage();
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GET_BATTERY, tmp, tmp>>8, 0, 0);
			break; 
		case P_POWER_OFF:
			// IoPowerOff(); ULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
			break;
		case P_SetNivellStopBits:
			MtSetNivellStopBits(Data1);
			break;
		case P_GetTimeFromDistance:
			tmp = MtGetTimeFromDistance(Data1, Data2);
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetTimeFromDistance, tmp,0 , 0, 0);
			break;
		case P_GetTimeFromAngle:
			tmp = MtGetTimeFromAngle(Data1, Data2);
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetTimeFromAngle, tmp, 0, 0, 0);
			break;
		case P_GetTrack:
			MtGetTrack(&x, &y); NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetTrack, x, y, 0, 0);
			break;
		case P_GetDireccio:
			MtGetDireccio(&x, &y); NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetDireccio, x, y, 0, 0);
			break;
		case P_IsStop:
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_IsStop, MtGetStop(Data1), 0, 0, 0);
			break; 
		case P_GetShake:
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetShake, AcGetShake(), 0, 0, 0);
			break; 
		case P_GetTap:
			NoAck = 1;
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetTap, AcGetTap(), 0, 0, 0);
			break; 
		case P_GetPosXYZ:
			NoAck = 1;
			AcGetXYZInt(&x, &y, &z); 
			RfSendTrama((unsigned char *)LaTrama.Origen, P_GetPosXYZ, x, y, z, 0);
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
