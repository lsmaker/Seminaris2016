//----------------------------------------------------------------------------------------
// Rutines de moviment del robot.
//
//
//----------------------------------------CONSTANTS----------------
//
#include "../LSApi.h"


/* definides a LSApi.h
#define MT_LEFT			0
#define MT_RIGHT		1
#define MT_FORWARD 		1
#define MT_BACKWARD		2

#define MT_STOP_0				(1<<0)
#define MT_STOP_1				(1<<1)
#define MT_STOP_2				(1<<2)
#define MT_STOP_3				(1<<3)
#define MT_STOP_4				(1<<4)
#define MT_STOP_5				(1<<5)
#define MT_STOP_6				(1<<6)
#define MT_STOP_7				(1<<7)
*/

int EsperaFiOrdre(int StopBits, int *StopReasons) {
	do {
		LS_Executiu();
	} while (MtGetEstat() == M_MOVIMENT);
	// Si ha sortit per que ha acabat l'ordre, poso a zero StopReasons
	if (MtGetEstat() == M_QUIET) *StopReasons = 0; else *StopReasons = MtGetStop(StopBits);
	return MtGetDurada();
}

//
//------------------------------END-------CONSTANTS----------------
//

unsigned int LS_MT_Lineal(unsigned int Time, int Speed, int StopBits, int * StopReasons) {
  // Pre : Time, en milisegons >= 0. Speed: [-100,100]
  // StopBits: Màscara amb MT_STOP_0, MT_STOP_1, ..., MT_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker es mogui cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // MotiuStop retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
	MtLineal(Time, Speed, StopBits);
	return EsperaFiOrdre(StopBits, StopReasons);
}


unsigned int LS_MT_TurnRight(unsigned int Time, int Speed, unsigned int Radius, int StopBits, int * StopReasons) {
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: Màscara amb MT_STOP_0, MT_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a dreta cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores
	MtTurnRight(Time, Speed, Radius, StopBits);
	return EsperaFiOrdre(StopBits, StopReasons);
}

unsigned int LS_MT_TurnLeft(unsigned int Time, int Speed, unsigned int Radius, int StopBits, int * StopReasons) {
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: Màscara amb M_STOP_0, M_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a esquerra cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores
	MtTurnLeft(Time, Speed, Radius, StopBits);
	return EsperaFiOrdre(StopBits, StopReasons);
}

void LS_MT_SetNivellStopBits(unsigned char Nivell) {
// Pre : Nivell pot valer 1 o 0
// Post: Defineix el valor dels senyals d'stop que fan aturar les rutines de moviment. 
// Si nivell val 1, els motors s'aturen quan els senyals d'STOPi valen 1 (3.3V) o bé zero vols
// si Nivell és zero
	LS_Executiu();
	MtSetNivellStopBits(Nivell);
}


unsigned int LS_MT_GetTimeFromDistance(int Distance, int Speed) {
  // Pre: Distance en centímetres i positiu.
  // Pre: Speed entre -100 i 100, però valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Lineal per 
  // tal que el robot avançi el nombre de centímetres expressats a Distance, a la velocitat
  // expressada per Speed. El punt òptim és dona quan la tensió de bateria està entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
	LS_Executiu();
	return MtGetTimeFromDistance(Distance, Speed);
}

unsigned int LS_MT_GetTimeFromAngle(int Angle, int Speed) {
  // Pre: Angle en graus, de 0 a 360.
  // Pre: Speed entre -100 i 100, però valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Turn Left o Right per 
  // tal que el robot giri el nombre de graus expressats a Angle, a la velocitat
  // expressada per Speed. El punt òptim és dona quan la tensió de bateria està entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
	LS_Executiu();
	return MtGetTimeFromAngle(Angle, Speed);
}

void LS_MT_InitTrack () {
// Post: A partir d'aquesta crida, l'exectiu comença a calcular la posició actual en funció de les ordres rebudes
// (LS_MT_Lineal, LS_MT_TurnLeft, LS_MT_TurnRight)
// La posició actual es memoritza cada segon i es pot baixar al PC amb el cable USB i el programa LSLoader
	LS_Executiu();
	MtInitTrack();
}

void LS_MT_EndTrack () {
// Post: Finalitza el seguiment de la posició 
	LS_Executiu();
	MtEndTrack();
}

void LS_MT_GetTrack (int *x, int *y) {
// Post: Retorna la posició actual de tracking, tenint en compte que després de LS_InitTrack la posició 
// es 0,0,0 i la direcció coincideix amb l'eix de les x
	LS_Executiu();
	MtGetTrack(x,y);
}

void LS_MT_GetDireccio (int *dx, int *dy) {
// Post: Retorna les components x, y del vector direccio (unitari) multiplicats per 1000
// a posició actual de tracking, tenint en compte que després de LS_InitTrack la posició 
// es 0,0 i el vector director és 1,0
	LS_Executiu();
	MtGetDireccio (dx, dy);
}

int LS_MT_IsStop(unsigned int StopBits) {
  // Pre: StopBits: Màscara amb M_STOP_0, M_STOP_1, M_STOP_7, que indica quins senyals d'stop es vol consultar
  // Post: retorna cert si algun dels senyals d'stop especificat a la màscara StopBits és cert en l'actualitat.
	LS_Executiu();
	return MtGetStop(StopBits);
}

void LS_MT_SetServo(int NumServo, int Posicio) {
// Pre: Num Servo val 2 o 3 
// Pre: Posicio  es un numero entre 0 i 100
// Post: Genera un pols repetitiu que val 1ms si Pos val 0 i 2 ms si pols val 1
// i entremig, pren valors de forma proporcional. Aquest pols apareix en els pins PWM2 i PWM3 respectivament
// del connector CN5 a la placa de l'LSMaker
// Aquests valors (0,100) és corresponen amb les posicions extremes del servo (en teoria), les
// quals depenen de cada servo concret, ja que no tots tenen el mateix angle d'obertura
// Segurament pots anar més enllà d'aquests límits
// Els servomotors tenen sempre tres cables: la massa, l'alimentacion (5V) i el senyal de control (el pols 
// que obtenim en PWM2 i PWM3. Lamentablement, no hi ha cap consens pel que fa a l'ordre i colors dels
// cables. A la web hi ha moltes pàgines dedicades a explicar-ho, es troba amb "servo motor pinout".
	LS_Executiu();
	MtSetServo(NumServo, Posicio);
}

unsigned int LS_MT_GetPosServo(int NumServo) {
// Pre: Num Servo val 2 o 3 
// Post: Retorina la posició actual, fixada per MtSetServo
	LS_Executiu();
	return MtGetPosServo(NumServo);
}

