
#include <stdio.h>

#ifdef	__PIC24FJ64GA006__
#define LSMAKER_BASE	// Es la placa base, si no, �s el control remot (hi ha codi compartit entre tots dos.
//#include <p24fxxxx.h>
#include <xc.h>
#endif 

// Els tads de baix nivell
// No hi son tots, n'hi ha que els he declarat al final per poder usar les constants
// definides en l'api

//#define API_V00	"0.0"		// Per a l'historics de versions

//#define API_V00	"0.1"	// 19/10/2010: Afegim la gesti� de l'API remota
//#define API_V00	"0.2"	// 1/4/2011: Afegim el concepte de broadcast lite per a fer compatibles els LSMAker amb els LSMaker Lite
//#define API_V00	"0.3"	// 6/4/2011: Refino el control de posici� i tinc en compte la calibraci� de traccio. Tamb� paro els motors despres de reset
#define API_V00	"0.4"	// 26/1/2012: Afegeixo el segon bot� de proposit general, V2 del HW

#ifndef	SOFT_PC
#include "./RAW_Source/TiTTImer.h"
#include "./RAW_Source/SiTSio.h"
#include "./RAW_Source/IoTIO.H"
#include "./RAW_Source/LCTLcd.h"
#include "./RAW_Source/I2Ti2c.h"
#include "./RAW_Source/MtTMotors.h"
#include "./RAW_Source/Protocol.h"
#include "./RAW_Source/ActAcc.h"
#include "./RAW_Source/MtTMotors.h"
#include "./RAW_Source/NvTNovol.h"
#endif

#include "./RAW_Source/RfTRadio.h"

#define VERSIO_LIB API_V00

// -----------------------------------Nom�s per a verificacions puntuals-------
#define ACTIVA_TRASSA (_TRISE0 = 0)	
#define TRASSA_ON 	(_LATE0 = 1)	// Pin 35 (el que fa sis compen�ant pel 40
#define TRASSA_OFF 	(_LATE0 = 0)

void LS_Init(void);
// El super constructor de la API del LS Maker
// MODE REMOT: Permesa

void LS_Executiu(void);
// El multimotor
// MODE REMOT: Permesa

int LS_OpenCom(int Com);
// Pre : Com entre 1 i 15. Nom�s te sentit en mode remot, no fa res en mode local
// Post: Obre aquest canal de comunicaci� per parlar amb el control remot
// Retorna 0 si ha pogut obrir el com, aix� no vol dir que en aquest COM
// hi hagi un control remot
// MODE REMOT: Permesa
// MODE LOCAL: no fa res

int LS_CloseCom(int Com);
// Pre : Com entre 1 i 15. Nom�s te sentit en mode remot, no fa res en mode local
// Post: Tanca aquest canal de comunicaci� 
// Retorna 0 si ha pogut tancar el com
// MODE REMOT: Permesa
// MODE LOCAL: no fa res

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� dels senyals d'entrada/sortida. 
// Prefix    : LS_IO
//   Aquestes rutines permeten accedir als quatre ports d'entrada anal�gics
// anomentas AN_0, AN_1, AN_2 i AN_3 en els esquemes, que trobareu en els 
// connectors d'expansi�.
//   Tamb� permet definir la direcci� (entrada o sortida) dels ports 
// GPIO_0, GPIO_1, GPIO_2 i GPIO_3, que teniu a disposici� en els connectors
// d'expansi�
//   A m�s, amb aquestes rutines podreu:
//   a) Saber si s'ha premut o s'ha deixat anar el polsador de prop�sit general
//      etiquetat com GpButton a la placa de circuit impr�s
//   b) Governar els leds de la placa
// TOTES LES RUTINES EN MODE REMOT TRIGUEN 30 ms
//------------------------------------------------------------------------------------------

#define GPIO_0	0
#define GPIO_1	1
#define GPIO_2	2
#define GPIO_3	3

#define AN_0	0
#define AN_1	1
#define AN_2	2
#define AN_3	3


#define DIR_OUT		0
#define DIR_IN		1

void LS_IO_SetDirection(int Port, int Dir);
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Dir val DIR_IN (entrada) o DIR_OUT sortida)
// Post: Defineix el sentit dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Post: Per defecte, el valor que pren un port de sortida �s 0 (0V)
// MODE REMOT: Permesa

void LS_IO_SetValue(int Port, int Value);
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Value val 1 (3.3 V) o 0 (0 V)
// Post: Defineix el valor dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a sortides
// MODE REMOT: Permesa 

