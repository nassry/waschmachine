//************************************************
// Deklaration
//************************************************

#include <xc866.h>

// ADC-Funktionen:
		extern void adc_init(void);
		extern unsigned int adc_in(unsigned char kanal);

// LCD Funktionen:
		extern void lcd_init(void);		      // Init LCD
		extern void lcd_clr(void);	 	     	// LCD Löschen 

		extern void lcd_str(char *ptr);	    // Ausgabe String
		extern void lcd_curs(char);		      // Cursor setzen
		extern void asc_out(char);	   		  // Ausgabe eines ASCII Zeichens
		extern void lcd_int(unsigned int);	// Ausgabe Int Wert => 4 stellig
	 
// Texte:
		unsigned char Waus[]="Maschine Aus";   	// LCD-Text Waschmaschine Aus
		unsigned char Wan[]="Maschine An ";   	// LCD-Text Waschmaschine An
		unsigned char WS3[]="Wasser bis S3";    // LCD-Text Wasserzulauf bis S3
		unsigned char Han[]="H An";   	        // LCD-Text Heizung An
		unsigned char Hau[]="H Aus";						// LCD-Text Heizun Aus
		unsigned char Man[]="Mtr An";   	      // LCD-Text Motor An
		unsigned char temp[]="Temp. zu hoch";   	// LCD-Text Motor An
		unsigned char def[]="Fehler";						// LCD-Text Fehler

// Eigene Funktionen:
		void init (void);
		void waschgang (void);
		void teiler (void);							// Teilen in einzelne Ziffer
		void timer (void);							// 5.461 msec. 
		void zustand (void);						// LCD-Anzeige

// Variablen:
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
		unsigned int ende;			// (Steuersignal) Ende			
		unsigned int p; 				// (Variable) 10sek.
		unsigned int anzeige;		// (Variable) LCD-Anzeige

		unsigned long temperatur_10;			// Temperatur 10_bit
		unsigned long temperatur_500;			// Temperatur 0-50,0°C

//************************************************
// Main
//************************************************

void main (void)
{
	init ();
	while(1)
	{
		teiler();
		zustand();
		start = 1;
		anzeige = 1;
		P1_DATA = 0x00;							//  Wasserzulauf(P1.2) (Y) schließen nach Timer-Interrupt 
		while (temperatur_500 >= 300)
		{
			while (temperatur_500 >= 270)
			{
				teiler ();
				start = 0;
				anzeige = 6;
				zustand();
			}
		}
		if (S1==1)
		{	
			anzeige = 0;
			if (start == 1)
			{
			EA = 1;
			waschgang();
			}
		}
	}
}

//************************************************
// Funktionen
//************************************************

void waschgang (void)			// Funktion Waschgang
{
	if (S3 == 1)					  // Wenn Signal 1 an S3; Füllstand erreicht
	{
		P1_DATA =0x00;        // Wasserzulauf(P1.2) (Y) schließen
		if (S2 == 0)					// Wenn Wahlschaler (P3.1); Signal 0 an S2; Temperatur 60°C
		{
			if (S6 == 1)				// Wenn Signal 1 an S6; Temperatur 60°C erreicht
			{
			P1_DATA = 0x00;  		// Heizung(P1.3) aus; Heizung H ausschalten wenn 60°C erreicht sind
			anzeige = 4;				// LCD-Text Heizung aus
			P0_DATA = 0x08;  		// Motor (P0.3) ein;  Motor M einschalten sobald Signal 1 an S3; Füllstand erreicht
			anzeige = 5;				// LCD-Text Motor an
			TR0 = 1;						// Steuerbit für Zähler/Zeitgeber 0
			}
			else								// Sonst
			{
			P1_DATA = 0x08;			// Heizung(P1.3) ein; Heizung einschalten, wenn Temperatur unter 60°C sinkt
			anzeige = 3;				// LCD-Text Heizung an
			P0_DATA = 0x08;			// Motor (P0.3) ein;  Motor M ist weiterhin an
			anzeige = 5;				// LCD-Text Motor an
			}
		}
		else									// Sonst
		{
			if (S5 == 1)				// Signal 1 an S5; Temperatur 90°C erreicht					
			{
			P1_DATA = 0x00;  		// Heizung(P1.3) aus; Heizung H ausschalten wenn 90°C erreicht sind
			anzeige = 4;				// LCD-Text Heizung aus
			P0_DATA = 0x08;  		// Motor (P0.3) ein;  Motor M einschalten sobald Signal 1 an S3; Füllstand erreicht
			anzeige = 5;				// LCD-Text Motor an
			TR0 = 1;						// Steuerbit für Zähler/Zeitgeber 0
			}
			else								// Sonst
			{
			P1_DATA = 0x08;			// Heizung(P1.3) ein; Heizung einschalten, wenn Temperatur unter 90°C sinkt
			anzeige = 4;				// LCD-Text Heizung an
			P0_DATA = 0x08;			// Motor (P0.3) ein;  Motor M ist weiterhin an
			anzeige = 5;				// LCD-Text Motor an
			}
		}		
	}
	else										// Sonst
	  {
	  P1_DATA = 0x04;				// Wasserzulauf(P1.2) (Y) öffnen
		anzeige = 2;					// Anzeige Wasserzulauf
		}
} 

