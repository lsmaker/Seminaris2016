//-----------------------------------------------------------------
//TAD         :Gestió de la memoria flash
//Autor       :FEC
//Descripció  :Grava i llegeix dades de la flash a l'estil de les
// funcions típiques de tractament de fitxers
//Prefix      :NV
//Data        :1-8-2010
//-----------------------------------------------------------------
//
#include "../LSApi.h"
#include "EEtEprom.h"

//
//----------------------------------------CONSTANTS----------------
//
//
//------------------------------END-------CONSTANTS----------------
//

//
//----------------------------------------VARIABLES----------------
//
struct FitxerFlash Track;
struct Coordenada {
	int X, Y;
};

//
//------------------------------END-------VARIABLES----------------
//

//
//----------------------------------------PROTOTIPUS---------------
//
void NvEsborra(unsigned int PMInici, unsigned char Block);
unsigned int NvGetPointer(unsigned int EEPointer);
void NvSetPointer(unsigned int EEPointer, unsigned int Pointer);
void NvLlegeixPage(unsigned char *Buffer, unsigned int PMInici,  unsigned char Pagina);
void NvWritePage  (unsigned char *Buffer, unsigned int PMInici,  unsigned char Pagina);

//
//------------------------------END-------PROTOTIPUS---------------
//

//
//----------------------------------------PÚBLIQUES----------------
//
void NvInit(void) {
}

void NVOpen(struct FitxerFlash *F, unsigned char Mode, unsigned char MaxBlocs, unsigned char EEPointer, unsigned int PMInici) {
// Pre : Mode pot ser 'a', 'w' o 'r'. No es comprova el valor de la resta de paràmetres. 
// Post: Obre aquest fitxer i el prepara per al seu tractament.
	int i; unsigned char Block, Page;
	// Omplo l'estructura
	F->MaxBlocs = MaxBlocs; F->EEPointer = EEPointer, F->PMInici = PMInici; F->Mode = Mode; F->Open = 1;
	if (Mode == 'w') {	// Mode d'escriptura, tot esborrant el contingut previ
		// Esborro els blocs que formen el fitxer
		for (i = 0; i < MaxBlocs; i++) NvEsborra(PMInici, i);	// Aquesta maniobra bloqueja la CPU 4ms per cada bloc
		F->Pointer = F-> Size = 0; // I llestos
	} else 
	if (Mode == 'a') {  // Mode d'escriptura afegint al final.
		// Primer cal recordar en quin punt es troba el punter
		F->Pointer = F-> Size = NvGetPointer(F->EEPointer);
		// I omplir el buffer de memòria amb aquest block
		Page  = F->Pointer / SIZE_NV_PAGE_B;
		NvLlegeixPage(F->Buffer, F->PMInici, Page);
		// Esborro aquest bloc, ara que l'he salvat i la resta fins al tamany màxim
		for (i = Block; i < F->MaxBlocs; i++) NvEsborra(PMInici, i); // Aquesta maniobra bloqueja la CPU 4ms per cada bloc
	} else 
	if (Mode == 'r') {
		// Poca cosa cal fer
		NvLlegeixPage(F->Buffer, F->PMInici, 0);
		F->Pointer 	= 0;
		F->Size 	= NvGetPointer(F->EEPointer);
	} // Si no, no sé que serà
	// LS_USB_printf("Open (%c) PMInici= %x Size = %d\n", Mode, PMInici, F->Size);
}

void NVClose(struct FitxerFlash *F) {
	if (F->Open != 1) return; 	// per si de cas
	if (F->Mode == 'r') return; // Això si que és fàcil
	if (F->Size == 0)  return;  // i això també.
	// LS_USB_printf("Close (%c) Size = %d EEP=%d\n", F->Mode, F->Size, F->EEPointer);
	// Cal gravar en la flash el que hi ha en el buffer.
	NvWritePage(F->Buffer, F->PMInici, (F->Size-1) / SIZE_NV_PAGE_B); // L'escrivim
	// I en l'eprom cal recordar la posició final del punter 
	NvSetPointer(F->EEPointer, F->Size);
	F->Open = 0;	// Tancat per vacances
}

