/* 
 * File:   I2C_LCD_Library.h
 * Author: mmoha
 *
 * Created on April 3, 2024, 12:17 PM
 */

#ifndef I2C_LCD_LIBRARY_H
#define	I2C_LCD_LIBRARY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void delay (unsigned int msec);
void lcdCmd(char package);
void print(char package);
void lcdSetCursor(char x, char y);
void lcdPrintString(char package[8]);
void lcdInit(void);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_LCD_LIBRARY_H */

