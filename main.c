Skip to content
Search or jump to…
Pull requests
Issues
Marketplace
Explore
 
@RaedAlhardan 
nassry
/
waschmachine
Public
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
waschmachine/Waschmaschine.c
@RaedAlhardan
RaedAlhardan Add files via upload
Latest commit 1c4a6f1 now
 History
 1 contributor
79 lines (69 sloc)  1.43 KB
   
//(Deklaration)*****

#include <xc866.h>
void init (void);
void zyklus1 (void);
void zyklus2 (void) ;
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

  
© 2022 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
