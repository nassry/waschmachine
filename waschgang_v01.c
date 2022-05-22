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
		unsigned char Wan[]="Masch. An ";   			// LCD-Text Waschmaschine An
		unsigned char Wau[]="Masch. Aus ";   		// LCD-Text Waschmaschine Aus
		unsigned char Y3A[]="Y Auf";    				// LCD-Text Ventil Auf
		unsigned char Y3Z[]="Y Zu ";						// LCD-Text Ventil Zu
		unsigned char Han[]="H An";   	        // LCD-Text Heizung An
		unsigned char Hau[]="H Aus";						// LCD-Text Heizun Aus
		unsigned char Man[]="Mtr. An";   	      // LCD-Text Motor An
		unsigned char Mau[]="Mtr. Aus";					// LCD-Text Motor Aus
		unsigned char temp[]="Temp. zu hoch";   // LCD-Text Temperatur zu hoch

// Eigene Funktionen:
		void init (void);											// Initialisierung
//		void anzeige (void);								// LCD-Anzeige
		void timer(void);											// Dauer Waschgang
		void waschgang(void);
		void ventil_auf(void);
		void ventil_zu(void);
		void motor_anschalten(void);
		void motor_ausschalten(void);
		void motor_schleudern_an(void);
		void motor_schleudern_aus(void);
		void heizung_anschalten (void);
		void heizung_ausschalten (void);
		void pumpe_anschalten (void);
		void pumpe_ausschalten (void);
		unsigned int zeitabfrage (unsigned int); 	// Zeitabfrage
		
// Variablen:
		sbit S1 = P0_DATA^5;    // (Schalter)Hauptschalter - Interruptbetrieb EXT0
		sbit S2 = P3_DATA^1;    // (Sensor)Temperatur Schalter
		sbit S3 = P3_DATA^2;    // (Sensor)Fühlstand erreicht
		sbit S4 = P3_DATA^3;    // (Sensor)Füllstand überschritten
		sbit S5 = P3_DATA^4;    // (Signal) 90 grad
		sbit S6 = P3_DATA^5;    // (Signal) 60 grad
	
	
		unsigned int zustand;		// (Variable) LCD-Anzeige
		unsigned int start;			// (Steuersignal) Start
		unsigned int ende;			// (Steuersignal) Ende
		unsigned int timer_1;		// Zeit für 1 Sekunde
		unsigned int timer_10;	// Zeit für 10 Sekunden

		
		unsigned char ProgStage;

//************************************************
// Main
//************************************************

