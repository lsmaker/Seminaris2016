//-----------------------------------------------------------------------0--------------------------------
// Aquestes rutines permeten controlar l'execuci� de la
// plataforma LS a nivell d'aplicaci�. Podem:
//   a) Coneixer el valor actual de la tensi� de la bateria (LS_SYS_GetBatteryVoltage)
//   b) Apagar l'alimentaci� de la placa (LS_SYS_PowerOff)
//	 c) Definir maquines d'estat que l'executiu del nucli LS cridar� 
//      peri�dicament
//	 d) Rutines per a esperar un temps determinat (LS_SleepS, LS_SleepM)
//------------------------------------------------------------------------------------------------------
#include "../LSApi.h"
struct Process {
	void (*MachineFunction)(void);
	int Periode;
	int TicsActuals;
} Processos[MAX_TASKS];

static int Tempo;
static int Ocupades;
void SysInit(void) { 
	Tempo = LS_SYS_OpenTimer();
	LS_SYS_ResetTics(Tempo);
	Ocupades = 0;
}

void SysMotor(void) {
	unsigned char CalDecrementar, i;
	if (LS_SYS_GetTics(Tempo) >= 1) {
		CalDecrementar = 1; 
		LS_SYS_ResetTics(Tempo);
	} else CalDecrementar = 0;
	for (i = 0; i < Ocupades; i++) {
		// Si el periode �s zero, vol dir que s'ha de cridar sempre
		if (Processos[i].Periode == 0) (*Processos[i].MachineFunction)();
		else {
			// Si ha transcorregut un milisegon, actualitzo el comptador de tics
			if (CalDecrementar) {	
				Processos[i].TicsActuals -= LS_SYS_GetTics(Tempo);
				if (--Processos[i].TicsActuals <= 0) {	
					// Hem arribat al final del comptatge, cal invocar la m�quina d'estats
					(*Processos[i].MachineFunction)();
					// I rearmar el comptador de tics
					Processos[i].TicsActuals = Processos[i].Periode;
				}
			}
		}
	}
}

int LS_SYS_GetBatteryVoltage(void) {
// Post: retorna un numero entre 0 i 100, que indica el nivell de tensi� de la bateria amb un "decimal".
// Es a dir, 72 vol dir 7.2 V.
	// La f�rmula de conversi� �s VBat = (VADC * 33 / 1023) * (R2+R9) / R9
	// Per tal d'evitar treballar amb coma flotant (float o double) far� les multiplicacions amb integer
	// per passos, primer multiplicant i despr�s dividint, ja que si hi fem al reves, perdrem precisi�
	// Cal vigilar que cap multiplicaci� superi el valor de 65535, el valor m�xim
	// d'un unsigned long
	unsigned long t;
#   define R9 		470		// El que importa �s la seva relaci�
#   define R2		1000 	// i no el seu valor real
	t = IoGetBatteryVoltage(); 
	t = 33 * t;    	// Com a molt 1023 * 33 = 33759
	t = t / 1023; 	// Entre 0 i 33 (podriem escriure t /= 1023)
	t = t * (R9+R2);// Entre 0 i 48510
	t = t / R9; 	// Entre 0 i 103
	// Per ajustar les petites imprecisons de fons d'escala, limito manualment el valor m�xim
	if (t > 100) t = 100;
	return (int) t; 
}


int LS_SYS_PowerOff(void) {
// Post: Apaga l'alimentaci� de la placa base.
	IoPowerOff(); 	// i a morir...
	return 0; // no recordo per que havia de retornar alguna cosa aquesta rutina.
}


void LS_SYS_SleepSecs(unsigned int Seconds) {
// Post: atura l'execuci� del programa durant el nombre de segons especificat.
// Mentrestant, l'executiu segueix funcionant per sota
	int i;
	for (i = 0; i < Seconds; i++) LS_SYS_SleepMiliSecs(1000);
}	

void LS_SYS_SleepMiliSecs(unsigned int MiliSeconds) {
// Post: atura l'execuci� del programa durant el nombre de milisegons segons especificat.
// Mentrestant, l'executiu segueix funcionant per sota
	int T;
	T = LS_SYS_OpenTimer();
	LS_SYS_ResetTics(T);
	while (LS_SYS_GetTics(T) < MiliSeconds) {
		LS_Executiu();
	}
	LS_SYS_CloseTimer(T);
}