int LS_IO_GetValue(int Port);
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 o GPIO_3
// Post: Retorna el valor actual dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a entrades. Retorna 1 si la tensi� en el port
// �s superior a 2.3V i 0 si la tensi� del port �s inferior a 0.9 V
// Altrament no es pot assegurar el valor
// MODE REMOT: Permesa 

int LS_IO_GetAnalog(int Port); 
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor anal�gic actual (AN_0, AN_1, AN_2 o AN_3), sense cap mena de filtratge, de
// forma que si la tensi� en el port �s 3,1V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal. Cal esperar un error del 5%
// MODE REMOT: Permesa 

int LS_IO_GetAnalogFiltered(int Port); 
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor anal�gic actual (AN_0, AN_1, AN_2 o AN_3), amb un filtratge, basat en un promig
// m�bil de 8 mostres a ritme de 2.5 ms per mostra. (Filtre de 50Hz)
// Igual que amb GetAnalog, si la tensi� en el port �s 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.
// MODE REMOT: Permesa 


int LS_IO_GpButtonPress(void);
// Post: Retorna cert si s'ha premut el polsador GpButton, convenientment filtrat contra
// rebots. La lectura �s destructiva.
// MODE REMOT: Permesa 

int LS_IO_GpButtonRelease(void);
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton, convenientment filtrat contra
// rebots. La lectura �s destructiva.
// MODE REMOT: Permesa 

int LS_IO_GpButton2Press(void);
// Post: Retorna cert si s'ha premut el polsador GpButton 2 (versi� 2 del HW), convenientment filtrat contra
// rebots. La lectura �s destructiva.
// MODE REMOT: Permesa 

int LS_IO_GpButton2Release(void);
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton 2 (versio 2 del HW), convenientment filtrat contra
// rebots. La lectura �s destructiva.
// MODE REMOT: Permesa 

void LS_IO_SetLedBeat(int Beat);
// Pre: Beat pot valer entre 0 (leds apagats) i > 40 (leds permanentment encesos)
// Post:Els valors intermedis (de 1 a 40) indiquen la frequencia de pulsaci� en d�cimes d'Hz  
// Exemple: Beat = 20 vol dir 2Hz
// MODE REMOT: Permesa

void LS_IO_Sincro(void);
// Post: reset del ritme de batec
// MODE REMOT: Permesa

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� de l'execuci�
// Prefix    : LS_SYS
// Aquestes rutines permeten controlar l'execuci� de la
// plataforma LS a nivell d'aplicaci�. Podem:
//   a) Coneixer el valor actual de la tensi� de la bateria (LS_SYS_GetBatteryVoltage)
//   b) Apagar l'alimentaci� de la placa (LS_SYS_PowerOff)
//	 c) Definir maquines d'estat que l'executiu del nucli LS cridar� 
//      peri�dicament
//	 d) Rutines per a esperar un temps determinat (LS_SleepS, LS_SleepM)
//------------------------------------------------------------------------------------------

int LS_SYS_GetBatteryVoltage(void);
// Post: retorna un numero entre 0 i 100, que indica el nivell de tensi� de la bateria amb un "decimal".
// Es a dir, 72 vol dir 7.2 V. Quan el sistema est� alimentat externament, no es pot fer cas d'aquest valor,
// ja que val VMain, que sol ser 12 o 13 volts i el senyal BatSample supera els 3.3v. De fet, si auqesta funci� 
// dona 100, es una indicaci� clara de que estem alimentats externament, ja que la bateria, per si sola, mai arriba
// a donar 10V.
// MODE REMOT: Permesa

int LS_SYS_PowerOff(void);
// Post: Apaga l'alimentaci� de la placa base.
// MODE REMOT: Permesa, per� no retorna ack per que es mora abans.


void LS_SYS_SleepSecs(unsigned int Seconds);
// Pre : Seconds > 0
// Post: atura l'execuci� del programa durant el nombre de segons especificat.
// Mentrestant, l'executiu segueix funcionant per sota
// MODE REMOT: es construiex amb un bucle de Sleep(1000)

void LS_SYS_SleepMiliSecs(unsigned int MiliSeconds);
// Pre : MiliSeconds > 0
// Post: atura l'execuci� del programa durant el nombre de milisegons segons especificat.
// Mentrestant, l'executiu segueix funcionant per sota
// MODE REMOT: Mapeja  a Sleep;

#define MAX_TASKS 8

