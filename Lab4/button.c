#include <xc.h>
#include "button.h"
/* This function initializes the button with the correct configurations. 
 * It allows for the PIC to read the input and capture it using TMR2.
 * The Input Capture is set to capture every edge.
 */
void initbutton() {
    TRISBbits.TRISB8 = 1;
    CNPU2bits.CN22PUE=1;
    
    //Unlocking and locking the pin configuration map for the Input Compare
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock 
    RPINR7bits.IC1R = 8; 
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock 
    
    T2CON = 0;
    T2CONbits.TCKPS = 0b11;
    PR2 = 62499;
    TMR2 = 0;
    _T2IE = 1;
    _T2IF = 0;
    
    //For the Input Compare
    IC1CON = 1;
    IC1CONbits.ICTMR = 1;
    IC1CONbits.ICM = 0b001; //capture every edge
    
    _IC1IF = 0;
    _IC1IE = 1;

    T2CONbits.TON = 1;
}
