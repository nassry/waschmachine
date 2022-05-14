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

void init (void);								//
void teiler (void);							// Teilen in einzelne Ziffer
void print (void);							// Temperatur darstellen

//Variablen:

unsigned long temperatur_8;				//temperatur 8_bit
unsigned long temperatur_10;			//temperatur 10_bit
unsigned long temperatur_500;			//temperatur 0-50,0°C
unsigned int H, Z, E, temperatur;	// Speicher für H,Z,E
unsigned int start = 0;						// Startsignal 

//************************************************
//Main
//************************************************

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
	}
}

//************************************************
//Function
//************************************************

void init (void)
{
	lcd_init();				// LCD initialisieren
	lcd_clr();				// Anzeige löschen
	adc_init();				// ADC initialisieren
	P3_DIR = 0xFF;
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
	lcd_str(" °C");
	lcd_str("Start : ");
	asc_out(0x30 + start);
}