void LS_SYS_AddStateMachine(void (*MachineFunction)(void), unsigned int Period);
// Pre: MachineFunction �s el nom d'una funci� que es comporta com
// una m�quina d'estats (vegeu m�s avall). 
// Pre : No hi pot haver m�s de MAX_TASKS m�quines d'estat (per defecte, 8).
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
// MODE REMOT: NO Permesa

int LS_SYS_OpenTimer (void);
//Post: Retorna un n�mero de timer per usar les funcions LS_SYS_GetTics i LS_SYS_ResetTics
//Post: si no hi ha cap timer lliure, retorna -1
// MODE REMOT: Permesa

void LS_SYS_ResetTics (unsigned char Timer);
//Pre : Timer ha estat retornat per LS_SYS_GetTimer.
//Post: Engega la temporitzaci� associada a 'Timer'.
// MODE REMOT: Permesa

unsigned int LS_SYS_GetTics (unsigned char Timer);
//Pre : Timer ha estat retornat per LS_SYS_GetTimer.
//Pre : la dist�ncia temmporal entre LS_SYS_GetTics i LS_SYS_ResetTics ha de ser menor
// de 30 segons
//Post: retorna el nombre de milisegons transcorregut des de que es va 
// cridar a LS_SYS_ResetTics
// MODE REMOT: Permesa, per� amb una ressoluci� de 15ms

void LS_SYS_CloseTimer (unsigned char Timer);
//Pre : Timer ha estat retornat per LS_SYS_GetTimer.
//Post: allibera aquest timer
// MODE REMOT: Permesa

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� del display LCD 
// Prefix    : LS_IO
// Rutines per a manipular el display alfanum�ric del LS MAker, que t�
// quatre files per setze columnes cadascuna. Atenci�: l'acc�s al LCD �s for�a lent: uns 50ms
// per escriure una fila. Aix� vol dir que no pots estressar l'LCD amb un ritme d'escriptura
// m�s r�pid que una fila cada 50ms.
//------------------------------------------------------------------------------------------

void LS_LCD_Clear(void);
// Post: Esborra el display i posa el cursor a la posici� zero en 
// l'estat en el que estava. 
// MODE REMOT: NO PERMESA

void LS_LCD_CursorOn(void);
// Post: Enc�n el cursor
// Post: La propera ordre pot trigar fins a 40us
// MODE REMOT: NO PERMESA

void LS_LCD_CursorOff(void);
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us
// MODE REMOT: NO PERMESA

void LS_LCD_GotoXY(unsigned int Columna, unsigned int Fila);
// Pre : Columna entre 0 i 15, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// MODE REMOT: NO PERMESA

void LS_LCD_PutChar(char C); 
// Post: Pinta C en l'actual poscici� del cursor i incrementa 
// la seva posici�. Si se supera la columna 15, el cursor salta a la fila seg�ent, 
// en cas que sigui la �ltima, el cursor salta a la posici� 0,0
// MODE REMOT: NO PERMESA

void LS_LCD_PutString(char *s);
// Post: Pinta l'string s (null terminated apartir de la posici� actual del cursor.
// El criteri de coordenades �s el mateix que a LS_LCD_PutChar. Si se supera la columna 15, el cursor salta a la fila seg�ent, 
// en cas que sigui la �ltima, el cursor salta a la posici� 0,0
// MODE REMOT: NO PERMESA

void LS_LCD_Printf(int X, int Y, char *fmt, ... );
// Pre : Columna entre 0 i 15, Fila entre 0 i 3
// Post: rutina equivalent a la printf, per� amb la difer�ncia que el 
// resultat el pinta en el display a la fila i columna especificada. Si se supera la columna 15, el cursor salta a la fila seg�ent, 
// en cas que sigui la �ltima, el cursor salta a la posici� 0,0
// MODE REMOT: NO PERMESA

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� de la mem�ria no vol�til
// Prefix    : LS_NVOL
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

//------------------------------------------------------------------------------------------
#define NVOL_LARGE_MAC_SIZE		6
#define NVOL_SHORT_MAC_SIZE		3
#define NVOL_INVALID_MODE		-2
#define NVOL_EOF				-1
#define NVOL_STREAM_A			"streamA"
#define NVOL_STREAM_B			"streamB"

void LS_NVOL_EERead(unsigned int Pos, unsigned char *buffer, unsigned int Length);
// Pre : Pos + Lenght < 128
// Pre : Pos i Length positius
// Post: Llegeix el nombre de caracters indicats a Lenght a partir
// de l'adre�a Pos de l'EEPROM i els posa a l'array buffer
// MODE REMOT: No Permesa

