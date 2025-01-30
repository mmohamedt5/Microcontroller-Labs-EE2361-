/* 
 * File:   Lab5_main.c
 * Author: mmoha
 *
 * Created on March 21, 2024, 11:03 AM
 */

#include <xc.h>
#include "stdint.h"
#include "I2C_LCD_Library.h"

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


/**
 * Simple Setup function for the PIC24
 */
void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff; // all digital 
}

/**
 * The animation function
 */
void lcdAnimation(void) {
    lcdPrintString("o0000");
}

/**
 * The main function initializes all of the LCD microcontroller functions. It also sets the animation
 */
int main(int argc, char** argv) {
    setup();
    lcdInit();
    lcdSetCursor(0,0);
    
    lcdAnimation();
    while(1) {
        
        lcdCmd(0b00011100);
        delay(250);
    }
}



