// Birk Nov2007
// Bibliotheks Funktionen für ADC des XC866:
// einmal muss adcinit() aufgerufen werden,
// mit wert = ADCIN (kanal)  kann ein Analogwert in 10 Bit 
// Auflösung eingelsen werden . Kanal = 4 .. 7 möglich !!

//  ADC Funktionen :
#include <xc866.h>
//*************************************++
 // Prototypen:
void adc_init(void);
unsigned int adc_in (unsigned char kanal);
unsigned char ADC_Busy(void);

//*******************************************
void adc_init(void)
{
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv 
	ADC_GLOBCTR = 0x70;	// ADC Global Control register
	ADC_PRAR	= 0x90;	// Prio und Arbitrationsreg
	SFR_PAGE(_ad1,noSST);	// Page 1 aktiv
	ADC_CHCTR0 	= 0x00;	// Channel Control Register
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv
	ADC_INPCR0	= 00;	// Sample Time Control
	SFR_PAGE(_ad4,noSST);	// Page 4 aktiv
	ADC_RCR0	= 0xc0;	// Result Control Reg. 0
	SFR_PAGE(_ad5,noSST);	// Page 5 aktiv
	ADC_CHINPR	= 0x00;	// Channel Interrupt Node Pointer
	ADC_EVINPR	= 0x00;	// Event Interrupt Reg
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv
	ADC_LCBR	= 0xb7	;	// Check boundery
	ADC_ETRCR	= 0x00	;	// Ext. Trigger Control
	SFR_PAGE(_ad6,noSST);	// Page 6 aktiv
	ADC_QMR0	= 0x00;		//Queue Mode
	ADC_CRMR1	= 0x01;		// Conv request
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv
	ADC_GLOBCTR  |= 0x80;	// On ADC
}
unsigned int adc_in(unsigned char kanal)
{	// Kanal 4.. 7 ist möglich !
	 
	ADC_PAGE = 0x06;
	ADC_CRPR1= 1 << kanal ;	// ANx gewählt 
	while(ADC_Busy());
	ADC_PAGE = 0x02;
	return ((ADC_RESR0H<<2)+ (ADC_RESR0L>>6) );
}
unsigned char ADC_Busy(void)
{	unsigned char bsyresult;

	SFR_PAGE(_ad0,SST1);	// Page 0 aktiv	
	bsyresult = (ADC_GLOBSTR & 0x01);
	SFR_PAGE(_ad0,RST1);	// Page 0 aktiv
	return(bsyresult);
}