void LS_NVOL_EEWrite(unsigned int Pos, unsigned char *buffer, unsigned int Length);
// Pre : Pos + Lenght < 128
// Pre : Pos i Length positius
// Post: Escriu el nombre de caracters indicats a Lenght de l'array
// buffer a partir de l'adre�a Pos de l'EEPROM. Requereix 6 milisegons per cada byte.
// MODE REMOT: No Permesa

int LS_NVOL_Open(char *Nom, char *Mode);
// Pre: Nom pot ser NVOL_STREAM_A o NVOL_STREAM_B. Mode pot ser "a", "r", o "w"
// Si es "r", mode lectura. La lectura ser� sequencial a partir
// de l'inici del fitxer. No es permet escriure (retornaria NVOL_INVALID_MODE)
// Si �s "w", esborra el contingut del fitxer i es permet escriure a partir
// de l'inici. No es permet llegir (retornaria NVOL_INVALID_MODE)
// Si �s "a", el fitxer s'obre en mode d'escriptura a partir del final del fitxer.
// No es permet llegir (retornaria NVOL_INVALID_MODE)
// Post: El valor que retorna s'ha de fer servir com a HANDLER en la resta de la
// rutina. En principi, no s'espera cap possible error.
// MODE REMOT: Mapeja a fopen, el valor de retorn no �s igual

int LS_NVOL_Close(int HANDLE);
// Pre : HANDLE prov� de LS_NVOL_Open
// Post: Tanca el fitxer
// MODE REMOT: mapeja a fclose

int LS_NVOL_Read(int HANDLE, char *buffer, unsigned int length);
// Pre : HANDLE prov� de LS_NVOL_Open
// Post: Llegeix el nombre de caracters (length) del fitxer i els posa en buffer.
// La seguent lectura es far� a partir d'on hagi acabat aquesta.
// Retorna el nombre de caracters que ha llegir realment o EOF si ha arribat 
// al final del fitxer. Retorna NVOL_INVALID_MODE si el fitxer s'ha obert amb "w" o amb "a"
// MODE REMOT: Mapeja a fread, el valor de retorn no �s igual


void LS_NVOL_ReOpen(int HANDLE);
// Pre : HANDLE prov� de LS_Open. El fitxer s'ha obert en mode "r"
// Post: Posiciona el punter intern a l'inici del fitxer. La seguent
// ordre de lectura comen�ar� a partir de l'inici del fitxer
// MODE REMOT: NO PERMESA

int LS_NVOL_Write(int HANDLE, char *buffer, unsigned int length);
// Pre : HANDLE prov� de LS_NVOL_Open 
// Post: Escriu el nombre de caracters del buffer (length) en el fitxer
// La seguent escriptura es far� a partir d'on hagi acabat aquesta.
// Retorna el nombre de caracters que ha escriut realment o EOF si ha arribat 
// al l�mit final del fitxer (1920)
// Retorna NVOL_INVALID_MODE si el fitxer s'ha obert amb "r"
// MODE REMOT: Mapeja a fwrite, el valor de retorn no �s igual

int LS_NVOL_Eof(int HANDLE);
// Pre : HANDLE prov� de LS_NVOL_Open.
// Post: retorna cert si el punter intern del fitxer ha arribat al final,
// tant per lectura com per escriptura.
// MODE REMOT: Mapeja a feof, el valor de retorn no �s igual

int LS_NVOL_Size(int HANDLE);
// Pre : HANDLE prov� de LS_NVOL_Open.
// Post: Retorna el nombre de car�cters que t� el fitxer
// MODE REMOT: No permesa