void NVReopen(struct FitxerFlash *F) { F->Pointer = 0; }

int NVRead(struct FitxerFlash *F, unsigned char *Buffer, unsigned int len) {
// Retorna el nombre de bytes que ha llegit o EOF si s'ha arribat a final de fitxer
	unsigned int L, i, j, k;
	unsigned int Page  = F->Pointer / SIZE_NV_PAGE_B;
	// Cas estupid, volem llegir en un fitxer obert en mode escriptura
	if (F->Mode != 'r') return NVOL_INVALID_MODE;
	// Cas trivial: hem arribat a final de fitxer
	if (NVEof(F)) return EOF;
	// Calculo el nombre de caracters que realment puc llegir
	if (F->Pointer+len > F->MaxBlocs*SIZE_NV_BLOCK_B) {
		// Ajusto L
		L = F->MaxBlocs*SIZE_NV_BLOCK_B - F->Pointer;
	} else L = len;
	for (k = 0, i = F->Pointer % SIZE_NV_PAGE_B, j = L; j; j--, k++) {
		//LS_USB_printf("(%d %d %x)", F->Pointer, i, F->Buffer[i]);
		Buffer[k] = F->Buffer[i]; 
		i++; F->Pointer++;
		if (i == SIZE_NV_PAGE_B) { // Buffer de maniobra exhaurit, cal reomplir
			//LS_USB_printf("Canvi Pagina = %d P = %d", Page, F->Pointer);
			NvLlegeixPage(F->Buffer, F->PMInici, Page+1); // La primera ja està carregada
			++Page;
		}
	}
	return L;
}

int NVWrite(struct FitxerFlash *F, char *buffer, unsigned int length) {
// Post: Escriu el nombre de caracters del buffer (length) en el fitxer
// La seguent escriptura es farà a partir d'on hagi acabat aquesta.
// Retorna el nombre de caracters que ha escriut realment o EOF si ha arribat 
// al límit final del fitxer (3840)
// Retorna INVALID_MODE si el fitxer s'ha obert amb "r"
	volatile int L, i, j;
	unsigned int Page  = F->Pointer / SIZE_NV_PAGE_B;
	// Cas estupid, volem escriure en un fitxer obert en mode lectura
	if (F->Mode == 'r') return NVOL_INVALID_MODE;
	// Cas trivial: hem arribat al final de fitxer
	L = i = 0; // Inici del buffer
	if (NVEof(F)) return EOF;
	while (length) {
		for (j = F->Pointer % SIZE_NV_PAGE_B; (j < SIZE_NV_PAGE_B) && length; j++, i++) {
			++L; --length; F->Pointer++; F->Size++;
			F->Buffer[j] = buffer[i];
			if (NVEof(F)) return EOF;
		}
		// o bé hem emplenat el buffer o bé hem acabat.
		if (j == SIZE_NV_PAGE_B) {	// Hem emplenat el buffer, 
			NvWritePage(F->Buffer, F->PMInici, Page); // L'escrivim
			// LS_USB_printf("WR Pointer=%d Page =%d NVMCOM=%x ", F->Pointer, Page, NVMCON);
			++Page;
		}
	}
	return L;
}

int NVEof(struct FitxerFlash *F) {
// Post: retorna cert si el punter intern del fitxer ha arribat al final,
	if (F->Mode == 'r') {
		return F->Pointer >= F-> Size;
	} 
	return F->Pointer >= F->MaxBlocs*SIZE_NV_BLOCK_B;
}

int NVSize(struct FitxerFlash *F) { return F->Size; }
// Post: Retorna el nombre de caràcters que té el fitxer

void NvInitTrack(void) {
// Post: Obre i inicialitza el fitxer de tracking
	NVOpen(&Track, 'w', MAX_BLOCS_C, EE_PUNTER_C, PM_INICI_C);
}