void LS_SYS_AddStateMachine(void (*MachineFunction)(void), unsigned int Period) {
// Pre: MachineFunction �s el nom d'una funci� que es comporta com
// una m�quina d'estats (vegeu m�s avall). 
// Pre: Periode es major o igual a zero
// Post: A partir d'aquest moment, l'executiu cridar� a la m�quina d'estats
// tant rapidament com pugui si Periode val 0 o cada cert nombre de milisegons,
// on aquest nombre de milisegons ser� el que indiqui el par�metre Periode
//
// Les altres m�quines definides per aquesta funci� segueixen treballant igual
//
// Una m�quina d'estats es una rutina que est� pensada per entrar i sortir
// molt r�pidament, sense fer cap bucle d'espera. Serveix per a que els
// programadors avan�ats puguin fer m�s d'una cosa a la vegada per� hi ha certes
// restriccions dins de les m�quines d'estat:
//	a) No es poden fer bucles d'espera
//  b) No es poden passar par�metres ni pot retornar cap valor
//  c) No es pot cridar a les rutines d'alt nivell, (les que comencen per LS_)
// ja que hi hauria un conflicte d'interessos. El programador pot d'usar les rutines
// de la capa de baix nivell sense problemes. Si que es poden cridar les rutines
// LS_SYS_OpenTimer, LS_SYS_CloseTimer, LS_SYS_GetTics i LS_SYS_ResetTics.
//  Es molt recomanable que les m�quines d'estat siguin autosuficients. Aix� vol dir que
// els timers que pugui necessitar i la seva variable d'estat han d'estar definides i
// inicialitzades dins de la mateixa rutina. Es a dir, cal usar variables de tipus estatic
//
// Per exemple:
// void Maquina1(void) {
//		static int Estat = 0; // El primer cop val zero i conserva el seu valor en successives crides
//		static int Timer;     // Conserva el seu valor en successives crides.
//		switch (Estat) {
//			case 0:	// El primer cop entra aqu�
//				T = LS_OpenTimer(); // Obt� un numero de timer 
//				Estat = 1;	// La segona vegada que entri valdr� 1
//				break;		// Surt 
//			case 1:	// El segon cop entra aqu�
//				// TODO: A partir d'aqu� comen�a la m�qiuna d'estats
//				break;
//		}
// }
//
// Per tal de posar en marxa aquesta m�quina de forma permanent nom�s cal dir
//
// LS_SYS_AddStateMachine(Maquina1, 0);
// 
// Si es vol que aquesta m�quina es posi en marxa cada 10ms, cal dir
// 
// LS_SYS_AddStateMachine(Maquina1, 10);
// 
// Cal tenir present que el per�ode de crida ES UNA MESURA APROXIMADA, no
// s'ha de pendre com un interval de precisi�.
	Processos[Ocupades].MachineFunction = MachineFunction;
	Processos[Ocupades].Periode = Period;
	Processos[Ocupades].TicsActuals = 0;
	++Ocupades;
}

int LS_SYS_OpenTimer (void) {
//Post: Retorna un n�mero de timer per usar les funcions LS_SYS_GetTics i LS_SYS_ResetTics
//Post: si no hi ha cap timer lliure, retorna -1
	return TiGetTimer();
}

void LS_SYS_ResetTics (unsigned char Timer) {
//Pre : Timer ha estat retornat per LS_SYS_GetTimer.
//Post: Engega la temporitzaci� associada a 'Timer'.
	TiResetTics(Timer);
}

unsigned int LS_SYS_GetTics (unsigned char Timer) {
//Pre : Timer ha estat retornat per LS_SYS_GetTimer.
//Pre : la dist�ncia temmporal entre LS_SYS_GetTics i LS_SYS_ResetTics ha de ser menor
// de 30 segons
//Post: retorna el nombre de milisegons transcorregut des de que es va 
// cridar a LS_SYS_ResetTics
	return TiGetTics(Timer);
}

void LS_SYS_CloseTimer (unsigned char Timer) {
//Pre : Timer ha estat retornat per LS_SYS_GetTimer.
//Post: allibera aquest timer
	TiCloseTimer(Timer);
}