//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� del les comunicacions per radiofreq��ncia
// Prefix    : LS_COM
// Aquestes rutines permeten enviar i rebre trames entre diferens LS. No serveix
// per a comunicar-se amb el control remot, ja que aix� ho fa de forma autom�tica
// l'executiu de la API LS de forma transparent
// 
// El nivell d'intensitat de senyal que retorna la rutina LS_COM_GetFrame d�na
// una idea APROXIMADA de la dist�ncia a la que es troba el LS que ha enviat
// la trama. Aquesta dist�ncia varia en funci� de la humitat relativa, del
// nombre de persones o altres b�sties que es trobin entre els dos cotxes o 
// del nombre de rebots que a patiti el senyal
// 
// El format de les trames que s'envien (i aix� �s valid per a les trames
// ocultes entre el LS i el seu control remot) segueix el seg�ent patro
// 
// [ADRE�A DESTI, 3 bytes][ADRE�A ORIGEN, 3 bytes][Dades, 9 bytes]
//
// El hardware s'encarrega de verificar el CRC i no deixar� passar una
// trama si no es pot garantir la seva correcci�.
// Per tal de simplificar el sofware al m�xim, aquestes rutines usen la mateixa
// infraestructura que permet el control remot del LS maker i la API remota. Per aix�, i
// per evitar conflictes, cal que el programador posi un 0xff en el primer byte
// de la trama: aix� obre un "tunel" que permet enviar el que es vulgui a qui es 
// vulgui, sense cap mena d'interfer�ncia. 
// 
// No hi ha cap mena de garantia de que en enviar una trama, aquesta
// arribi al seu dest�, ni hi ha cap informaci� sobre si realment ha arribat. 
// 
//-----------------------------------------------------------------------------------------
#define COM_LARGE_MAC_SIZE		(RF_LEN_ADDRESS+3)
#define COM_SHORT_MAC_SIZE		RF_LEN_ADDRESS
#define COM_FRAME_SIZE			RF_FRAME_SIZE 


void LS_COM_GetLargeMAC(unsigned char MAC[COM_LARGE_MAC_SIZE]);
// Post: Posa en MAC el valor de l'adre�a mac en format llarg
// Aquesta adre�a es troba emmagatzemada a l'EEPROM i es pot garantir que �s �nica.
// MODE REMOT: No permesa

void LS_COM_GetShortMAC(unsigned char MAC[COM_SHORT_MAC_SIZE]);
// Post: Posa en MAC el valor abreujat de l'adre�a mac del LS MAker en format llarg
// Aquesta adre�a es troba emmagatzemada a l'EEPROM i es pot garantir que �s �nica.
// MODE REMOT: Retorna l'adre�a MAC del control remot que hi hagi connectat

void LS_COM_SetPromiscuousMode(void);
// Post: Activa el mode promiscu. Totes les trames coherents que es detectin
// seran informades a les rutines LS (es refereix a les trames amb 0xff al primer byte)
// MODE REMOT: No permesa

void LS_COM_UnsetPromiscuousMode(void);
// Post: Desactiva el mode promiscu. Nom�s les trames adre�ades al LS 
// seran informades a les rutines LS (es refereix a les trames amb 0xff al primer byte)
// MODE REMOT: No permesa

int LS_COM_FrameAvailable(void); 
// Post: Retorna cert si s'ha rebut una trama adre�ada al LS
// (si el mode promiscu �s fals) o b� s'ha rebut una trama adre�ada a qualsevol
// (per� nom�s si el mode promiscu �s cert)
// MODE REMOT: No permesa

int  LS_COM_GetFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Origin[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]);
// Pre : LS_COM_FrameAvailable �s cert
// Post: Omple els camps Desti (que en mode no promiscu coincideix amb l'adre�a local), Origen i Dades
// Retorna el nivell de RSSI (for�a del senyal rebut) expressat en dBm, de -10 (m�xim nivell rebut)
// fins a -100 (m�nim nivell rebut)
// Aquesta rutina nom�s retorna trames tals que el primer byte valgui 0xff, ja que la resta de trames
// estan reservades per al protocol de comunicaci�.
// MODE REMOT: No permesa

void LS_COM_SendFrame(unsigned char Destination[COM_SHORT_MAC_SIZE], unsigned char Data[COM_FRAME_SIZE]);
// Post: munta una trama amb l'adre�a dest�, l'adre�a propia del robot i les dades.
// i l'envia. No es garanteix que el dest� la rebi. El primer byte de l'array Data ha de valdre 0xff, ja que aix�
// no es confondr� amb les trames de telecontrol (API Remota) .
// MODE REMOT: No permesa

int LS_COM_IsBroadcastFrame(unsigned char Destination[COM_SHORT_MAC_SIZE]); 
// Post: Retorna cert si l'adre�a continguda en Destination �s una adre�a de de Broadcasd
// MODE REMOT: No permesa

void LS_COM_SendBroadcastFrame(unsigned char Data[COM_FRAME_SIZE]);
// Post: munta una trama amb l'adre�a propia del robot i les dades.
// i l'envia per broadcast. No es garanteix que cap dest� la rebi.
// MODE REMOT: No permesa

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� del moviment del robot
// Prefix    : LS_MT
// Aquestes rutines permeten moure el robot amb els motors esquerra i dreta
// aix� com l'enregistrament de la posici� aproximada. Els motors es poden aturar per temps
// o per qu� alguna de les entades STOP (de 0 a 7) esdeve un "1" (m�s de 2.3V).
// El valor dels senyals d'stop es pot consultar en qualsevol moment, gr�cies a la
// rutina LS_MT_IsStop(int Stop);
//
//----------------------------------------CONSTANTS----------------
//

