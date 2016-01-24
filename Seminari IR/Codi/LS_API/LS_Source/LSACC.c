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
// robot. En canvi, un shake �s una saccejada, una acceleraci� important.
// 
// La sensibilitat del TAP es pot modificar a dins de la rutina AcInit, per� cal llegir el manual del
// MMA7660 (vegeu la carpeta DS Interessants). Tal com est� ara, nom�s �s sensible a l'eix Z, per� amb
// un lleuger copet a l'estructura del LSMaker ja n'hi ha prou per a detectar-lo. Evidentment,
// nom�s es pot treballar amb aquesta funcionalitat quan l'LSMaker est� aturat, ja que quan est� en marxa,
// la vibraci� dels motors i de les orugues l'activa constantment.
//
//-------------------------OPERACIONS-BASIQUES---------------------
#include "../LSApi.h"

int LS_ACC_Tap(void) {
// Post: retorna cert si ha detectat un TAP en l'exi de les Z
// La lectura �s destructiva
	LS_Executiu();
	return AcGetTap();
}
	

int LS_ACC_Shake(void) {
// La lectura �s destructiva
// �s: funci� booleana de detecci� de Shake
	LS_Executiu();
	return AcGetShake();
}


void LS_ACC_GetPosXYZf(float *X, float *Y, float *Z) {
// Post: XYZ valen de 90 a -90, per� fent una regla de tres entre la projecci� del vector 
// d'atracci� gravitatoria est�tica (que l'acceler�metre dona entre 21 i -21) i 90 graus.
// �s a dir X = Acceler�metreX * 90 / 21. Obviament, aix� NO es l'angle, si no el cosinus
// de l'angle. La conversi� seria acos( X / 90.0) i ull amb el signe. Per� l'arccosinus �s
// cost�s des del punt de vista computacional i com que l'acceler�metre no es gaire prec�s,
// creiem que l'aproximaci� de l'arccosinus a una recta ja �s suficient per als nostres prop�sits.
// Cal tenir en compte que el que m�s �s interessa �s la mesura relativa de l'angle i els canvis
// en l'angle, sobretot per detectat canvis de pendent.
	LS_Executiu();
	AcGetXYZ(X, Y, Z);
}

