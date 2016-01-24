//-----------------------------------------------------------------
//TAD         :Motors del robot OMNIA
//Autor       :FEC
//Descripció  :Gestiona els 4 PWM del PIC24FJ64GA006
//Prefix      :Mt
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------

//
//----------------------------------------CONSTANTS----------------
//
#define M_ESQUERRA		0
#define M_DRET			1
#define M_2				2
#define M_3				3
#define M_DIR_ENDAVANT 	1
#define M_DIR_ENDARRERA	2
#define M_STOP0		(1<<0)
#define M_STOP1		(1<<1)
#define M_STOP2		(1<<2)
#define M_STOP3		(1<<3)
#define M_STOP4		(1<<4)
#define M_STOP5		(1<<5)
#define M_STOP6		(1<<6)
#define M_STOP7		(1<<7)
#define M_LIN		1		// Moviment lineal (endavant o endarrera)
#define M_GD		3		// Gira dreta (endavant o endarrera)
#define M_GE		4		// Gira esquerra (endavant o endarrera)


#define M_QUIET 		1
#define M_ATURAT 		2
#define M_MOVIMENT		3


//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------PÚBLIQUES----------------
//
void MtInit(void);
void MtMotor(void);
  // Post: motor per a qestionar el moviment del robot. Es cridat pel SO_OmniaScheduler()  
void MtEnd(void);
void MtSetPWM(int Motor, int Nivell, int Direccio);
int  MtGetNivell(int quin); // Només per a test
int  MtGetDir(int quin);    // Només per a test

void MtSetServo(int NumServo, int Posicio);
// Pre: Num Servo val 2 o 3 (
// Pre: Posicio  es un numero entre 0 i 100
// Post: Genera un pols repetitiu que val 1ms si Pos val 0 i 2 ms si pols val 1
// i entremig, pren valors de forma proporcional
// Aquests valors (0,100) és corresponen amb les posicions extremes del servo (en teoria), les
// quals depenen de cada servo concret, ja que no tots tenen el mateix angle d'obertura
// Segurament pots anar més enllà d'aquests límits

unsigned int MtGetPosServo(int NumServo);
// Pre: Num Servo val 2 o 3 
// Post: Retorina la posició actual, fixada per MtSetServo


void MtLineal(unsigned int Time, int Speed, int StopBits);
  // Pre : Time, en milisegons >= 0. Speed: [-100,100]
  // StopBits: Màscara amb MT_STOP_0, MT_STOP_1, ..., MT_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker es mogui cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // MotiuStop retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)

void MtTurnRight(unsigned int Time, int Speed, unsigned int Radius, int StopBits);
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: Màscara amb MT_STOP_0, MT_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a dreta cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les màscares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, només s'atura per StopBits
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores

void MtTurnLeft(unsigned int Time, int Speed, unsigned int Radius, int StopBits);
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

void MtSetNivellStopBits(unsigned char Nivell);
// Pre : Nivell pot valer 1 o 0
// Post: Defineix el valor dels senyals d'stop que fan aturar les rutines de moviment. 
// Si nivell val 1, els motors s'aturen quan els senyals d'STOPi valen 1 (3.3V) o bé zero vols
// si Nivell és zero


unsigned int MtGetTimeFromDistance(int Distance, int Speed);
  // Pre: Distance en centímetres i positiu.
  // Pre: Speed entre -100 i 100, però valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Lineal per 
  // tal que el robot avançi el nombre de centímetres expressats a Distance, a la velocitat
  // expressada per Speed. El punt òptim és dona quan la tensió de bateria està entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%

unsigned int MtGetTimeFromAngle(int Angle, int Speed);
  // Pre: Angle en graus, de 0 a 360.
  // Pre: Speed entre -100 i 100, però valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Turn Left o Right per 
  // tal que el robot giri el nombre de graus expressats a Angle, a la velocitat
  // expressada per Speed. El punt òptim és dona quan la tensió de bateria està entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
  // Només es correcte si el radi de gir és menor de 10


void MtInitTrack ();
// Post: A partir d'aquesta crida, l'exectiu comença a calcular la posició actual en funció de les ordres rebudes
// (LS_MT_Lineal, LS_MT_TurnLeft, LS_MT_TurnRight)
// La posició actual es memoritza cada segon i es pot baixar al PC amb el cable USB i el programa LSLoader

void MtEndTrack ();
// Post: Finalitza el seguiment de la posició 

void MtGetTrack (int *x, int *y);
// Post: Retorna la posició actual de tracking, tenint en compte que després de LS_InitTrack la posició 
// es 0,0,0

void MtGetDireccio (int *Dx, int *Dy);
// Post: Retorna les components x, y del vector direccio (unitari) multiplicats per 1000
// a posició actual de tracking, tenint en compte que després de LS_InitTrack la posició 
// es 0,0 i el vector director és 1,0

int MtGetEstat(void);
	// Post: Retorna: M_QUIET o M_ATURAT o M_MOVIMENT, on QUIET vol dir que ha acabat amb éxit totes
	// les ordres de moviment encomanades; ATURAT vol dir que la darrera ordre s'ha interromput per culpa
	// d'algun dels sensors d'stop i MOVIMENT vol dir que encara està executant una ordre

int MtGetDurada(void);
	// Post: retorna el temps real que ha durat l'última ordre

int MtGetStop(unsigned char stopBits);
	// Pre: StopBits: Màscara amb M_STOP0, M_STOP1, M_STOP7, que indica quins senyals d'stop es vol consultar
	// Post: retorna cert si algun dels senyals d'stop especificat a la màscara StopBits és cert en l'actualitat.


void MtCalibra(int Esquerra, int Dreta);
	// Pre : Esquerra i dreta contenen els valors amb els que cal ajustar el PWM dels motors de tracció
	// Post: Aquests valors es guarden a la EEPROM i es faran servir el proper cop que arrenqui la placa


void MtGetCalibracio(int *Esquerra, int *Dreta);
	// Post: Retorna els valors de calibració actuals

void NvSetOmega(int SpeedEsquerra, int DirEsquerra, int SpeedDreta, int DirDreta);
// Pre : Speed esquerra i dreta de 0 a 100
// Pre : DirEsquerra i DirDreta poden valer  M_DIR_ENDAVANT o M_DIR_ENDARRERA
// Post: Calcula la nova velocitat angular del vector de direcció 
// a partir de les diferències entre la velocitat dels dos motors

void NvSetDireccio(int SpeedEsquerra, int DirEsquerra, int SpeedDreta, int DirDreta);
// Pre : Speed esquerra i dreta de 0 a 100
// Pre : DirEsquerra i DirDreta poden valer  M_DIR_ENDAVANT o M_DIR_ENDARRERA
// Post: Calcula el mòdul de la nova velocitat lineal i de la velocitat angular
// a partir de les diferències entre la velocitat dels dos motors

unsigned char NvAdquirint(void);
// Retorna cert si estem adquirint
//                                                    
//
//------------------------------END-------PÚBLIQUES----------------
//



