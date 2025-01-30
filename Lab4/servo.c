#include "servo.h"
#include <xc.h>
/* This function takes a period of 0 and PR3 and produces a Pulse-Width 
 * between 0 and PR3 
 * Val = 
 */
void setServo(int Val) {
    OC1RS = Val;
}

/* This function initializes the Servo motor with the correct configurations
 * 
 */
void initServo(void) {
    
    TRISBbits.TRISB6 = 0; 
    
    //Allows the PIC24 to change one of the pins to output compare
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock 
    RPOR3bits.RP6R = 18; 
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock 
    
    OC1CON =0;
    OC1CONbits.OCTSEL=0b1;
    OC1CONbits.OCM=0b110;  //Control register for OC1
    OC1R=3;
    OC1RS = 0;
    
    
    
    T3CON=0;
    T3CONbits.TCKPS =0b01; // Pre scaler 1,8,64,256   
    TMR3 = 0;   
    PR3 = 40000 - 1;   //20ms

    T3CONbits.TON=1; 

}
