//-----------------------------------------------------------------
//TAD         :Gestió de la memoria flash
//Autor       :FEC
//Descripció  :Grava i llegeix dades de la flash a l'estil de les
// funcions típiques de tractament de fitxers
//Prefix      :NV
//Data        :1-8-2010
//-----------------------------------------------------------------
//

//
//----------------------------------------CONSTANTS----------------
//
#define SIZE_NV_BLOCK_B		(1536) 		// Son 8 files de 64 instruccions de 3 bytes; 8*64*3 = 1536 bytes
#define SIZE_NV_BLOCK_W		(1024) 		// Son 8 files de 64 instruccions de 2 words = 1024 words
#define SIZE_NV_PAGE		64			// instruccions
#define SIZE_NV_PAGE_B		(64*3)		// 64 instruccions de tres bytes
#define SIZE_NV_PAGE_W		(64*2)		// 64 instruccions de dos words

#define MAX_BLOCS		1		// Els fitxers A i B ocupen 1*1536 = 1536  bytes
#define MAX_BLOCS_C		2		// El fitxer C (intern) ocupa 2 * 1536 = 3072 bytes
								// Això permet 10 minuts de recollida de coordenades de 5 bytes (X,Y,Z)
								// a ritme de 1 coordenada per segon (5x60x10)
#define TOP_FLASH		0xA800	// Aquest valor canvia si es munta un altre model de microprocessador 
								// i ha de ser una potencia de 1024
#define PM_INICI_A		(TOP_FLASH-2*(MAX_BLOCS*SIZE_NV_BLOCK_W)-MAX_BLOCS_C*SIZE_NV_BLOCK_W)
#define PM_INICI_B		(TOP_FLASH-  (MAX_BLOCS*SIZE_NV_BLOCK_W)-MAX_BLOCS_C*SIZE_NV_BLOCK_W)
#define PM_INICI_C		(TOP_FLASH-  						   MAX_BLOCS_C*SIZE_NV_BLOCK_W)
//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------VARIABLES----------------
//

struct FitxerFlash {	
	unsigned char MaxBlocs;	// Tamany màxim, en nombre de blocks de 192 bytes que pot
							// tenir el fitxer
	unsigned char EEPointer;  	// Posició de l'EEPROM on hi ha el punter al final del fitxer
	unsigned int  Pointer;		// Punter actual del fitxer
	unsigned int  PMInici;		// Inici del fitxer a la memòria flash (<<8)
	unsigned char Mode;
	unsigned char Open;			// Flag per indicar l'estat d'obertura
	unsigned int  Size; 		// Tamany actual del fitxer
	unsigned char Buffer[SIZE_NV_PAGE_B];	// Buffer de maniobra
};
//
//------------------------------END-------VARIABLES----------------
//


//
//----------------------------------------PÚBLIQUES----------------
//
void NvInit(void);

void NVOpen(struct FitxerFlash *F, unsigned char Mode, unsigned char MaxBlocs, unsigned char EEPointer, unsigned int PMInici);
// Pre : Mode pot ser 'a', 'w' o 'r'. No es comprova el valor de la resta de paràmetres. 
// Post: Obre aquest fitxer i el prepara per al seu tractament.

void NVClose(struct FitxerFlash *F);

void NVReopen(struct FitxerFlash *F);

int NVRead(struct FitxerFlash *F, unsigned char *Buffer, unsigned int len);

int NVWrite(struct FitxerFlash *F, char *buffer, unsigned int length);
// Post: Escriu el nombre de caracters del buffer (length) en el fitxer
// La seguent escriptura es farà a partir d'on hagi acabat aquesta.
// Retorna el nombre de caracters que ha escriut realment o EOF si ha arribat 
// al límit final del fitxer (3840)
// Retorna INVALID_MODE si el fitxer s'ha obert amb "r"

int NVEof(struct FitxerFlash *F);
// Post: retorna cert si el punter intern del fitxer ha arribat al final,
// tant per lectura com per escriptura.

int NVSize(struct FitxerFlash *F);
// Post: Retorna el nombre de caràcters que té el fitxer


void NvInitTrack(void);
// Post: Obre i inicialitza el fitxer de tracking

void NvEndTrack(void);
// Post: tanca i grava el fitxer de tracking a la flash

void NvSetTrack(int x, int y);
// Pre : NvInitTrack
// Post: Grava aquestes coordenades en el fitxer de track

void NvResetTrack(void); 
//Post: Prepara el fitxer de track per a la lectura. Només per a fer proves

int NvGetTrack(int *x, int *y);
// Pre: has fet un  init track i un reset track
// POst: va omplint sequencialment les X Y capturades i les components del vector
// direcció unitari. S'enten que una parellla és X,Y i la següent parella és Dx,Dy (multiplicades per 1000)
// Retorna 0 si to va bé
// o -1 si ha arribat a final de fitxer.
