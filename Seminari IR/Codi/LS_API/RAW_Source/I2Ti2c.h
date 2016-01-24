//-----------------------------------------------------------------
//TAD         :I2C del robot OMNIA
//Autor       :FEC
//Descripció  :Gestiona l'I2C del PIC24FJ64GA006
//Prefix      :i2
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------


//-------------------------------PUBLIQUES-------------------------
int i2Init(void);
// Post: retorna 0 si tot ok, 2 si no queden timers

void i2End(void);

int i2WriteData(char SlaveAddress, unsigned char Address, unsigned char Dada);
// Pre: Bytes >= 0;
// Pre: El bit m‚s baix de Address no t‚ informaci¢ (ser… read o write)
// Post: retorna 0 si tot OK 

int i2ReadData(char SlaveAddress, unsigned char Address, int Bytes, unsigned char *Dades);
// Pre: Bytes >= 0; Dades t‚ espai per a NumBytes
// Pre: El bit m‚s baix de Address no t‚ informaci¢ (ser… read o write)
// Post: retorna 0 si tot OK 

//--------------------------END--PUBLIQUES-------------------------

