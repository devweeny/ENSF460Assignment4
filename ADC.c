/*
 * File:   ADC.c
 * Author: Dylan Wenaas, Isabelle Gaudet, David Anez
 *
 * Created on November 2, 2024, 6:26 PM
 */


#include "xc.h"
#include "ADC.h"

uint16_t do_ADC(void) {
    uint16_t ADCvalue;
    
    AD1CON2bits.VCFG = 0b000;
    
    AD1CON3bits.ADRC = 0;
    
    
    AD1CON1bits.SAMP = 1;
    AD1CON1bits.ADON = 1;
    
    while (!AD1CON1bits.DONE) {}
    ADCvalue = ADC1BUF0;
    
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;
    
    return (ADCvalue);  
}