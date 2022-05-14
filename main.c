//************************************************
//Deklaration
//************************************************

#include <xc866.h>

//ADC-Funktionen:

extern void adc_init(void);
extern unsigned int adc_in(unsigned char kanal);

// LCD Funktionen
   extern void lcd_init(void);		      // Init LCD
   extern void lcd_clr(void);	 	     	  // LCD Löschen 
   extern void lcd_byte(char);  	     	// Ausgabe unsigned char  => 3 stellig
   extern void lcd_str(char *ptr);	    // Ausgabe String
   extern void lcd_curs(char);		      // Cursor setzen
   extern void asc_out(char);	   		    // Ausgabe eines ASCII Zeichens
   extern void lcd_int(unsigned int);	  // Ausgabe Int Wert => 4 stellig
	 
// Texte
	unsigned char Waus[]="Waschmaschine Aus";   	// LCD-Text Waschmaschine Aus
	 unsigned char Wan[]="Waschmaschine An";   	  // LCD-Text Waschmaschine An
	 unsigned char WS3[]="Wasserzulauf bis S3";   // LCD-Text Wasserzulauf bis S3
	 unsigned char Han[]="Heizung An";   	        // LCD-Text Heizung An
	 unsigned char Man[]="Motor An";   	          // LCD-Text Motor An

//Eigene Funktionen:

void init (void);
void zyklus1 (void);
void zyklus2 (void);
void teiler (void);							// Teilen in einzelne Ziffer
void print (void);							// Temperatur darstellen

//Variablen:

sbit S1 = P0_DATA^5;    // (Schalter)Hauptschalter - Interruptbetrieb EXT0
sbit S2 = P3_DATA^1;    // (Sensor)Temperatur Schalter
sbit S3 = P3_DATA^2;    // (Sensor)Fühlstand erreicht
sbit S4 = P3_DATA^3;    // (Sensor)Füllstand überschritten
sbit S5 = P3_DATA^4;    // (Signal) benutzer Anwahl 60 grad
sbit S6 = P3_DATA^5;    // (Signal) benutzer Anwahl 90 grad

unsigned int Y;         // (Steuersignal) Wasserzulauf
unsigned int H;         // (Steuersignal) Heizung
unsigned int M;         // (Steuersignal) Motor
unsigned int start;			// (Steuersignal) Start

unsigned long temperatur_8;				//temperatur 8_bit
unsigned long temperatur_10;			//temperatur 10_bit
unsigned long temperatur_500;			//temperatur 0-50,0°C
unsigned int H, Z, E, temperatur;	// Speicher für H,Z,E

//(Main)*****

void main (void)
{
	init ();
	while(1)
	{
		if (S1==1)
		{	
			print();
			start = 1;
			while (temperatur_500 >= 300)
			{
				while (temperatur_500 >= 270)
				{
					print();
					start = 0;
				}
			}
			if (start == 1)
			{
			print ();
			zyklus1();
			}
		}

	}
	
}

//(Funktionen)*****

void zyklus1 (void)
{
	if (S3 == 1)
	{
	  {
		P1_DATA =0x00;              //Wasserzulauf(P1.2) (Y) schließen
		}
		if (S2 == 0)
				{
					if (S6 == 1)
					{
					P1_DATA =0x00;  // Heizung(P1.3) aus
					P0_DATA = 0x08; // Motor (P0.3) ein
					}
					else
					{
					P1_DATA =0x08;	// Heizung(P1.3) ein
				        P0_DATA = 0x08; // Motor (P0.3) ein
					}
				}
			else
				{
					if (S5 == 1)
					{
					P1_DATA =0x00;  // Heizung(P1.3) aus
					P0_DATA = 0x08; // Motor (P0.3) ein	
					}
					else
					{
					P1_DATA =0x08;	// Heizung(P1.3) ein
				        P0_DATA = 0x08; // Motor (P0.3) ein
					}
				}	
	}
	else
	  {
	  P1_DATA = 0x04;				      //Wasserzulauf(P1.2) (Y) öffnen
		}
} 

void init (void)
{
	lcd_init();														// Initiierung des Bildschirms
	lcd_clr();														// Löschen des Bildschirms
	
	P0_DIR = 0x18;	// P0.5 als Eingang, P0.4 und P0.3 als Ausgang
	P1_DIR = 0xFF;
	P3_DIR = 0x00;	
	
	P0_DATA = 0x00;
	P1_DATA = 0x00;
	P3_DATA = 0x00;
	
	TMOD = 0x11;      // Timer 0 initialisiert
	
	adc_init();										// ADC initialisieren
}

void teiler (void)
{
	temperatur_10 = adc_in(4);
	temperatur_500 = ( 500 * temperatur_10 ) / 1024 ;			// In temperatur_500 konvertieren
	H = temperatur_500 / 100 ;														// Hundert
	Z = ( temperatur_500 % 100 ) / 10 ;										// Zehn mit Rest von Hundert und durch 10
	E = ( temperatur_500 % 100 ) % 10 ;										// Einzeln mit Rest von 10
	
	temperatur_8 = temperatur_10 >> 2;
}

void print (void)
{
	teiler ();
	lcd_curs(1);				// Cursor auf Position 1 setzen
	lcd_str("Temperatur: ");
	asc_out(0x30 + H);	// Ausgabe H
	asc_out(0x30 + Z);	// Ausgabe Z
	lcd_str(",");
	asc_out(0x30 + E);	// Ausgabe E
	lcd_str("°C");
	lcd_str("Start : ");
	asc_out(0x30 + start);
}
