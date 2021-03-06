//************************************************
//Declaration
//************************************************

#include <xc866.h>

//ADC-Funktionen:

extern void adc_init(void);
extern unsigned int adc_in(unsigned char kanal);

//Eigene Funktionen:

void init (void);
void eine_minute (void);
void motor_einschalten (void);
void motor_ausschalten (void);
void heizung_einschalten (void);
void heizung_ausschalten (void);

//Variablen:

unsigned int motor;				//Motor als Variablen
unsigned int i;						//i loop


//************************************************
//Main
//************************************************

void main (void)
{
	init ();
	while(1)
	{
		motor_einschalten();
		heizung_einschalten();
		eine_minute();
		motor_ausschalten();
		heizung_ausschalten();
		eine_minute();
	}
}

//************************************************
//Function
//************************************************

void init (void)
{
	P3_DIR = 0x00;					//P3.1 bis P3.7 als Eing?nge
	P1_DIR = 0xFF;					//P1.2 bis P1.4 als Ausg?nge
	P0_DIR = 0xFF;					//P0.3 und P0.4 als Ausg?nge
	TMOD = 0x11;						//Timer 0 initialisiert
}

void motor_einschalten (void)
{
	P0_DATA = 0x08;
}

void motor_ausschalten (void)
{
	P0_DATA = 0x00;
}
void heizung_einschalten (void)
{
	P1_DATA = 0x08;
}

void heizung_ausschalten (void)
{
	P1_DATA = 0x00;
}

void eine_minute (void)
{
	//*****
	// Timertakt 12MHz
	// 60 Sek / 5.461 mSek = 10.986,99 Flags (10.987)
	// bzw. 1 sec = 186
	//*****
	for (i = 1; i < 186; i++)
		{
			TF0 = 0;								//Timer-Flag auf 0 - bitaddresiert
			TL0 = 0x00;							//Erste 8-binare - bitadresiert (start)
			TH0 = 0x00;							//Zweite 8-binare - bitadresiert
			TR0 = 1;								//Timer 0 starten - bitadresiert 
			while (TF0 == 0);
			TR0 = 0;								//Timer 0 stoppt - bitadressiert
		}
}