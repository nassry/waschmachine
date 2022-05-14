//************************************************
//Declaration
//************************************************

#include <xc866.h>

//ADC-Funktionen:

//Eigene Funktionen:

void init (void);
void heizung_einschalten (void);
void heizung_ausschalten (void);
void regler (void);
sbit temp = P3_DATA^2;
sbit S5 = P3_DATA^5;
sbit S6 = P3_DATA^6;

//Variablen:

//************************************************
//Main
//************************************************

void main (void)
{
	init ();
	while(1)
	{
		regler();
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

void regler (void)
{
	if(temp == 0)
	{
		if(S6 == 1)									//60° Grad
		{
			heizung_ausschalten();
		}
		else
		{
			heizung_einschalten();
		}
	}
	else if(temp == 1)
	{
		if(S5 == 1)									//90° Grad
		{
		heizung_ausschalten();
		}
		else
		{
			heizung_einschalten();
		}
	}
	else
	{
		heizung_einschalten();
	}
}