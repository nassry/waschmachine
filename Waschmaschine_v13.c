//************************************************
// Deklaration
//************************************************

#include <xc866.h>

// ADC-Funktionen:
		extern void adc_init(void);													// A/D-Wandler initialisieren
		extern unsigned int adc_in(unsigned char kanal);		// Definition Eingang A/D-Wandler

// LCD Funktionen:
		extern void lcd_init(void);		      // Init LCD
		extern void lcd_clr(void);	 	     	// LCD Löschen 
		extern void lcd_str(char *ptr);	    // Ausgabe String
		extern void lcd_curs(char);		      // Cursor setzen
		extern void lcd_int(unsigned int);	// Ausgabe Int Wert => 4 stellig
	 
// Texte:
		unsigned char Masch[]="Maschine";   		// LCD-Text Waschmaschine An
		unsigned char Y[]="Ventil";    					// LCD-Text Ventil Auf
		unsigned char H[]="Heizung";   	     	 	// LCD-Text Heizung An
		unsigned char M[]="Motor";   	   	 			// LCD-Text Motor An
		unsigned char Schl[]="Schleudern";   	  // LCD-Text Motor An
		unsigned char Pumpe[]="Pumpe";					// LCD-Text Pumpe An
		unsigned char temp[]="°C Zu Hoch";  	 	// LCD-Text Temperatur zu hoch
		unsigned char Wab[]="Error";		  			// LCD-Text Waschabbruch
		unsigned char An[]="An";								// LCD-Text An					
		unsigned char Aus[]="Aus";							// LCD-Text Aus
		unsigned char Auf[]="Auf";							// LCD-Text Auf					
		unsigned char Zu[]="Zu";								// LCD-Text Zu
		unsigned char Leer[]=" ";								// LCD-Text Text Löschen
		

// Eigene Funktionen:
		void init (void);																// Initialisierung
		void reset_timer(void);													// Funktion Timer zurücksetzten
		void stoppuhr(void);														// Funktion Stoppuhr
		void teiler (void);															// Funktion teiler für Umgebungstemperatur
		void temp_ueberwachung(void);										// Temperaturüberwachung für Umgebung
		void waschgang(void);														// Funktion Waschgang
		void maschine_an(void);													// Funktion Maschine an
		void maschine_aus(void);												// Funktion Maschine aus
		void ventil_auf(void);													// Funktion Ventil auf
		void ventil_zu(void);														// Funktion Ventil zu
		void motor_anschalten(void);										// Funktion Motor an
		void motor_ausschalten(void);										// Funktion Motor aus
		void motor_schleudern_an(void);									// Funktion Motor schleudern an
		void motor_schleudern_aus(void);								// Funktion Motor schleudern aus
		void heizung_anschalten (void);									// Funktion Heizung an
		void heizung_ausschalten (void);								// Funktion Heizung aus
		void pumpe_anschalten (void);										// Funktion Pumpe an
		void pumpe_ausschalten (void);									// Funktion Pumpe aus
		void temp_zu_hoch(void);												// Funktion Temperatur zu hoch
		void temp_ok(void);															// Funktion Temperatur ok
		void abbruch(void);															// Funktion Programmabbruch
		void text_loeschen(unsigned int, unsigned int);	// Funktion Text Loeschen	
		
		unsigned int zeitabfrage (unsigned int); 	// Zeitabfrage
		
// Variablen:
		sbit S1 = P0_DATA^5;    // (Schalter) Hauptschalter - Interruptbetrieb EXT0
		sbit S2 = P3_DATA^1;    // (Sensor) Temperatur Schalter
		sbit S3 = P3_DATA^2;    // (Sensor) Fühlstand erreicht
		sbit S4 = P3_DATA^3;    // (Sensor) Füllstand überschritten
		sbit S5 = P3_DATA^4;    // (Signal) 90 grad
		sbit S6 = P3_DATA^5;    // (Signal) 60 grad
	
	
		unsigned int zustand;					// (Variable) LCD-Anzeige
		unsigned int timer_1;					// Zeit für 1 Sekunde
		unsigned int zeitpluseins;		// Zählt Ticks
		unsigned int start;						// Programmstart
		unsigned int ende; 						// Programmende
		unsigned int i;								// Variable i

		unsigned long temperatur_10;		// Temperatur 10_bit
		unsigned long temperatur_500;		// Temperatur 0-50,0°C