#define MT_STOP_0				(1<<0)
#define MT_STOP_1				(1<<1)
#define MT_STOP_2				(1<<2)
#define MT_STOP_3				(1<<3)
#define MT_STOP_4				(1<<4)
#define MT_STOP_5				(1<<5)
#define MT_STOP_6				(1<<6)
#define MT_STOP_7				(1<<7)
//
//------------------------------END-------CONSTANTS----------------
//
unsigned int LS_MT_Lineal(unsigned int Time, int Speed, int StopBits, int * StopReasons);
  // Pre : Time, en milisegons >= 0. Speed: [-100,100]
  // StopBits: M�scara amb MT_STOP_0, MT_STOP_1, ..., MT_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker es mogui cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les m�scares M_STOPi 
  // esdevingui cert. 
  // Si milisegons == 0, Time indefnit, nom�s s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // MotiuStop retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // Velocitats menors de 20 (en m�dul) no mouen el robot llevat que la bateria estigui
  // a plena carrega. Cal evitar-les
  // MODE REMOT: Permesa

unsigned int LS_MT_TurnRight(unsigned int Time, int Speed, unsigned int Radius, int StopBits, int * StopReasons);
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: M�scara amb MT_STOP_0, MT_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a dreta cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les m�scares M_STOPi 
  // esdevingui cert. 
  // Si el radi �s menor de 10, el robot gira sobre si mateix
  // Si milisegons == 0, Time indefnit, nom�s s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores
  // Velocitats menors de 20 (en m�dul) no mouen el robot llevat que la bateria estigui
  // a plena carrega. Cal evitar-les
  // MODE REMOT: Permesa

unsigned int LS_MT_TurnLeft(unsigned int Time, int Speed, unsigned int Radius, int StopBits, int * StopReasons);
  // Pre : Time, en milisegons, >=0. Speed: [-100,100], Radius en cm (sempre positiu)
  // StopBits: M�scara amb M_STOP_0, M_STOP_1, ..., M_STOP_7, que indica quins senyals d'stop
  // faran aturar el motor abans de Time.
  // Post: 
  // Fa que LSMaker giri a esquerra cap endavant (Speeds positives) o cap endarrera (Speeds negatives)
  // durant el Time especificat o fins que un dels senyals d'stop habilitats amb les m�scares M_STOPi 
  // esdevingui cert. 
  // Si el radi �s menor de 10, el robot gira sobre si mateix
  // Si milisegons == 0, Time indefnit, nom�s s'atura per StopBits
  // Es retorna el nombre de milisegons que finalment ha executat
  // StopReason retorna per referencia el o els M_STOPi del motiu d'aturada (StopBits)
  // El Radius es pren des del centre de l'eix que uneix les dues rodes tractores
  // Velocitats menors de 20 (en m�dul) no mouen el robot llevat que la bateria estigui
  // a plena carrega. Cal evitar-les
  // MODE REMOT: Permesa

void LS_MT_SetNivellStopBits(unsigned char Nivell);
// Pre : Nivell pot valer 1 o 0
// Post: Defineix el valor dels senyals d'stop que fan aturar les rutines de moviment. 
// Si nivell val 1, els motors s'aturen quan els senyals d'STOPi valen 1 (3.3V) o b� zero vols
// si Nivell �s zero
// MODE Remot: Permesa

unsigned int LS_MT_GetTimeFromDistance(int Distance, int Speed);
  // Pre: Distance en cent�metres i positiu.
  // Pre: Speed entre -100 i 100, per� valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Lineal per 
  // tal que el robot avan�i el nombre de cent�metres expressats a Distance, a la velocitat
  // expressada per Speed. El punt �ptim �s dona quan la tensi� de bateria est� entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
// MODE Remot: Permesa

unsigned int LS_MT_GetTimeFromAngle(int Angle, int Speed);
  // Pre: Angle en graus, de 0 a 360.
  // Pre: Speed entre -100 i 100, per� valors per sota de 20 retornen zero
  // Post: Retorna el nombre de milisegons que cal donar a la rutina LS_MT_Turn Left o Right per 
  // tal que el robot giri el nombre de graus expressats a Angle, a la velocitat
  // expressada per Speed. El punt �ptim �s dona quan la tensi� de bateria est� entre 7.2 i 7.1,
  // fora d'aquest marges, l'error pot ser del 10%
  // Nom�s es correcte si el radi de gir �s menor de 10
