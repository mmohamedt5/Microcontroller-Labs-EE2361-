/* 
 * File:   lab6_main.c
 * Author: mmoha
 *
 * Created on April 4, 2024, 11:23 AM
 */

#include <xc.h>
#include "stdint.h"
#include <stdio.h>
#include "I2C_LCD_Library.h"
#include "moha_buffer.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

volatile int adValue;
volatile char adStr[20];

/**
 * Simple Setup function for the PIC24
 */
void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff; // all digital 
}

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void) {
    _AD1IF = 0; 
    putVal(ADC1BUF0); //adds the reading from the input  
}

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;

    adValue = getAvg();
    	sprintf(adStr, "%6.4f V", (3.3/1024)*adValue);  // ?x.xxxx V?
                       // 6.4 in the format string ?%6.4f? means 6 placeholders for the whole
                       // floating-point number, 4 of which are for the fractional part.
        lcdPrintString(adStr);
}
void setupTimer1() {
    TMR1 = 0; 
    T1CON = 0; 
    T1CONbits.TCKPS = 0b11; //256 pre
    PR1 = 6249;   //100ms timer
    
    IFS0bits.T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 1;
}


void adcInit(void) {
    TRISAbits.TRISA0 = 1;
    
    AD1PCFGbits.PCFG0 = 0;  //setup
    
    AD1CON2bits.VCFG = 0b000;
    AD1CON3bits.ADCS = 0b00000001;
    AD1CON1bits.SSRC = 0b010;  //Timer 3 Compare ens Sampling and starts Conversio
    AD1CON3bits.SAMC = 0b1; //
    AD1CON1bits.FORM = 0b00;
    
    AD1CON1bits.ASAM = 0b1;
    AD1CON2bits.SMPI = 0b0000;
    AD1CON1bits.ADON = 0b1;
    
    _AD1IF = 0; 
    _AD1IE = 1; 
    
    TMR3 = 0; 
    T3CON = 0; 
    T3CONbits.TCKPS = 0b10; 
    PR3 = 15624; 
    T3CONbits.TON = 1;
}
int main() {
    setup();
    lcdInit();
    adcInit();
    initBuffer();
    lcdSetCursor(0,0);
    setupTimer1();

    while(1) {

    }
}


