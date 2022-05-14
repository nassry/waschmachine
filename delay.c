//Declaration*******************
#include <xc866.h>
void init (void);
void warten (void);
char LED[4];
unsigned int i;


//Main**************************

void main (void)
{
	init ();
	LED[0] = P0_DATA;
	LED[1] = P1_DATA;
	LED[2] = P2_DATA;
	LED[3] = P3_DATA;
	while(1)
	{
		LED[3] = 0xFF;
		
		for (i = 0; i<10; i++)
		{
		warten ();
		}
		
		LED[3] = 0x00;
		
		for (i = 0; i<10; i++)
		{
		warten ();
		}
	}
}

//Function**********************
void init (void)
{
	P3_DIR = 0xFF;
	TMOD = 0x11;
}

// 2000/

void warten (void)
{
	TF0 = 0;								//Timer-Flag auf 0 - bitaddresiert
	TL0 = 0xa2;							//Erste 8-binare - bitadresiert (start)
	TH0 = 0x32;							//Zweite 8-binare - bitadresiert
	TR0 = 1;								//Timer 0 starten - bitadresiert 
	while (TF0 == 0);
	TR0 = 0;								//Timer 0 stoppt - bitadressiert
}