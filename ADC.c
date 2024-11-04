/*
 * File:   ADC.c
 * Author: Dylan Wenaas, Isabelle Gaudet, David Anez
 *
 * Created on November 2, 2024, 6:26 PM
 */


#include "xc.h"
#include "ADC.h"
#include "clkChange.h"

uint16_t do_ADC(void) {
    uint16_t ADCvalue;
    
    TRISAbits.TRISA3 = 1;
    AD1PCFGbits.PCFG5 = 0;
   
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

void IOinit() {
       
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    newClk(500);
    
    //T3CON config
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T3CONbits.TCKPS = 3; // set prescaler to 1:256
    T3CONbits.TCS = 0; // use internal clock
    T3CONbits.TSIDL = 0; //operate in idle mode
    IPC2bits.T3IP = 2; //7 is highest and 1 is lowest pri.
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1; //enable timer interrupt

    /* Let's set up some I/O */
    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8 = 0;
    
    TRISAbits.TRISA4 = 1;
    CNPU1bits.CN0PUE = 1;
    CNEN1bits.CN0IE = 1;
    
    TRISBbits.TRISB4 = 1;
    CNPU1bits.CN1PUE = 1;
    CNEN1bits.CN1IE = 1;
    
    TRISAbits.TRISA2 = 1;
    CNPU2bits.CN30PUE = 1;
    CNEN2bits.CN30IE = 1;
    
    /* Let's clear some flags */
    PB1_event = 0;
    mode = 0;
    tmr3_event = 0;
    
    IPC4bits.CNIP = 6;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    
    /* Let's set up our UART */    
    InitUART2();
    
    TMR3 = 0;
    PR3 = 980;
    T3CONbits.TON = 1;
    
}

void IOcheck() {
    
    if (tmr3_event) { // Clock interrupt to run once a second

        Disp2String("\033[2J"); // Clears the terminal
        Disp2String("\033[H"); // Sets cursor to top left of terminal
        LATBbits.LATB8 ^= 1;

//        uint16_t result = do_ADC();
        uint16_t result = 0;
        Disp2Hex(result);
        tmr3_event = 0;
    }

    if (PB1_event) {
        Disp2String("\033[2J"); // Clears the terminal
        Disp2String("\033[H"); // Sets cursor to top left of terminal
        mode ^= 1;
        Disp2String("Mode: ");
        Disp2Dec(mode);
        PB1_event = 0;
    }
    Idle();

}