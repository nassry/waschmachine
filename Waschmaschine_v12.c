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
		extern void lcd_int(unsigned int);	// Ausgabe Int Wert => 4 stellig
	 
// Texte:
		unsigned char Wan[]="Masch. An ";   		// LCD-Text Waschmaschine An
		unsigned char Wau[]="Masch. Aus ";   		// LCD-Text Waschmaschine Aus
		unsigned char Y3A[]="Y Auf";    				// LCD-Text Ventil Auf
		unsigned char Y3Z[]="Y Zu ";						// LCD-Text Ventil Zu
		unsigned char Han[]="H An ";   	        // LCD-Text Heizung An
		unsigned char Hau[]="H Aus";						// LCD-Text Heizun Aus
		unsigned char Man[]="Mtr. An";   	      // LCD-Text Motor An
		unsigned char Mau[]="Mtr. Aus";					// LCD-Text Motor Aus
//		unsigned char Pan[]="P an";							// LCD-Text Pumpe An
//		unsigned char Pau[]="P aus";						// LCD-Text Pumpe Aus
		unsigned char temp[]="Temp. zu hoch";   // LCD-Text Temperatur zu hoch
		unsigned char Wab[]="Abbruch";					// LCD-Text Waschabbruch

// Eigene Funktionen:
		void init (void);													// Initialisierung
		void anzeige (void);											// LCD-Anzeige
		void reset_timer(void);										// Dauer Waschgang
		void stoppuhr(void);											// Funktion Stoppuhr
		void teiler (void);												// Funktion teiler für Umgebungstemperatur
		void temp_ueberwachung(void);							// Funktion Umgebungstemperaturüberwachung
		void waschgang(void);											// Funktion Waschgang
		void ventil_auf(void);										// Funktion Ventil auf
		void ventil_zu(void);											// Funktion Ventil zu
		void motor_anschalten(void);							// Funktion Motor an
		void motor_ausschalten(void);							// Funktion Motor aus
		void motor_schleudern_an(void);						// Funktion Motor schleudern an
		void motor_schleudern_aus(void);					// Funktion Motor schleudern aus
		void heizung_anschalten (void);						// Funktion Heizung an
		void heizung_ausschalten (void);					// Funktion Heizung aus
		void pumpe_anschalten (void);							// Funktion Pumpe an
		void pumpe_ausschalten (void);						// Funktion Pumpe aus
		unsigned int zeitabfrage (unsigned int); 	// Zeitabfrage
		
// Variablen:
		sbit S1 = P0_DATA^5;    // (Schalter)Hauptschalter - Interruptbetrieb EXT0
		sbit S2 = P3_DATA^1;    // (Sensor)Temperatur Schalter
		sbit S3 = P3_DATA^2;    // (Sensor)Fühlstand erreicht
		sbit S4 = P3_DATA^3;    // (Sensor)Füllstand überschritten
		sbit S5 = P3_DATA^4;    // (Signal) 90 grad
		sbit S6 = P3_DATA^5;    // (Signal) 60 grad
	
	
		unsigned int zustand;					// (Variable) LCD-Anzeige
		unsigned int timer_1;					// Zeit für 1 Sekunde
		unsigned int zeitpluseins;		// Zählt Ticks
		unsigned int ende = 0;				// Programmende

		unsigned long temperatur_10;		// Temperatur 10_bit
		unsigned long temperatur_500;		// Temperatur 0-50,0°C

//************************************************
// Main
//************************************************

void main (void)
{
	init ();
	reset_timer();
	while(1)
	{
	waschgang();	
	}
}
	

//************************************************
// Funktionen
//************************************************

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
	
	zustand = 0;
	
	zeitpluseins = 0;		// Initialisiierung zähler Ticks
	
	//Interrupt
		IP = 1;						// Ex. Interrupt 0 auf höchste Prio
		EXICON0=0x00;			// Fallende Flanken
		IT0=0;					  // Fallende Flanken 2-ter Freigabe
		EX0 = 1;					// Ext. 0 Freigabe
		ET0 = 1;					// Timer 0 Freigabe
		EA = 1; 					// Globale Freigabe
}

void ventil_auf(void)
{
	P1_DATA = 0x04;
	lcd_curs(23);				// Cursor auf Position 1 setzen
	lcd_str(Y3A);				// LCD-Text Y Auf
}	

void ventil_zu(void)
{
	P1_DATA =0x00;
	lcd_curs(23);				// Cursor auf Position 1 setzen
	lcd_str(Y3Z);				// LCD-Text Y Zu
}

void motor_anschalten (void)
{
	P0_DATA = 0x08;
	lcd_curs(43);				// Cursor auf Position 1 setzen
	lcd_str(Man);				// LCD-Text Mtr. An
}
void motor_ausschalten (void)
{
	P0_DATA = 0x00;
  lcd_curs(43);				// Cursor auf Position 1 setzen
	lcd_str(Mau);				// LCD-Text Mtr. Aus
	
}
void motor_schleudern_an(void)
{
	P0_DATA = 0x18;
}
	
void motor_schleudern_aus(void)
{
	P0_DATA = 0x00;
}	
	