void NvSetTrack(int x, int y) {
// Pre : NvInitTrack
// Post: Grava aquestes coordenades en el fitxer de track
	struct Coordenada C; 
	C.X = x; C.Y = y; 
	if (!NVEof(&Track)) NVWrite(&Track, (char *)&C, sizeof (struct Coordenada));
}

void NvEndTrack(void) { 	NVClose(&Track); }

void NvResetTrack(void) { 
// Post: Prepara el fitxer de track per a la lectura. Només per a fer proves, es millor 
// veure el fitxer usant el LSLoader
	NVClose(&Track);
	NVOpen(&Track, 'r', MAX_BLOCS_C, EE_PUNTER_C, PM_INICI_C);
}

int NvGetTrack(int *x, int *y) {
// Pre: has fet un  init track i un reset track
// POst: va omplint sequencialment les X Y capturades. Retorna 0 si to va bé
// o -1 si ha arribat a final de fitxer.
	struct Coordenada C; 
	if (!NVEof(&Track)) {
		NVRead(&Track, (unsigned char *)&C, sizeof (struct Coordenada));
		*x = C.X; *y = C.Y;;	
		return 0;	
	}
	return -1;
}
//
//-----------------------------------END--PÚBLIQUES----------------
//
static	unsigned int PMFinal; 
static	unsigned int PMFinalH = 0; 
//-----------------------------------RUTINES DEFINIDES EN ASM antigament a memory.s
unsigned long ReadLatch(unsigned int, unsigned int);
void Erase(unsigned int, unsigned int, unsigned int);
void WriteLatch(unsigned int, unsigned int, unsigned int, unsigned int);
void WriteMem(unsigned int);
//------------------------------END--RUTINES DEFINIDES EN ASM antigament a memory.s
void NvEsborra(unsigned int PMInici, unsigned char Block) {
	PMFinal = PMInici+Block*SIZE_NV_BLOCK_W; 
	//LS_USB_printf("Erase PM=%x, Block= %d\n\r", PMFinal, Block);
	Erase(PMFinalH, PMFinal, 0x4042);
}

unsigned int NvGetPointer(unsigned int EEPointer) {
	unsigned int t; 
	EERead(EEPointer, (unsigned char *)&t, 2);
	return t;
}

void NvSetPointer(unsigned int EEPointer, unsigned int Pointer) {
	EEWrite(EEPointer, Pointer);
	EEWrite(EEPointer+1, Pointer>>8);
}


void NvLlegeixPage(unsigned char *Buffer, unsigned int PMInici, unsigned char Pagina) {
	unsigned long Tmp; int i, j; 
	PMFinal = PMInici+Pagina*SIZE_NV_PAGE_W; 
	for (i = j = 0; j < SIZE_NV_PAGE; i+=3, j++) {
		Tmp = ReadLatch(PMFinalH, PMFinal);
		Buffer[i] 	= Tmp;
		Buffer[i+1] = Tmp>>8;
		Buffer[i+2] = Tmp>>16;	
		PMFinal+=2;
	}
}

void NvWritePage(unsigned char *Buffer, unsigned int PMInici, unsigned char Pagina) {
	int i, c;
	unsigned int B0, B1;
	PMFinal = PMInici+Pagina*SIZE_NV_PAGE_W; 
	//LS_USB_printf("Write I  PAge PM=%x, Pagina = %d\n\r", PMFinal, Pagina);
	for (i = 0, c = 0; c < SIZE_NV_PAGE; i+=3, c++) {
		B0 = Buffer[i];
		B0 = ((Buffer[i+1]) << 8) | B0;
		B1 = Buffer[i+2];
	 	WriteLatch(PMFinalH, PMFinal,B1,B0);
		PMFinal+=2;
	}
	//LS_USB_printf("Write O PAge PM=%x, Pagina = %d\n\r", PMFinal, Pagina);

	WriteMem(0x4001);	// Ara comença l'escriptura
}