//************************************************
// Main
//************************************************

void main (void)						// Funktion Main
{
	init ();									// Initialisierung aufrufen
	reset_timer();						// Timer Reset
	while(1)									// While schleife
	{	
		temp_ueberwachung();		// Temperaturueberwachung aufrufen
		if(start == 1)					// Wenn Variable start = 1, starte Waschgang		
		{	
			waschgang();					// Waschgang aufrufen
		}	
	}
}	

//************************************************
// Funktionen
//************************************************

void init (void)									// Funktion Initialisierung
{
	lcd_init();					// Initiierung des Bildschirms
	lcd_clr();					// Löschen des Bildschirms
	
	P0_DIR = 0x18;			// Port 0; P0.5 als Eingang, P0.4 und P0.3 als Ausgang
	P1_DIR = 0xFF;			// Port 1; alles als Ausgang
	P3_DIR = 0x00;			// Port 3; alles als Eingang
	
	P0_DATA = 0x00;			// Port 0; Datenausgabe
	P1_DATA = 0x00;			// Port 1; Datenausgabe
	P3_DATA = 0x00;			// Port 3; Datenausgabe
	
	TMOD = 0x11;     		// Timer 0 initialisiert; 16 Bit Zähler/Zeitgeber

	adc_init();					// ADC initialisieren
	
	zustand = 0;				// Zurücksetzten von switch(zustand)
	
	zeitpluseins = 0;		// Initialisiierung zähler Ticks
	
	start = 1;					// Programmstart
	ende = 0;						// Programmende
	
	//Interrupt
		IP = 1;						// Ex. Interrupt 0 auf höchste Prio
		EXICON0=0x00;			// Fallende Flanken
		IT0=0;					  // Fallende Flanken 2-ter Freigabe
		EX0 = 1;					// Ext. 0 Freigabe
		ET0 = 1;					// Timer 0 Freigabe
		EA = 1; 					// Globale Freigabe
}

void maschine_an(void)						// Funktion Maschine an P0.5
{
	lcd_curs(3);										// Cursor auf Position 1 setzen
	lcd_str(Masch);									// LCD-Text Maschine
	lcd_curs(12);										// Cursor auf Position 1 setzen
	lcd_str(An);										// LCD-Text An
	text_loeschen(14, 15);					// LCD-Text Löschen
}	

void maschine_aus(void)						// Funktion Maschine aus P0.5
{
	lcd_curs(3);										// Cursor auf Position 1 setzen
	lcd_str(Masch);									// LCD-Text Maschine
	lcd_curs(12);										// Cursor auf Position 1 setzen
	lcd_str(Aus);										// LCD-Text Masch. Aus
}	

void ventil_auf(void)							// Funktion Ventil auf P1.2
{
	P1_DATA = 0x04;									// Port 1 P1.2
	lcd_curs(23);										// Cursor auf Position 1 setzen
	lcd_str(Y);											// LCD-Text Ventil
	lcd_curs(30);										// Cursor auf Position 1 setzen
	lcd_str(Auf);										// LCD-Text Auf
}	

void ventil_zu(void)							// Funktion Ventil zu P1.2
{
	P1_DATA =0x00;									// Port 0 alles aus
	lcd_curs(23);										// Cursor auf Position 1 setzen
	lcd_str(Y);											// LCD-Text Ventil
	lcd_curs(30);										// Cursor auf Position 1 setzen
	lcd_str(Zu);										// LCD-Text Zu
	text_loeschen(32, 33);					// LCD-Text Löschen
}

void motor_anschalten (void)			// Funktion Motor anschalten P0.3
{
	P0_DATA = 0x08;									// Port 0 P0.3
	lcd_curs(43);										// Cursor auf Position 1 setzen
	lcd_str(M);											// LCD-Text Motor
	lcd_curs(49);										// Cursor auf Position 1 setzen
	lcd_str(An);										// LCD-Text An
	text_loeschen(51, 63);					// LCD-Text Löschen
}

