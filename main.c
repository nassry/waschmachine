//************************************************
//Deklaration
//************************************************

#include <xc866.h>

//ADC-Funktionen:

extern void adc_init(void);
extern unsigned int adc_in(unsigned char kanal);

//LCD-Funktionen:

extern void lcd_init(void);			// Init LCD
extern void lcd_clr(void);	 		// LCD Löschen 
extern void lcd_curs(char);			// Cursor setzen
extern void lcd_str(char *ptr); // LCD String
extern void asc_out(char);			// ASC Funktion

//Eigene Funktionen:
void init (void);
void zyklus1 (void);
void zyklus2 (void) ;

//Variablen:

sbit S1 = P3_DATA^0;    // (Schalter)Hauptschalter
sbit S2 = P3_DATA^1;    // (Sensor)Temperatur Schalter
sbit S3 = P3_DATA^2;    // (Sensor)Fühlstand erreicht
sbit S4 = P3_DATA^3;    // (Sensor)Füllstand überschritten
sbit S5 = P3_DATA^4;    // (Signal) benutzer Anwahl 60 grad
sbit S6 = P3_DATA^5;    // (Signal) benutzer Anwahl 90 grad

unsigned int Y;         // (Steuersignal) Wasserzulauf
unsigned int H;         // (Steuersignal) Heizung
unsigned int M;         // (Steuersignal) Motor

//(Main)*****

void main (void)
{
	init ();
	while(1)
	{
				print();
		start = 1;
		while (temperatur_500 >= 300)
		{
			while (temperatur_500 >= 270)
			{
				print ();
				start = 0;
			}
		}
	  P0_DATA = 0x00;
	  P1_DATA = 0x00;
	  P3_DATA = 0x00;
		zyklus1();
		zyklus2();
	}
	
}

//(Funktionen)*****

void zyklus1 (void)
{
	if (S3 == 1)
	{
	  {
		P1_DATA =0x00;              //Wasserzulauf (Y) schließen
		}
		if (S2 == 0)
				{
					if (S6 == 1)
					{
					P0_DATA =0x80;				// Heizung aus ; Motor ein;
					}
					else
					{
					P0_DATA =0xa0;				// Heizung ein ; Motor ein;
					}
				}
			else
				{
					if (S5 == 1)
					{
					P0_DATA =0x80;				// Heizung aus ; Motor ein;	
					}
					else
					{
					P0_DATA =0xa0;				// Heizung ein ; Motor ein;
					}
				}	
	}
	else
	  {
	  P1_DATA = 0x04;				      //Wasserzulauf (Y) öffnen
		}
} 

void init (void)
{
	P0_DIR = 0xFF;
	P1_DIR = 0xFF;
	P3_DIR = 0x00;
}
