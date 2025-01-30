#include "moha_buffer.h"
#include <xc.h>
#include <stdio.h>

#define LEN 128
#define SAMPLE 128
//Buffer variables
unsigned long int buffer[LEN];
int end = 0;

/**
 * This function puts an element in the buffer
 * @param newValue : the new variable to be put in.
 */
void putVal(int newValue){
    buffer[end++] = newValue; 
    end %= LEN;
    
}	
/**
 * This function calculates the average of the elements in the Buffer
 * @return  the average
 */
int getAvg(){
    unsigned long int sum = 0;   
    for (int i =0; i< SAMPLE; i++) {
        sum+= buffer[i];
    }
    int avg = sum/SAMPLE;
    return  avg;
}         	

/**
 * This function initializes the Buffer to 0
 */
void initBuffer(){		// set all buffer values to zero
    for (int i = 0; i<LEN; i++) {
        buffer[i] = 0;
    }
}