void motor_ausschalten (void)			// Funktion Motor ausschalten P0.3
{
	P0_DATA = 0x00;									// Port 0 alles aus
  lcd_curs(43);										// Cursor auf Position 1 setzen
	lcd_str(M);											// LCD-Text Motor
	lcd_curs(49);										// Cursor auf Position 1 setzen
	lcd_str(Aus);										// LCD-Text Aus
	text_loeschen(52, 63);					// LCD-Text Löschen	
}
void motor_schleudern_an(void)		// Funktion Motor schleudern an P0.4
{
	P0_DATA = 0x18;									// Port 0 P.04
	lcd_curs(43);										// Cursor auf Position 1 setzen
	lcd_str(M);											// LCD-Text Motor
	lcd_curs(49);										// Cursor auf Position 1 setzen
	lcd_str(Schl);									// LCD-Text Schleudern
}
	
void motor_schleudern_aus(void)		// Funktion Motor schleudern aus P0.3
{
	motor_ausschalten();						// Funktion inkl. LCD-Text	
}	
	
void heizung_anschalten (void)		// Funktion Heizung anschalten P1.3
{
	P1_DATA = 0x08;									// Port 1 P1.3
	lcd_curs(63);										// Cursor auf Position 1 setzen
	lcd_str(H);											// LCD-Text Heizung
	text_loeschen(70, 71);					// LCD-Text Löschen
	lcd_curs(71);										// Cursor auf Position 1 setzen
	lcd_str(An);										// LCD-Text An
	text_loeschen(73, 74);					// LCD-Text Löschen
}
void heizung_ausschalten (void)		// Funktion Heizung ausschalten P1.3
{
	P1_DATA = 0x00;									// Port 1 alles aus
	lcd_curs(63);										// Cursor auf Position 1 setzen
	lcd_str(H);											// LCD-Text Heizung
	text_loeschen(70, 71);					// LCD-Text Löschen
	lcd_curs(71);										// Cursor auf Position 1 setzen
	lcd_str(Aus);										// LCD-Text Aus
}

void pumpe_anschalten(void)				// Funktion Pumpe anschalten P1.4
{
	P1_DATA = 0x10;									// Port 1 P1.4
	lcd_curs(63);										// Cursor auf Position 1 setzen
	lcd_str(Pumpe);									// LCD-Text Pumpe
	text_loeschen(68, 69);					// LCD-Text Löschen	
	lcd_curs(69);										// Cursor auf Position 1 setzen
	lcd_str(An);										// LCD-Text An
	text_loeschen(71, 74);					// LCD-Text Löschen	
}
void pumpe_ausschalten(void)			// Funktion Pumpe ausschalten P1.4
{
	P1_DATA = 0x00;									// Port 1 alles aus
	lcd_curs(63);										// Cursor auf Position 1 setzen
	lcd_str(Pumpe);									// LCD-Text Pumpe
	text_loeschen(68, 69);					// LCD-Text Löschen
	lcd_curs(69);										// Cursor auf Position 1 setzen
	lcd_str(Aus);										// LCD-Text Aus
	text_loeschen(72, 74);					// LCD-Text Löschen	
}

void abbruch(void)								// Funktion Abbruch	
{
	lcd_curs(3);										// Cursor auf Position 1 setzen //In Funktion abbilden!!
	lcd_str(Wab);										// LCD-Text Abbruch
	text_loeschen(8, 73);						// LCD-Text Löschen
}

void temp_zu_hoch(void)						// Funktion Temperatur zu hoch
{
	lcd_curs(63);										// Cursor auf Position 1 setzen //IN Funktion abbilden!!
	lcd_str(temp);									// LCD-Text Temperatur zu hoch
	text_loeschen(73, 74);					// LCD-Text Löschen
}	

void temp_ok(void)						// Funktion Temperatur zu hoch
{
	text_loeschen(63, 74);					// LCD-Text Löschen
}	
void text_loeschen(unsigned int cours_start, unsigned int cours_end)
{
	for (i = cours_start; i < cours_end; i++)
	{
		lcd_str(Leer);
	}	
}	