void init (void)
{
	lcd_init();				// Initiierung des Bildschirms
	lcd_clr();				// Löschen des Bildschirms
	
	P0_DIR = 0x18;		// Port 0; P0.5 als Eingang, P0.4 und P0.3 als Ausgang
	P1_DIR = 0xFF;		// Port 1; alles als Ausgang
	P3_DIR = 0x00;		// Port 3; alles als Eingang
	
	P0_DATA = 0x00;		// Port 0; Datenausgabe
	P1_DATA = 0x00;		// Port 1; Datenausgabe
	P3_DATA = 0x00;		// Port 3; Datenausgabe
	
	TMOD = 0x11;      // Timer 0 initialisiert; 16 Bit Zähler/Zeitgeber
	
	adc_init();				// ADC initialisieren
	
	//Interrupt
		EXICON0=0x00;			// Fallende Flanken
		IT0=0;					  // Fallende Flanken 2-ter Freigabe
		EX0 = 1;					// Ext. 0 Freigabe
		ET0 = 1;					// Timer 0 Freigabe
		EA = 1; 					// Globale Freigabe
}

void teiler (void)
{
	temperatur_10 = adc_in(4);
	temperatur_500 = ( 500 * temperatur_10 ) / 1024 ;			// In temperatur_500 konvertieren
}

//void print (void)
//{
//	teiler ();
//	lcd_curs(1);							// Cursor auf Position 1 setzen
//	lcd_str("Temperatur: ");
//	asc_out(0x30 + H);				// Ausgabe H
//	asc_out(0x30 + Z);				// Ausgabe Z
//	lcd_str(",");
//	asc_out(0x30 + E);				// Ausgabe E
//	lcd_str("°C");
//	lcd_str("Start : ");
//	asc_out(0x30 + start);
//}

void timer (void)
{
	TF0 = 0;								// Timer-Flag auf 0 - bitaddresiert
	TL0 = 0x00;							// Erste 8-binare - bitadresiert (start)
	TH0 = 0x00;							// Zweite 8-binare - bitadresiert
	TR0 = 1;								// Timer 0 starten - bitadresiert 
	while (TF0 == 0);				// Loop; Timerflag 0 = 0
	TR0 = 0;								// Timer 0 stoppt - bitadressiert
}

void interrupt_switch_0 (void) interrupt 0
{
	ET0 = 0;								// Timer 0 Freigabe löschen
	TR0 = 0;								// Timer 0 stoppt
	P1_DATA = 0x00;  				// Heizung (P1.3)aus
	P0_DATA = 0x00;  				// Motor (P0.3) aus
	P1_DATA = 0x10;  				// Pumpe (P1.4) an
	for(p=0; p<1831; p++)		// Warten-Funktion Pumpe für 10sek.
		{
			timer();
		}
	P1_DATA = 0x00;  				// Pumpe (P1.4) aus	
  while(S1==0);	
	IRCON0=0x00;
	TF0 = 0;								// Timer Flag 0
	ET0 = 1;								// Timer 0 interruptbetrieb-Freigabe
}

void interrupt_timer_0 (void) interrupt 1
{
	ET0 = 0;								// Timer 0 Freigabe löschen
	TR0 = 0;								// Timer 0 stoppt
	for(p=0; p<1860; p++)		// Warten-Funktion Pumpe für 10sek.
		{
			timer();
			waschgang();
		}
	P0_DATA = 0x18;  				// MSchleudern (P0.4) und Motor (P0.3) an
	P1_DATA = 0x10;  				// Pumpe (P1.4) an
	for(p=0; p<1860; p++)		// Warten-Funktion Pumpe für 10sek.
	{
		timer();
	}
	P1_DATA = 0x00;  				// Pumpe (P1.4) aus
	for(p=0; p<1860; p++)		// Warten-Funktion Pumpe für 10sek.
	{
		timer();
	}
	P0_DATA = 0x00;  				// MSchleudern (P0.4) und Motor (P0.3) aus
	TF0 = 0;								// Timer Flag 0
	ET0 = 1;								// Timer 0 interruptbetrieb-Freigabe
	EA = 0;									// Freigabe zurücksetzen
	while(S1==1);						// Programm Neustart
	IRCON0=0x00;						// Flag zurücksetzen; durch Ausschalten betätigt.
}

void zustand (void)
{
	switch(anzeige)
	{
		case 0:
			lcd_curs(3);				// Cursor auf Position 1 setzen
	    lcd_str(Wan);				// LCD-Text Waschmaschine an
		  break;							// Abschluss
		case 1:
			lcd_curs(3);				// Cursor auf Position 1 setzen
	    lcd_str(Waus);			// LCD-Text Waschmaschine aus
			break;							// Abschluss
		case 2:
			lcd_curs(23);				// Cursor auf Position 1 setzen
			lcd_str(WS3);				// LCD-Text Wasserzulauf bis S3
			break;							// Abschluss
		case 3:
			lcd_curs(63);				// Cursor auf Position 1 setzen
			lcd_str(Han);				// LCD-Text Heizung an
			break;							// Abschluss
		case 4:								// Auskommentiert, da Speicher zu klein
			lcd_curs(63);				// Cursor auf Position 1 setzen
			lcd_str(Hau);				// LCD-Text Heizung an
			break;							// Abschluss
		case 5:								// Auskommentiert, da Speicher zu klein
			lcd_curs(43);				// Cursor auf Position 1 setzen
			lcd_str(Man);				// LCD-Text Motor an
			break;							// Abschluss
		case 6:
			lcd_curs(63);				// Cursor auf Position 1 setzen
	    lcd_str(temp);			// LCD-Text Temperatur zu hoch
		  break;							// Abschluss
		default:
			lcd_curs(31);				// Cursor auf Position 1 setzen
	    lcd_str(def);				// LCD-Text FEHLER
			break;							// Abschluss
	}
}