// MODE Remot: Permesa

void LS_MT_InitTrack ();
// Post: A partir d'aquesta crida, l'exectiu comen�a a calcular la posici� actual en funci� de les ordres rebudes
// (LS_MT_Lineal, LS_MT_TurnLeft, LS_MT_TurnRight)
// La posici� actual es memoritza cada segon i es pot baixar al PC amb el cable USB i el programa LSLoader
// MODE Remot: Permesa

void LS_MT_EndTrack ();
// Post: Finalitza el seguiment de la posici� 
// MODE Remot: Permesa

void LS_MT_GetTrack (int *x, int *y);
// Post: Retorna la posici� actual de tracking, tenint en compte que despr�s de LS_InitTrack la posici� 
// es 0,0,0 i la direcci� coincideix amb l'eix de les x
// MODE Remot: Permesa

void LS_MT_GetDireccio (int *dx, int *dy);
// Post: Retorna les components x, y del vector direccio (unitari) multiplicats per 1000
// la posici� actual de tracking, tenint en compte que despr�s de LS_InitTrack la posici� 
// es 0,0 i el vector director �s 1,0
// MODE Remot: Permesa


int LS_MT_IsStop(unsigned int StopBits);
// Pre: StopBits: M�scara amb MT_STOP_0, MT_STOP_1, MT_STOP_7, que indica quins senyals d'stop es vol consultar
// Post: retorna cert si algun dels senyals d'stop especificat a la m�scara StopBits �s cert en l'actualitat.
// MODE Remot: Permesa

void LS_MT_SetServo(int NumServo, int Posicio);
// Pre: Num Servo val 2 o 3 
// Pre: Posicio  es un numero entre 0 i 100
// Post: Genera un pols repetitiu que val 1ms si Pos val 0 i 2 ms si pols val 1
// i entremig, pren valors de forma proporcional. Aquest pols apareix en els pins PWM2 i PWM3 respectivament
// del connector CN5 a la placa de l'LSMaker
// Aquests valors (0,100) �s corresponen amb les posicions extremes del servo (en teoria), les
// quals depenen de cada servo concret, ja que no tots tenen el mateix angle d'obertura
// Segurament pots anar m�s enll� d'aquests l�mits
// Els servomotors tenen sempre tres cables: la massa, l'alimentacion (5V) i el senyal de control (el pols 
// que obtenim en PWM2 i PWM3. Lamentablement, no hi ha cap consens pel que fa a l'ordre i colors dels
// cables. A la web hi ha moltes p�gines dedicades a explicar-ho, es troba amb "servo motor pinout".

unsigned int LS_MT_GetPosServo(int NumServo);
// Pre: Num Servo val 2 o 3 
// Post: Retorina la posici� actual, fixada per LS_MT_SetServo

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� de la consoloa
// Prefix    : LS_USB
//  Aquestes rutines simulen el comportament de les rutine est�ndards d'entrada-sortida, com
// ara getc, scanf, printf, putc, etc. La consola del LSMaker es el programa LSLoader, el qual,
// si es connecta l'LSMaker a un PC mitjan�ant el cable USB, envia el que s'escriu pel teclat cap
// a les rutines LS_USB_sscanf i LS_USB_getc i el resultat de les rutines LS_USB_printf i 
// LS_USB_putc apareix a la finestra central del programa LSLoader
// 

int LS_USB_CharAvailable(void);
// Post: retorna el nombre de caracters que ha rebut l'LSMaker i que encara no han estat
// processats per cap rutina
// MODE REMOT: No permesa

char LS_USB_getc(void);
// Post: retorna un car�cter que ha rebut l'LSMaker. Si no hi ha cap car�cter, aquesta rutina 
// s'espera fins que arribi. 
// MODE REMOT: mapeja a getc seguit de fflush

void LS_USB_putc(char c); 
// Post: Envia aquest car�cter cap al PC
// MODE REMOT: mapeja a putc

unsigned int LS_USB_gets(char *buffer, ...) ;
// Post: Processa el flux de car�cters d'entrada fins que trobi un '\r' (tecla enter) o un '\n' 
// (final de linia oficial) i ho diposita en l'array buffer. Llavors, podeu utilitzar la funci�
// sscanf(bufer, ...) per a formatejar la informaci� d'entrad a la vostra voluntat, exactament igual 
// que ho fa la rutina estandard scanf. Vegeu la documentaci� d'aquesta rutina en els llibres
// de text del llenguatge C o en les innumerables refer�ncies que trobareu per internet i vigila
// de no oblidar el & al davant de les variables que no siguin arrays.
// MODE REMOT: mapeja a gets seguit de fflush