void waschgang(void)							// Funktion Waschgang
{
	if(ende == 1)										// Wenn Variable ende = 1
	{
		if(zustand != 0)							// Wenn Variable zustand != 0 (!=; ungleich)
		{
			if(zustand != 200)					// Wenn Variable zusttand != 200 (!=; ungleich)
			{
				zustand = 100;						// Aufrufen von zustand = 100
			}
		}	
		ende = 0;											// Sonst ist Variable ende = 0; Programmende
	}
	switch (zustand)								// Switch_Case Funktion
	{
		case 0:												// Case 0; Startabfrage
			if(S1 == 1)									// Wenn S1 = 1; Wenn Hauptschalter auf Ein
			{
			maschine_an();							// Maschine anschalten aufrufen
			zustand = 1;								// Aufrufen von zustand = 1
			}
			else												// Sonst
			{	
			maschine_aus();							// Maschine ausschalten aufrufen
			}	
			break;											// Abschluss
			
		case 1:												// Case 1; Wasserzulauf			
			if (S3 == 1)								// Wenn S3 = 1; Wenn Füllstand S3 erreicht
			{
			ventil_zu();								// Ventil zu aufrufen
			zustand = 2;								// Aufrufen von zustand = 2
			stoppuhr();									// Stoppuhr aufrufen
			}
			else												// Sonst
			{
			ventil_auf();								// Ventil auf aufrufen
			}
			break;											// Abschluss
			
		case 2:												// Case 2; Waschmaschine Vorheizen
			if (S2 == 0)								// Wenn S2 = 0; Wenn Temperaturwahlhebel auf 60°C
				{
					if(S6 == 0)							// Wenn S6 = 0; Wenn Temperatur 60°C nicht erreicht
					{
					heizung_anschalten();		// Heizung einschalten aufrufen
					}
					else										// Sonst
					{
					zustand = 3;						// Aufrufen von zustand = 3
					stoppuhr();							// Stoppuhr aufrufen
					}
				}
				else											// Sonst
				{
					if(S5 == 0)							// Wenn S5 = 0; Wenn Temperatur 90°C nicht erreicht
					{
					heizung_anschalten();		// Heizung einschalten aufrufen
					}
					else										// Sonst
					{
					zustand = 3;						// Aufrufen von zustand = 3
					stoppuhr();							// Stoppuhr aufrufen
					}			
				}	
				break;										// Abschluss
				
		case 3:												// Case 3; Waschen und Temperatur regeln für 60 Sekunden aktivieren
			if(zeitabfrage(30))					// Wenn zeitabfrage(30); Funktion Zeitabfrage gibt die tatsächliche Laufzeit von 60 Sekunden an
			{	
				zustand = 4;							// Aufrufen von zustand = 4
				stoppuhr();								// Stoppuhr aufrufen
			}
			else												// Sonst
			{
				if (S2 == 0)							// Wenn S2 = 0; Wenn Temperaturwahlhebel auf 60°C
				{
					if(S6 == 0)							// Wenn S6 = 0; Wenn Temperatur 60°C nicht erreicht
					{
					heizung_anschalten();		// Heizung einschalten aufrufen
					}
					else										// Sonst
					{
					heizung_ausschalten();	// Heizung ausschalten aufrufen
					}
				}	
				if (S2 == 1)							// Wenn S2 = 0; Wenn Temperaturwahlhebel auf 90°C 
				{
					if(S5 == 0)							// Wenn S5 = 0; Wenn Temperatur 90°C nicht erreicht
					{
					heizung_anschalten();		// Heizung einschalten aufrufen
					}
					else										// Sonst
					{
					heizung_ausschalten();	// Heizung ausschalten aufrufen
					}
				}
			}	
			motor_anschalten();					// Motor anschalten aufrufen
			break;											// Abschluss
			
		case 4:												// Case 4; Maschinen-Schleudern und Pumpe für 10 Sekunden aktivieren			
			if(zeitabfrage(10))					// Wenn zeitabfrage(10); Funktion Zeitabfrage gibt die tatsächliche Laufzeit von 10 Sekunden an					
			{
				zustand = 5;							// Aufrufen von zustand = 5
				stoppuhr();								// Stoppuhr aufrufen
			}
			else												// Sonst
			{
			 motor_schleudern_an();			// Motor-Schleudern anschalten aufrufen
			 pumpe_anschalten();				// Pumpe anschalten aufrufen
			}
			break;											// Abschluss
	
		case 5:												// Case 5; Maschinen-Schleudern weiterhin aktiv; Pumpe ausschalten 
			if(zeitabfrage(20))					// Wenn zeitabfrage(20); Funktion Zeitabfrage gibt die tatsächliche Laufzeit von 20 Sekunden an
			{
				zustand = 6;							// Aufrufen von zustand = 6
			}
			else												// Sonst
			{
			 motor_schleudern_an();			// Motor-Schleudern anschalten aufrufen
			 pumpe_ausschalten();				// Pumpe ausschalten aufrufen
			}
			break;											// Abschluss

		case 6:												// Case 6; Motor-Schleudern auschalten aufrufen; Sonderzustand 200 aufrufen
		{
			motor_schleudern_aus();			// Motor-Schleudern ausschalten aufrufen	
			zustand = 200;							// Aufrufen von zustand 200
		}
		break;												// Abschluss
		
		case 100:											// Case 100; Ext. Interrupt-Sequenz
			motor_ausschalten();				// Motor ausschalten aufrufen
			heizung_ausschalten();			// Heizung ausschalten aufrufen
			zustand = 101;							// Aufrufen von zustand = 101
			stoppuhr();									// Stoppuhr aufrufen
			abbruch();									// Abbruch aufrufen
			pumpe_anschalten();					// Pumpe anschalten aufrufen
			break;											// Abschluss
		
		case 101:											// Case 101; Ext. Interrupt-Sequenz; Laufzeit Pumpe von 10 Sekunden
			if(zeitabfrage(10))					// Wenn zeitabfrage(10); Funktion Zeitabfrage gibt die tatsächliche Laufzeit von 10 Sekunden an				
			{
				pumpe_ausschalten();			// Pumpe ausschalten aufrufen
				zustand = 200;						// Aufrufen von zustand = 200
			}
			break;											// Abschluss
			
		case 200:											// Case 200; Programmabschluss
			if(S1 == 0)									// Wenn S1 = 0; Wenn Hauptschalter auf Aus 
			{
			zustand = 0;								// Aufrufen von zustand = 0
			}
			maschine_an();							// Maschine anschalten aufrufen
			break;											// Abschluss
	}	
}	

