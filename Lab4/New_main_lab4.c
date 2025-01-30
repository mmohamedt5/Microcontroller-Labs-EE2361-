/* 
 * File:   New_main_lab4.c
 * Author: mmoha
 *
 * Created on March 20, 2024, 11:55 AM
 */
#include <xc.h>
#include "stdint.h"
#include "button.h"
#include "servo.h"

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


volatile unsigned long  int overflow = 0;
volatile unsigned long  int prevOverflow = 0;
volatile unsigned long  int lastEventTime = 0; // calculate the time of the last event
volatile unsigned int prevState = 0; //to track if 0- relase or 1- if press

//Buffer variables
volatile unsigned long long int buffer[3] = {10, 0, 0};  //Buffer with size 3 to place the times in
volatile unsigned int numBuffer = 0;  
volatile unsigned int writeIdx = 0;
volatile unsigned int readIdx = 0;

/*
 * Simple Setup function for the PIC24
 */
void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff; // all digital 
}

/*
 *This is a simple delay function that takes in a msec as its input.
 */
void delay (unsigned int msec) {
    for (int i =0; i<msec; i++) {
        asm("repeat #15998");
        asm("nop");
    }
}
/*
 *This function is The T2 Interrupt where it keeps track of overflow. It also keeps track if two seconds have passed.
 * If 2 seconds have passed, then the servo is set to 0;
 */ 
void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void) {
    _T2IF = 0;
    overflow++;
    
    if (overflow - prevOverflow >= 2 ) {
        setServo(0);
    }
}

/*
 *This function is the Input Capture Interrupt function. It calculates if the triple click is valid.
 * It uses a circular Buffer meaning Whenever a new time is inserted, the first time is overwritten.
 */
void __attribute__((__interrupt__, __auto_psv__)) _IC1Interrupt(void) {
   _IC1IF = 0; // don?t forget to reset your flags to zero!
   
   //setServo(1000000);
   unsigned long int currentEventTime = (TMR2+((long int)(PR2+1)*overflow));
   
   
    //Compare current and last event times. if value is large enough than we move to the next part, if not then it is a bounce
    if ( ((lastEventTime - currentEventTime) > 1000)  || lastEventTime == 0) {// 

        lastEventTime = currentEventTime ;  // save the time

        if (prevState == 0) { //meaning if it was a release or no button press
            prevState = 1 ;  //change the state to a press
            //save the time to circular buffer

            buffer[writeIdx++] = lastEventTime;
            writeIdx %= 3 ; //reset the index
            numBuffer++;
            
            if ( ((buffer[writeIdx - 1] - buffer[writeIdx])  < 31250) ) {  //&& (numBuffer >=3)
                setServo(10000);
                
            }
        //numBuffer = 0;
        } else { //meaning there was a button press, then change state to a release 
            prevState = 0;
            
        }
    }
   prevOverflow = overflow;
}


/*
 * The main function initializes all of the Servo and button functions.
 */
int main(int argc, char** argv) {
    setup();
    initServo();
    initbutton();
    setServo(0);
    int count = 0; 
    
    
    
    while(1) {
        //

    }
}

