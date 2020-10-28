#include "encoder.h"
void encoderInit(){
    P1OUT |= (ENCODER_A+ENCODER_B);
    P1IFG &= ~ENCODER_A;
    P1IE |= ENCODER_A;
    __enable_interrupt();
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IN & ENCODER_B){
        stepCW();
    }else{
        stepCCW();
    }
    P1IFG &= ~ENCODER_A;
}
