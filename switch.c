// Deklaration ***********************************************

#include <xc866.h>

void init (void);
void zustand (void);

unsigned char Waus[]="Waschmaschine Aus";   	// LCD-Text Waschmaschine Aus
unsigned char Wan[]="Waschmaschine An";   	  // LCD-Text Waschmaschine An

unsigned int i;

// LCD Funktionen
   extern void lcd_init(void);		      // Init LCD
   extern void lcd_clr(void);	 	     	  // LCD Löschen 
   extern void lcd_byte(char);  	     	// Ausgabe unsigned char  => 3 stellig
   extern void lcd_str(char *ptr);	    // Ausgabe String
   extern void lcd_curs(char);		      // Cursor setzen
   extern void asc_out(char);	   		    // Ausgabe eines ASCII Zeichens
   extern void lcd_int(unsigned int);	  // Ausgabe Int Wert => 4 stellig

//(Main)*****

void main (void)
{
	init();
	while(1)
	{
		i=1;
		zustand();
	}
}

//(Funktionen)*****

void init (void)
{
	lcd_init();														// Initiierung des Bildschirms
	lcd_clr();														// Löschen des Bildschirms
}

void zustand (void)
{
	switch(i)
	{
		case 0:
			lcd_curs(1);				// Cursor auf Position 1 setzen
	    lcd_str(Wan);
		  break;
		default:
			lcd_curs(1);				// Cursor auf Position 1 setzen
	    lcd_str(Waus);
			break;
	}
}
			
			
		

