//-----------------------------------------------------------------------------------------
// Descripció: Gestió de l'acceleròmetre MMA75660
// Prefix    : LS_ACC
// Aquest circuit integrat conté tres acceleromtres, (XYZ) orientats de forma que l'eix X correspon
// amb l'ex longitudinal del robot (el més llarg, per entendre'ns), l'eix Y és l'eix tranversal 
// i l'eix Z és l'exi perpendicular al pla de la placa de circuit imprés.
// El criteri de signes és el seguent:
// Eix X: Positiu si capbuça i negatiu si s'aixeca el morro (el morro és el cantó oposat al LCD)
// Eix Y: Negatiu si vira cap a la dreta i positiu cap a l'esquerra
// Eix Z: Positiu si el robot està en la seva posició normal o negatiu si està panxa enlaire
//
// L'acceleròmetre és sensible a l'acceleració gravitatoria i a l'acceleració produida per un
// canvi en la velocitat. Així, si el robot no presenta moviment angular en cap dels seus eixos,
// el resultat que obtenim són les components del camp gravitatori i, després d'aplicar l'arc cosinus,
// aquestes rutines calculen la inclinació del LSMaker.
//
// Val a dir que la mesura no es molt precissa ni gaire ressolutiva
//
// A més, el circuit integrat MMA75660 ofereix la posibilitat de detectar TAPs i SHAKEs. Un tap és 
// un canvi abrupte però petit, com el que es pot provocar si donem petits cops a l'estructura del
// robot. En canvi, un shake és una saccejada, una acceleració important.
// 
// La sensibilitat del TAP es pot modificar a dins de la rutina AcInit, però cal llegir el manual del
// MMA7660 (vegeu la carpeta DS Interessants). Tal com està ara, només és sensible a l'eix Z, però amb
// un lleuger copet a l'estructura del LSMaker ja n'hi ha prou per a detectar-lo. Evidentment,
// només es pot treballar amb aquesta funcionalitat quan l'LSMaker està aturat, ja que quan està en marxa,
// la vibració dels motors i de les orugues l'activa constantment.
//
//-------------------------OPERACIONS-BASIQUES---------------------
#include "../LSApi.h"

int LS_ACC_Tap(void) {
// Post: retorna cert si ha detectat un TAP en l'exi de les Z
// La lectura és destructiva
	LS_Executiu();
	return AcGetTap();
}
	

int LS_ACC_Shake(void) {
// La lectura és destructiva
// Ús: funció booleana de detecció de Shake
	LS_Executiu();
	return AcGetShake();
}


void LS_ACC_GetPosXYZf(float *X, float *Y, float *Z) {
// Post: XYZ valen de 90 a -90, però fent una regla de tres entre la projecció del vector 
// d'atracció gravitatoria estàtica (que l'acceleròmetre dona entre 21 i -21) i 90 graus.
// És a dir X = AcceleròmetreX * 90 / 21. Obviament, això NO es l'angle, si no el cosinus
// de l'angle. La conversió seria acos( X / 90.0) i ull amb el signe. Però l'arccosinus és
// costós des del punt de vista computacional i com que l'acceleròmetre no es gaire precís,
// creiem que l'aproximació de l'arccosinus a una recta ja és suficient per als nostres propòsits.
// Cal tenir en compte que el que més és interessa és la mesura relativa de l'angle i els canvis
// en l'angle, sobretot per detectat canvis de pendent.
	LS_Executiu();
	AcGetXYZ(X, Y, Z);
}

