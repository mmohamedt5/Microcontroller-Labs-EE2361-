#include <xc.h>
#include "I2C_LCD_Library.h"

/*
 *This is a simple delay function that takes in a msec as its input.
 */
void delay (unsigned int msec) {
    for (int i =0; i<msec; i++) {
        asm("repeat #15998");
        asm("nop");
    }
}

/**
 * This sends a command to the control with the right setup
 * @param package the command that is going to be written
 */
void lcdCmd(char package) {
    IFS3bits.MI2C2IF =0; //clear
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1); //wait for SEN ==0
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b00000000;
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
 
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = package;
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1

    IFS3bits.MI2C2IF =0; //clear  
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN != 0);
   
}
/**
 * This print function prints one character at a time
 * @param package the character asci number to be excuted
 */
void print(char package) {
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1); //wait for SEN ==0
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte w/Rs =1
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    

    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = package; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN != 0);
}
/**
 * This sets the cursor of the lcd
 * @param x sets the row (0,1)
 * @param y sets the column (0, 7)
 */
void lcdSetCursor(char x, char y) {
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2CONbits.SEN = 1; //Start Bit
    while(I2C2CONbits.SEN == 1); //wait for SEN ==0
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b00000000; // Control Byte: CO = 0 RS =0
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0x40*(x) + y + 0x80; // 8-bits consisting of cursor location
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    IFS3bits.MI2C2IF =0; //clear  
    I2C2CONbits.PEN = 1;  //Stop
    while(I2C2CONbits.PEN != 0);
}
/**
 * Prints a string or char array with size 8
 * @param package the char array
 */
void lcdPrintString(char package[8]) {
    lcdSetCursor(0,0);
    IFS3bits.MI2C2IF =0; //clear
    I2C2CONbits.SEN = 1; //Start Bit
    while(I2C2CONbits.SEN == 1); //wait for SEN ==0
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    for (int i = 0; i < 7; i++) {
        IFS3bits.MI2C2IF =0; //clear
        I2C2TRN = 0b11000000; // 8-bits consisting of control byte w/ CO = 1 and R = 1
        while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
        
        IFS3bits.MI2C2IF =0; //clear
        I2C2TRN = package[i]; // 8-bits consisting of char
        while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    }
    
    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte w/ CO = 0 and R = 1
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1

    IFS3bits.MI2C2IF =0; //clear
    I2C2TRN = package[7]; // 8-bits consisting of char
    while(IFS3bits.MI2C2IF != 1); //Wait for IFS3bits.MI2C2IF == 1
    
    
    IFS3bits.MI2C2IF =0; //clear  
    I2C2CONbits.PEN = 1;  //Stop
    while(I2C2CONbits.PEN != 0);
    
}
/**
 * The initialization function which sets the BRG rate and sets up the Microcontroller
 */
void lcdInit(void) {
    I2C2BRG = 0x9D;   //100KHZ
    IFS3bits.MI2C2IF = 0;   //clear Interrupt Flag
    I2C2CONbits.I2CEN = 1;
    
    delay(40) ;
    
    lcdCmd(0b00111000);
    lcdCmd(0b00111001);
    lcdCmd(0b00010100);
    lcdCmd(0b01110000);
    lcdCmd(0b01010110);
    lcdCmd(0b01101100);
    
    delay(200); //power reset
    
    lcdCmd(0b00111000);
    lcdCmd(0b00001100);
    lcdCmd(0b00000001);
    
    delay(1);
    
}