void stoppuhr(void)		// Funktion Stoppuhr
{
	timer_1 = 0;				// timer_1 auf 0 Sekunden stellen
	reset_timer();			// Startet Stoppuhr
}	

unsigned int zeitabfrage (unsigned int zielzeit)		// Variable zeitabfrage gibt aus, ob die eingestellten Zeiten (60s,30s,10s) erreicht sind	
{
	return (timer_1 >= zielzeit);											// Abgleich ob zielzeit in Sekunden erreicht ist
}
	
void reset_timer(void)	// Funktion reset Timer0		
{
	TR0 = 1;							// Timer Run
	ET0 = 1;							// Timer 0 Freigabe
	EA = 1; 							// Globale Freigabe
}

void interrupt_switch_0 (void) interrupt 0		// Funktion Ext. Interrupt; Ein/Aus Schalten während laufendem Betrieb auf Aus
{
	IRCON0 = 0x00;															// Löschen der Flag im IRCON-Register
	ende = 1;																		// Variable ende = 1 setzten
}

void interrupt_timer_0 (void) interrupt 1		// Funktion Timer Interrupt
{
	ET0 = 0;																	// Timer 0 Freigabe löschen
	TR0 = 0;																	// Timer 0 stoppt
	zeitpluseins++;														// zeitpluseins beginnt zu zählen
	if(zeitpluseins >= 183)										// Wenn zeitpluseins größer gleich 183 Ticks (183 Ticks entspricht 1 Sekunde)
	{
		zeitpluseins = 0;												// Setzt zählerpluseins auf 0
		timer_1++;															// Setzt Zähler für 1 Sekunde auf 0 zurück
	}
	reset_timer();														// Reset_Timer aufrufen
}

void teiler (void)																			// Funktion Spannungsteiler
{
	temperatur_10 = adc_in(4);														// Eingang auf AN4 gelegt
	temperatur_500 = ( 500 * temperatur_10 ) / 1024 ;			// In temperatur_500 konvertieren
}

void temp_ueberwachung(void)					// Funktion Temperatursensor
{
	teiler();														// Teiler aufrufen
	if(start == 1)											// Wenn start = 1
	{		
		if(temperatur_500 >= 300)					// Wenn temperatur_500 größer gleich 30 °C
		{
			start = 0;											// Variable start = 0 setzen
			temp_zu_hoch();									// Temp_zu_Hoch aufrufen						
		}
	}	
	else																// Sonst
	{
		if(temperatur_500 <= 270)					// Wenn temperatur_500 kleiner gleich 27°C
		{
			start = 1;											// Variable start = 1 setzen
			temp_ok();											// Temp_ok aufrufen
		}	
	}
}