void heizung_anschalten (void)
{
	P1_DATA = 0x08;
	lcd_curs(63);				// Cursor auf Position 1 setzen
	lcd_str(Han);				// LCD-Text H An
}
void heizung_ausschalten (void)
{
	P1_DATA = 0x00;
	lcd_curs(63);				// Cursor auf Position 1 setzen
	lcd_str(Hau);				// LCD-Text H Aus
}

void pumpe_anschalten(void)
{
	P1_DATA = 0x10;
}
void pumpe_ausschalten(void)
{
	P1_DATA = 0x00;
}


void waschgang(void)
{
	if(ende == 1)
	{
		if(zustand != 0)
		{
			if(zustand != 200)
			{
				zustand = 100;
			}
		}	
		ende = 0;
	}
	switch (zustand)
	{
		case 0:
			if(S1 == 1)
			{
			zustand = 1;
			}
			lcd_curs(3);				// Cursor auf Position 1 setzen
	    lcd_str(Wau);				// LCD-Text Masch. Aus
			break;
			
		case 1:
			if (S3 == 1)
			{
			ventil_zu();
			zustand = 2;
			stoppuhr();
			}
			else
			{
			ventil_auf();
			}
			lcd_curs(3);				// Cursor auf Position 1 setzen
	    lcd_str(Wan);				// LCD-Text Masch. An
			break;
			
		case 2:
			if (S2 == 0)
				{
					if(S6 == 0)
					{
					heizung_anschalten();
					}
					else
					{
					zustand = 3;
					stoppuhr();
					}
				}
				else
				{
					if(S5 == 0)
					{
					heizung_anschalten();
					}
					else
					{
					zustand = 3;
					stoppuhr();
					}			
				}	
				break;
				
		case 3:
			if(zeitabfrage(30))
			{	
				zustand = 4;
				stoppuhr();
			}
			else
			{
				if (S2 == 0)
				{
					if(S6 == 0)
					{
					heizung_anschalten();
					}
					else
					{
					heizung_ausschalten();
					}
				}	
				if (S2 == 1)
				{
					if(S5 == 0)
					{
					heizung_anschalten();
					}
					else
					{
					heizung_ausschalten();
					}
				}
			}	
			motor_anschalten();
			break;
		case 4:
			if(zeitabfrage(10))
			{
				zustand = 5;
				stoppuhr();
			}
			else
			{
			 motor_schleudern_an();
			 pumpe_anschalten();
			}
			break;
		case 5:
			if(zeitabfrage(20))
			{
				zustand = 6;
			}
			else
			{
			 motor_schleudern_an();
			 pumpe_ausschalten();
			}
			break;
		case 6:
		{
			motor_schleudern_aus();
			zustand = 200;
		}
			break;
		
		case 100:
			motor_ausschalten();
			heizung_ausschalten();
			pumpe_anschalten();
			zustand = 101;
			stoppuhr();
			lcd_curs(3);				// Cursor auf Position 1 setzen
	    lcd_str(Wab);				// LCD-Text Abbruch
			break;
		
		case 101:
			if(zeitabfrage(10))
			{
				pumpe_ausschalten();
				zustand = 200;
			}
		break;
			
		case 200:
			if(S1 == 0)
			{
			zustand = 0;
			}
			lcd_curs(3);				// Cursor auf Position 1 setzen
	    lcd_str(Wau);				// LCD-Text Masch. Aus
			break;	
	}	
}	

void stoppuhr(void)
{
	timer_1 = 0;				// timer_1 auf 0 Sekunden stellen
	reset_timer();			// Startet Stoppuhr
}	

unsigned int zeitabfrage (unsigned int zielzeit)
{
	return (timer_1 >= zielzeit);		// Abgleich ob zielzeit in Sekunden erreicht ist
}
	
void reset_timer(void)
{
	TR0 = 1;					// Timer Run
	ET0 = 1;					// Timer 0 Freigabe
	EA = 1; 					// Globale Freigabe
}

void interrupt_switch_0 (void) interrupt 0
{
	IRCON0 = 0x00;
	ende = 1;
}

void interrupt_timer_0 (void) interrupt 1
{
	ET0 = 0;										// Timer 0 Freigabe löschen
	TR0 = 0;										// Timer 0 stoppt
	zeitpluseins++;							// zeitpluseins beginnt zu zählen
	if(zeitpluseins >= 183)
	{
		zeitpluseins = 0;					// Setzt zählerpluseins auf 0
		timer_1++;								// Setzt Zähler für 1 Sekunde auf 0 zurück
	}
	reset_timer();
}

void teiler (void)
{
	temperatur_10 = adc_in(4);														// Eingang auf AN4 gelegt
	temperatur_500 = ( 500 * temperatur_10 ) / 1024 ;			// In temperatur_500 konvertieren
}

void temp_ueberwachung(void)
{
 while(1)
 {
	 teiler();
	 anzeige();
	 zustand = 0;
	 while(temperatur_500 >= 300)
	 {
		 while(temperatur_500 >= 270)
		 {
			 teiler();
			 anzeige();
			 zustand = 9;
		 }
	 }	 
	}	
} 

void anzeige (void)
{
	switch(zustand)
	{

		case 9:
			lcd_curs(63);				// Cursor auf Position 1 setzen
	    lcd_str(temp);			// LCD-Text Temperatur zu hoch
		  break;							// Abschluss
	}
}

