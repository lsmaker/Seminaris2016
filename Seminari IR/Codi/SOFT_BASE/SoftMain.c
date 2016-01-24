//
//-------------------------PROGRAMA DE TEST DE l'LSMAKER--------------------
//
// Aquest és el programa que porta carregat "de fàbrica" l'LSMaker.
// Només fa que moure els motors endavant i endarrera i enviar una
// trama per l'enllaç de radiofreqüencia. Per la pantalla de l'LCD
// mostra la tensió de la bateria, l'adreça mac, el nom d'usuari 
// que hagis configurat amb el programa LSLoader, la inclinació XYZ i
// el valor dels senyals STOP0, STOP1 i STOP2 (Blanc o Negre)
//
// Per crear la teva propia aplicació, només cal que baixis el directori
// LS_API i el directori Proves i ja pots engegar el MPLAB i obrir el projecte
// TestMain. El projecte ja te tots els tads i l'API ben configurada, només
// cal que afegeixis els teus propis TADS i la teva aplicació en lloc de 
// la "ProvaDeCalibracio"
// 

//------------------------------LLIBRERIES-------------------------
#include "../LS_API/LSApi.h"
//-------------------------END--LLIBRERIES-------------------------

//------------------------------CONSTANTS--------------------------
// Opcions de configuració del microprocessador PIC16F64
_CONFIG2(OSCIOFNC_ON);
_CONFIG1(JTAGEN_OFF & ICS_PGx1);
//-------------------------END--CONSTANTS--------------------------

//------------------------------TIPUS------------------------------
//-------------------------END--TIPUS------------------------------


#define LLINDAR 1000
#define TEMPS_ENDAVANT 100

int main(void) {

    //Declaració de variables
    unsigned char bFlag = 0;
    unsigned int nTime30 = 0;
    unsigned int nTime60 = 0;
    unsigned int  nTime150 = 0;
    int nStops = 0;

    //Inicialització del robot
    LS_Init();

    //Pintem missatge de benvinguda per la pantalla
    LS_LCD_Clear();
    LS_LCD_Printf(0,0,"%s","Prem GPButton...");

    nTime30 = LS_MT_GetTimeFromAngle(30,100);
    nTime60 = LS_MT_GetTimeFromAngle(60,100);
    nTime150 = LS_MT_GetTimeFromAngle(150,100);


    while (1) {

        //Mirem si ens han premut GPButton
        if(LS_IO_GpButtonPress()){
            bFlag = 1;
            LS_LCD_Clear();
            LS_LCD_Printf(0,0,"%s","   A  correr!   ");
        }


        //Comprovem si toca moure'ns
        if(bFlag){

            //Movem el robot endavant
            while(TEMPS_ENDAVANT < LS_MT_Lineal(TEMPS_ENDAVANT,100,0,&nStops)){
                LS_Executiu();
            }

            //Mirem si hi ha obstacle
            if(LS_IO_GetAnalogFiltered(AN_0) <= LLINDAR){

                while(nTime30 < LS_MT_TurnLeft(nTime30,100,0,0,&nStops)){
                    LS_Executiu();
                }

                //Hi ha obstacle a l'esquerra?
                if(LS_IO_GetAnalogFiltered(AN_0) <= LLINDAR){

                    //Si hi ha obstacle a l'esquerra mirem a la dreta.
                    while(nTime60 < LS_MT_TurnRight(nTime60,100,0,0,&nStops)){
                        LS_Executiu();
                    }

                    //Hi ha obstacle a la dreta?
                    if(LS_IO_GetAnalogFiltered(AN_0) <= LLINDAR){

                        //Si hi ha obstacle a la dreta, girem 180
                        while(nTime150 < LS_MT_TurnRight(nTime150,100,0,0,&nStops)){
                            LS_Executiu();
                        }

                    }

                }


            }

        }

        LS_Executiu();


    }

    return 0;
}
