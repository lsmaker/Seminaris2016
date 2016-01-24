//-----------------------------------------------------------------
// TAD:Gestió dels senyals d'entrada/sortida
// Descripció:
//   Aquestes rutines permeten accedir als quatre ports d'entrada analògics
// anomentas AN_0, AN_1, AN_2 i AN_3 en els esquemes, que trobareu en els 
// connectors d'expansió.
//   També permet definir la direcció (entrada o sortida) dels ports 
// GPIO_0, GPIO_1, GPIO_2 i GPIO_3, que teniu a disposició en els connectors
// d'expansió
//   A més, amb aquestes rutines podreu:
//   a) Saber si s'ha premut o s'ha deixat anar el polsador de propòsit general
//      etiquetat com GpButton a la placa de circuit imprès
//   b) Governar els leds de la placa
//Prefix      :IO_
//Data        :Wednesday, June 30, 2010
//-----------------------------------------------------------------

//
//----------------------------------------CONSTANTS----------------
//
/* Definides en LSAPi.h
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
*/

//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------PÚBLIQUES----------------
//
void IoInit(void);
void IoMotor(void);

void IoSetDirection(int Port, int Dir);
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Dir val DIR_IN (entrada) o DIR_OUT sortida)
// Post: Defineix el sentit dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// Post: Per defecte, el valor que pren un port de sortida és 0 (0V)

void IoSetValue(int Port, int Value);
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 i GPIO_3; Value val 1 (3.3 V) o 0 (0 V)
// Post: Defineix el valor dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a sortides

int IoGetValue(int Port);
// Pre: Port val GPIO_0, GPIO_1, GPIO_2 o GPIO_3
// Post: Retorna el valor actual dels ports GPIO_0, GPIO_1, GPIO_2 i GPIO_3
// que s'hagin definit com a entrades. Retorna 1 si la tensió en el port
// és superior a 2.3V i 0 si la tensió del port és inferior a 0.9 V
// Altrament no es pot assegurar el valor

int IoGetAnalog(int Port); 
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor analògic actual (AN_0, AN_1, AN_2 o AN_3), sense cap mena de filtratge, de
// forma que si la tensió en el port és 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.

int IoGetAnalogFiltered(int Port); 
// Pre: Port val AN_0, AN_1, AN_2 o AN_3; 
// Post: retorna el valor analògic actual (AN_0, AN_1, AN_2 o AN_3), amb un filtratge, basat en un promig
// mòbil de quatre mostres a ritme de 10 ms per mostra.
// Igual que amb GetAnalog, si la tensió en el port és 3,3V, retorna 1023 i si val zero, retorna zero.
// Entremig es comporta de forma lineal.

int IoGetBatteryVoltage(void);
// Post: retorna un numero entre 0 i 1023, que indica el valor analògic del senyal BatSample

void IoPowerOff(void);
// Post: Apaga l'alimentació de la placa base.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int IoGpButtonPress(void);
// Post: Retorna cert si s'ha premut el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.

int IoGpButtonRelease(void);
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton, convenientment filtrat contra
// rebots. La lectura és destructiva.

int IoGpButton2Press(void);
// Post: Retorna cert si s'ha premut el polsador GpButton 2, convenientment filtrat contra
// rebots. La lectura és destructiva.

int IoGpButton2Release(void);
// Post: Retorna cert si s'ha deixat de premer el polsador GpButton 2, convenientment filtrat contra
// rebots. La lectura és destructiva.

void IoSetLedBeat(int Beat);
// Pre: Beat pot valer entre 0 (leds apagats) i > 40 (leds permanentment encesos)
// Post:Els valors intermedis (de 1 a 40) indiquen la frequencia de pulsació en dècimes d'Hz  
// Exemple: Beat = 20 vol dir 2Hz

void IoSincro(void);
// Post: reset del ritme de batec

void IoWait(unsigned int Micros);
// Pre : Micros és positiu
// Post: Espera el nombre de microsegons especificat. NO ES COOPERATIVA
// i per tant s'ha d'usar amb molta precaució

