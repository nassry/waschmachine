//************************************************
//Declaration
//************************************************

#include <xc866.h>

//ADC-Funktionen:

extern void adc_init(void);
extern unsigned int adc_in(unsigned char kanal);

//Eigene Funktionen:

void init (void);
void wasser_einschalten (void);
void wasser_ausschalten (void);

//Variablen:

unsigned int wasserzulauf;				//Wasserzulauf
sbit S3 = P3_DATA^3;							//S3 Wasserpegel vom P3.3


//************************************************
//Main
//************************************************

void main (void)
{
	init ();
	while(1)
	{
		while (S3 != 1)
		{
			wasser_einschalten();
		}
		wasser_ausschalten();
		}
}

//************************************************
//Function
//************************************************

void init (void)
{
	P3_DIR = 0x00;					//P3.1 bis P3.7 als Eingänge
	P1_DIR = 0xFF;					//P1.2 bis P1.4 als Ausgänge
	P0_DIR = 0xFF;					//P0.3 und P0.4 als Ausgänge
	TMOD = 0x11;						//Timer 0 initialisert
}

void wasser_einschalten (void)
{
	P1_DATA = 0x04;
}

void wasser_ausschalten (void)
{
	P1_DATA = 0x00;
}
void warten (void)
{
	TF0 = 0;								//Timer 0 Flag auf 0 gesetzt
	TL0 = 0xa2;							//
	TH0 = 0x32;
	TR0 = 1;
	while (TF0 == 0);
	TR0 = 0;
}