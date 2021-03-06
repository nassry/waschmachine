//(Deklaration)*****

#include <xc866.h>
void init (void);
void zyklus1 (void);
void zyklus2 (void) ;
sbit S1 = P3_DATA^0;    // (Schalter)Hauptschalter
sbit S2 = P3_DATA^1;    // (Sensor)Temperatur Schalter
sbit S3 = P3_DATA^2;    // (Sensor)F?hlstand erreicht
sbit S4 = P3_DATA^3;    // (Sensor)F?llstand ?berschritten
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
		P1_DATA =0x00;              //Wasserzulauf (Y) schlie?en
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
	  P1_DATA = 0x04;				      //Wasserzulauf (Y) ?ffnen
		}
} 

void init (void)
{
	P0_DIR = 0xFF;
	P1_DIR = 0xFF;
	P3_DIR = 0x00;
}

  