void main (void)
{
	init ();
	while(1)
	{
//		anzeige();
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
	
	zustand = 0;
	
	//Interrupt
		EXICON0=0x00;			// Fallende Flanken
		IT0=0;					  // Fallende Flanken 2-ter Freigabe
		EX0 = 1;					// Ext. 0 Freigabe
		ET0 = 1;					// Timer 0 Freigabe
		EA = 1; 					// Globale Freigabe
}

void ventil_auf(void)
{
	P1_DATA = 0x04;
}	

void ventil_zu(void)
{
	P1_DATA =0x00;
}

void motor_anschalten (void)
{
	P0_DATA = 0x08;
}
void motor_ausschalten (void)
{
	P0_DATA = 0x00;;
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
}
void heizung_ausschalten (void)
{
	P1_DATA = 0x00;
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
	switch (zustand)
	{
		case 0:
			if(S1 == 1)
			{
			zustand = 1;
			}
			break;
		case 1:
			if (S3 == 1)
			{
			ventil_zu();
			zustand = 2;
			}
			else
			{
			ventil_auf();
			}
			break;
		case 2:
			if(zeitabfrage(1))
			{
				zustand = 4;
			}
			else
			{				
				motor_anschalten();
				if (S2 == 0)
				{
					if(S6 == 0)
					{
					heizung_anschalten();
					}
					else
					{
					zustand = 3;
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
					zustand = 3;
					}
				}	
			}
			break;
		case 3:
			if(zeitabfrage(1))
			{	
				zustand = 4;
			}
			else
			{
				if (S2 == 0)
				{
					if(S6 == 0)
					{
					zustand = 2;
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
					zustand = 2;
					}
					else
					{
					heizung_ausschalten();
					}
				}
			}	
			break;
		case 4:
			ventil_auf();
			break;			
	}	
}	
	
unsigned int zeitabfrage (unsigned int zielzeit)
{
	timer();	
	return (timer_10 >= zielzeit);		// Abgleich ob zielzeit*10Sekunden erreicht sind
}

void timer(void)
{
	timer_1 = 0;						// timer_1 auf 0 setzten
	timer_10 = 0;						// timer_10 auf 0 setzten
	TF0 = 0;								// Timer-Flag auf 0
	TL0 = 0x00;							// Erste 8-binare Timelow-Bit
	TH0 = 0x00;							// Zweite 8-binare Timerhigh-Bit
	TR0 = 1;								// Timer 0 starten
	EA = 1;									// Globale Freigabe
	while(TF0 == 0);				// Loop; Timer Flag 0=0
	TR0 = 0;								// Timer 0 stop
}	
	
void interrupt_switch_0 (void) interrupt 0
{
//	ventil_auf();
}

void interrupt_timer_0 (void) interrupt 1
{
	ET0 = 0;								// Timer 0 Freigabe löschen
	TR0 = 0;								// Timer 0 stoppt
	timer_1++;							// Anzahl wie oft 5.461ms verstrichen sind
	if(timer_1 >= 1860)			// Rechnet einzelne Ticks auf 10 Sekunden hoch
	{
		timer_1 = 0;					// Setzt Zähler für 1 Sekunde auf 0 zurück
		timer_10++;						// Gibt an wie oft 10 Sekunden vergangen sind
	}	
	pumpe_anschalten();
}

//void anzeige (void)
//{
//	switch(zustand)
//	{
//		case 0:
//			lcd_curs(3);				// Cursor auf Position 1 setzen
//	    lcd_str(Wau);				// LCD-Text Masch. Aus
//			break;							// Abschluss
//		case 1:
//			lcd_curs(3);				// Cursor auf Position 1 setzen
//	    lcd_str(Wan);				// LCD-Text Masch. An
//		  break;							// Abschluss
//		case 2:
//			lcd_curs(3);				// Cursor auf Position 1 setzen
//	    lcd_str(Wau);				// LCD-Text Masch. Aus
//			break;							// Abschluss
//		case 3:
//			lcd_curs(23);				// Cursor auf Position 1 setzen
//			lcd_str(Y3A);				// LCD-Text Y Auf
//			break;							// Abschluss
//		case 4:
//			lcd_curs(23);				// Cursor auf Position 1 setzen
//			lcd_str(Y3Z);				// LCD-Text Y Zu
//			break;							// Abschluss
//		case 5:								
//			lcd_curs(63);				// Cursor auf Position 1 setzen
//			lcd_str(Han);				// LCD-Text H An
//			break;							// Abschluss
//		case 6:								
//			lcd_curs(63);				// Cursor auf Position 1 setzen
//			lcd_str(Hau);				// LCD-Text H Aus
//			break;							// Abschluss
//		case 7:
//			lcd_curs(43);				// Cursor auf Position 1 setzen
//	    lcd_str(Man);				// LCD-Text Mtr. An
//		  break;							// Abschluss
//		case 8:
//			lcd_curs(43);				// Cursor auf Position 1 setzen
//	    lcd_str(Mau);				// LCD-Text Mtr. Aus
//		  break;							// Abschluss
//		case 9:
//			lcd_curs(63);				// Cursor auf Position 1 setzen
//	    lcd_str(temp);			// LCD-Text Temperatur zu hoch
//		  break;							// Abschluss
//	}
//}

