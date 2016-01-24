//
//------------------------LS MAKER---PROTOCOL-----------------------------------------------
//
//	TRAMES DEL PC AL ROBOT
//
//-----------------------Generiques------------------------------------------------------
#define P_ACK			0x00		// Trama de reconexiement, no té paràmetres
#define P_STOP			0x01		// Ordre de no executar cap ordre fins rebre RESTART
#define P_RESTART		0x02		// Ordre que anula l'anterior

//---------------------------------------------------------------------------------------
//-----------------------PERIFERICS------------------------------------------------------
//---------------------------------------------------------------------------------------

// ----------------------MOTORS-----------------------------------------------------------
#define	P_MOTOR_E		0x04		// Acció sobré el motor Esquerra
#define	P_MOTOR_D		0x05		// Acció sobré el motor Dret
#define	P_MOTOR_2		0x06		// Acció sobré el motor 2
#define	P_MOTOR_3		0x07		// Acció sobré el motor 3
// Paràmetres dels motors
// El primer paràmetre pot ser:
#define P_MOTOR__ENDAVANT	1
#define P_MOTOR__ENDARRERA	2
// El segon paràmetre és un numero de 0 a 100, que indica la potència
// del motor. 0 Vol dir aturat (sense energia)
// Periferics conjunts
#define P_ME_A_MD_A		0x08		// Motor esquerra endavant, motor dret endavant
#define P_ME_A_MD_R		0x09		// Motor esquerra endavant, motor dret recula
#define P_ME_R_MD_A		0x0A		// Motor esquerra recula, motor dret endavant
#define P_ME_R_MD_R		0x0B		// Motor esquerra recula, motor dret recula
// Els dos paràmetres que queden són el nivell del motor dret i el motor esquerra
#define P_LS_MT_Lineal	0x0c		// Rutina MTLineal. Paràmetres: Temps, Speed i Stop
#define P_LS_MT_TurnRight	0x0d		// Rutina LS_MT_TurnRight. Paràmetres: Time, Speed, Radi i Stop
#define P_LS_MT_TurnLeft	0x22		// Rutina LS_MT_TurnLeft. Paràmetres: Time, Speed, Radi i Stop
#define P_INIT_TRACK	0x0e		// NvInitTrack()
#define P_END_TRACK		0x0f		// NvEndTrack()
// --------------END-----MOTORS-----------------------------------------------------------

//-----------------------LEDS connectats a HBeat------------------------------------------
#define P_HBEAT			0x10
// Paràmetre 1
#define	P_SINCRO		0x01	// Sincronitza el batec
#define P_SET_BEAT		0x02	// Definexi la freqüencia del batec
// El segon paràmetre 	pot valer entre 0 (leds apagats) i >40 (leds permanentment encesos)
// Post:Els valors intermedis (de 1 a 40) indiquen la frequencia de pulsació en dècimes d'herts  
// Exemple: Beat = 20 vol dir 2Hz
//------------------END--LEDS connectats a HBeat------------------------------------------

//-----------------------Assignació de sentit i valor dels GPIO---------------------------
#define P_SET_DIR_GPIO	0x11	// Sentit dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Paràmetre 1: Port
// Pot ser GPIO_0, GPIO_1, GPIO_2 i GPIO_3 
// Paràmetre 2: Dir
// Pot ser DIR_IN (entrada) o DIR_OUT sortida)
#define P_SET_VALUE_GPIO	0x12	// Defineix el valor de GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Paràmetre 1: Port
// Pot ser GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Paràmetre 2: Value 
// Val 1 (3.3 V) o 0 (0 V) 
#define P_GET_VALUE_GPIO	0x13	// Demana el valor de GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Paràmetre 1: Port
// Pot ser GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Aquesta trama obliga l'LS Maker a retornar una trama P_GET_VALUE_GPIO
#define P_GET_ANALOG_FILTERED	0x14	// Demana el valor filtrat de AN_0, AN_1, AN_2 o AN_3; 
// Paràmetre 1: Port
// Pot ser: AN_0, AN_1, AN_2 o AN_3; 
// Aquesta trama obliga l'LS Maker a retornar una trama P_GET_ANALOG_FILTERED
#define P_GET_ANALOG			0x15	// Demana el valor sense filtrar de AN_0, AN_1, AN_2 o AN_3; 
// Paràmetre 1: Port
// Pot ser: AN_0, AN_1, AN_2 o AN_3; 
// Aquesta trama obliga l'LS Maker a retornar una trama P_GET_ANALOG_FILTERED
#define P_GET_BUTTON_PRESS_RELEASE  0x16
// Retorna la trama P_GET_BUTTON_PRESS_RELEASE amb Data1 = IoGpButtonRelease()
#define P_GET_BUTTON_PRESS		0x17
// Retorna la trama P_GET_BUTTON_PRESS_RELEASE amb Data1 = IoGpButton()
#define P_GET_BATTERY			0x18
// Retorna la trama P_GET_BATTERY amb Data1 = IoGetBatteryVoltage()
#define P_POWER_OFF				0x20
// Executa IoPowerOff, no és segur que retorni res
#define P_RESPOSTA_MOTORS		0x21
// Trama de resposta a P_LS_MT_Lineal, Turn left i turn right, informa de la durada i dels stopbits
#define P_SetNivellStopBits		0x23	
// Parametre 1: nivell, crida a MtSetNivellStopBits
#define P_GetTimeFromDistance	0x24		
// Parametre 1: Distance, 2 Speed, crida MtGetTimeFromDistance(int Distance, int Speed)
#define P_GetTimeFromAngle		0x25
// Parametre 1: Angle, 2 Speed, crida MtGetTimeFromAngle(int Angle, int Speed)
#define P_GetTrack				0x26
// Retorna el resultat de la crida a MtGetTrack
#define P_GetDireccio			0x27
// Retorna el resultat de la crida a MtGetDireccio
#define P_IsStop				0x28
// Retorna el resultat de la crida a P_IsStop
#define P_GetShake				0x29
// Retorna el resultat de la crida a AcGetShake();
#define P_GetTap				0x2A
// Retorna el resultat de la crida a AcGetTap();
#define P_GetPosXYZ				0x2B
// Retorna el resultat de la crida a AcGetXYZInt()
#define P_ARP					0x2C
// Crida generica a respondre amb una P_ARP. De moment no es fa servir, però
// mes endavant permetrà usar els punts d'accés LSMAker

// Control de servos (V0.5)
#define P_Servo					0x3c

//----------------------------------------------------------------------------------------
//-----------------------ORDRES-----------------------------------------------------------
//----------------------------------------------------------------------------------------

void PtInit(void);
void PtMotor(void);

// TRAMES DEL LSMAKER AL PC, com a resposta a peticions del PC

// P_VALUE_GPIO