unsigned int LS_USB_printf(const char *, ...);
// Post: Formateja els par�metres exactament igual que ho fa la rutina estandard printf i envia el 
// resultat cap el PC. Vegeu les immmenses posibilitats que ofereix la funci� printf en els llibres
// de text del llenguatge C o en les innumerables refer�ncies que trobareu per internet.
// MODE REMOT: mapeja a printf

//-----------------------------------------------------------------------------------------
// Descripci�: Gesti� de l'acceler�metre MMA75660
// Prefix    : LS_ACC
// Aquest circuit integrat cont� tres acceleromtres, (XYZ) orientats de forma que l'eix X correspon
// amb l'ex longitudinal del robot (el m�s llarg, per entendre'ns), l'eix Y �s l'eix tranversal 
// i l'eix Z �s l'exi perpendicular al pla de la placa de circuit impr�s.
// El criteri de signes �s el seguent:
// Eix X: Positiu si capbu�a i negatiu si s'aixeca el morro (el morro �s el cant� oposat al LCD)
// Eix Y: Negatiu si vira cap a la dreta i positiu cap a l'esquerra
// Eix Z: Positiu si el robot est� en la seva posici� normal o negatiu si est� panxa enlaire
//
// L'acceler�metre �s sensible a l'acceleraci� gravitatoria i a l'acceleraci� produida per un
// canvi en la velocitat. Aix�, si el robot no presenta moviment angular en cap dels seus eixos,
// el resultat que obtenim s�n les components del camp gravitatori i, despr�s d'aplicar l'arc cosinus,
// aquestes rutines calculen la inclinaci� del LSMaker.
//
// Val a dir que la mesura no es molt precissa ni gaire ressolutiva
//
// A m�s, el circuit integrat MMA75660 ofereix la posibilitat de detectar TAPs i SHAKEs. Un tap �s 
// un canvi abrupte per� petit, com el que es pot provocar si donem petits cops a l'estructura del
// robot. En canvi, un shake �s una saccejada, una acceleraci� superior a 1.3 vegades 
// l'acceleraci� gravitat�ria
//
// La sensibilitat del TAP es pot modificar a dins de la rutina AcInit, per� cal llegir el manual del
// MMA7660 (vegeu la carpeta DS Interessants). Tal com est� ara, nom�s �s sensible a l'eix Z, per� amb
// un lleuger copet a l'estructura del LSMaker ja n'hi ha prou per a detectar-lo. Evidentment,
// nom�s es pot treballar amb aquesta funcionalitat quan l'LSMaker est� aturat, ja que quan est� en marxa,
// la vibraci� dels motors i de les orugues l'activa constantment.
//
///
//-------------------------OPERACIONS-BASIQUES---------------------

int LS_ACC_Tap(void);
// Post: retorna cert si ha detectat un TAP. 
// La lectura �s destructiva
// MODE REMOT: Permesa

int LS_ACC_Shake(void);
// �s: funci� booleana de detecci� de Shake
// La lectura �s destructiva
// MODE REMOT: Permesa

void LS_ACC_GetPosXYZf(float *X, float *Y, float *Z);
// Post: XYZ valen de 90 a -90, per� fent una regla de tres entre la projecci� del vector 
// d'atracci� gravitatoria est�tica (que l'acceler�metre dona entre 21 i -21) i 90 graus.
// �s a dir X = Acceler�metreX * 90 / 21. Obviament, aix� NO es l'angle, si no el cosinus
// de l'angle. La conversi� seria acos( X / 90.0) i ull amb el signe. Per� l'arccosinus �s
// cost�s des del punt de vista computacional i com que l'acceler�metre no es gaire prec�s,
// creiem que l'aproximaci� de l'arccosinus a una recta ja �s suficient per als nostres prop�sits.
// Cal tenir en compte que el que m�s �s interessa �s la mesura relativa de l'angle i els canvis
// en l'angle, sobretot per detectat canvis de pendent.
// ATENCI�: Hi pot haver diferencies entre diferents acceler�metres, per la qual cosa �s 
// possible que retorni un angle lleugerament superior a 90 en algun del seus eixos. Es cosa teva
// control.lar aquests l�mits

#include "./RAW_Source/EEtEprom.h"






