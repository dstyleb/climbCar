#define LED1 BIT0
#define LED2 BIT1
#define LED3 BIT2
#define LED4 BIT3
#include <msp430.h>
#include <encoder.h>
#include "oled.h"
#include <math.h>
int ir=0;
int setPID=1;//0:motor;1:OpenMVServo;2:irServo


int flag=1;
int CCW=0;
int CW=0;
int p=0;
float servo=0;
int motor=0;
int speedLimit=200;
float speed=0;
float setSpeed=75;
float kp=0.2;
float ki=0.1;
float kd=0.1;
float err=0;
float lastErr=0;
float sumErr=0;
float pOut=0;
float iOut=0;
float dOut=0;
float iLimit=1000;
float ETA=15000;
float R=200;
int keyChanging=0;
char currentKey;
char irStats;
int inited=0;
int running=0;
int updateRequest=1;
int select=0;
float dMod=0.0001;
int edit=0;
float kpS=3.5;
float kiS=0.35;
float kdS=0.35;
float errS=0;
float lastErrS=0;
float sumErrS=0;
float pOutS=0;
float iOutS=0;
float dOutS=0;
float iLimitS=250;
float kpI=1.2;
float kiI=0.02;
float kdI=0;
float errI=0;
float lastErrI=0;
float sumErrI=0;
float pOutI=0;
float iOutI=0;
float dOutI=0;
float iLimitI=1500;
int ledID=0;
int finish=0;
int beep=0;
//ir
float offset=0;
#pragma vector = USART0RX_VECTOR
__interrupt void UART0_RXISR(void){
    inited=1;
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    p++;
    //upto10s
    if(p==1000){
        //P2OUT ^= LED3;
        //OLED_Clear();
        p=0;
    }
    //beep
    if(p%4==0&&finish){
        beep++;
        if(beep%2==0){
            P2OUT|=BIT4;
        }
        if(beep%2==1){
            P2OUT&=~BIT4;
        }
        if(beep>30){
            finish=0;
            beep=0;
            P2OUT&=~BIT4;
        }
    }
    //Stats
    if(p%10==0&&running){
        ledID++;
        if(ledID>3){
            ledID=0;
        }
        switch(ledID){
        case 0:
            P2OUT|=LED1;P2OUT&=~(LED2+LED3+LED4);break;
        case 1:
            P2OUT|=LED2;P2OUT&=~(LED1+LED3+LED4);break;
        case 2:
            P2OUT|=LED3;P2OUT&=~(LED1+LED2+LED4);break;
        case 3:
            P2OUT|=LED4;P2OUT&=~(LED1+LED2+LED3);break;
        }
    }
    //RunningShow
    if(p%100==0&&running==1){
        OLED_ShowNum(48,4,(int)RXBUF0,2,16);
    }
    if(p%10==0&&running==1&&updateRequest==1){
        OLED_Clear();
        OLED_ShowChinese(0,0,7);
        OLED_ShowChinese(36,0,8);
        OLED_ShowChinese(72,0,9);
        OLED_ShowChinese(108,0,10);
        updateRequest=0;
    }
    //SettingShow
    if(p%10==0&&running==0&&updateRequest==1){
        OLED_Clear();
        OLED_ShowString(0,0,"Run!Run!Run!",12);
        OLED_ShowString(0,1,"Kp:",12);
        OLED_ShowString(0,2,"Ki:",12);
        OLED_ShowString(0,3,"Kd:",12);
        OLED_ShowString(0,4,"Speed:",12);
        OLED_ShowString(0,5,"ETA:",12);
        OLED_ShowString(0,6,"PID(0-2):",12);
        OLED_ShowString(0,7,"IR(0-1):",12);
        switch(setPID){
        case 0:
            OLED_ShowNum(84,1,kp*1000,5,12);
            OLED_ShowNum(84,2,ki*1000,5,12);
            OLED_ShowNum(84,3,kd*1000,5,12);
            OLED_ShowNum(84,5,ETA,5,12);
            break;
        case 1:
            OLED_ShowNum(84,1,kpS*1000,5,12);
            OLED_ShowNum(84,2,kiS*1000,5,12);
            OLED_ShowNum(84,3,kdS*1000,5,12);
            OLED_ShowNum(84,5,ETA,5,12);
            break;
        case 2:
            OLED_ShowNum(84,1,kpI*1000,5,12);
            OLED_ShowNum(84,2,kiI*1000,5,12);
            OLED_ShowNum(84,3,kdI*1000,5,12);
            OLED_ShowNum(84,5,ETA,5,12);
            break;
        }
        OLED_ShowNum(84,4,setSpeed,5,12);
        OLED_ShowNum(84,6,setPID,5,12);
        OLED_ShowNum(84,7,ir,5,12);
        if(edit){
            OLED_ShowString(116,select,"?",12);
        }
        else{
            OLED_ShowString(116,select,"<",12);
        }
        updateRequest=0;
    }
    //findLine
    if(p%2==0&&running){
        if((P2IN & 0xe0)!=0x00){
            irStats=P2IN & 0xe0;
            if(irStats==0xe0||irStats==0xC0||irStats==0x60){
                P2OUT&=~(LED1 + LED2);
                running=0;
                P5OUT&=~BIT6;
                servo=0;
                motor=0;
                err=0;
                lastErr=0;
                sumErr=0;
                pOut=0;
                iOut=0;
                dOut=0;
                errS=0;
                lastErrS=0;
                sumErrS=0;
                pOutS=0;
                iOutS=0;
                dOutS=0;
                errI=0;
                lastErrI=0;
                sumErrI=0;
                pOutI=0;
                iOutI=0;
                dOutI=0;
                offset=0;
                updateRequest=1;
                finish=1;
            }
        }
    }
    //key
    if(p%10==0){
        if((P1IN & 0x03)!=0x03&&keyChanging==1){
            currentKey=P1IN & 0x03;
            while((P1IN & 0x03)!=0x03);
            switch(currentKey){
            case 0x02:
                if(running){
                    P2OUT&=~(LED1 + LED2);
                    running=0;
                    P5OUT&=~BIT6;
                    servo=0;
                    motor=0;
                    err=0;
                    lastErr=0;
                    sumErr=0;
                    pOut=0;
                    iOut=0;
                    dOut=0;
                    errS=0;
                    lastErrS=0;
                    sumErrS=0;
                    pOutS=0;
                    iOutS=0;
                    dOutS=0;
                    errI=0;
                    lastErrI=0;
                    sumErrI=0;
                    pOutI=0;
                    iOutI=0;
                    dOutI=0;
                    offset=0;
                }
                else{
                    select++;
                    if(select>7){
                        select=0;
                    }
                }
                updateRequest=1;
                break;
            case 0x01:
                switch(select){
                case 0:
                    running=1;
                    P5OUT|=BIT6;
                    break;
                case 6:
                    setPID++;
                    if(setPID>2){
                        setPID=0;
                    }
                    break;
                case 7:
                    ir++;
                    if(ir>1){
                        ir=0;
                    }
                    break;
                default:
                    edit=~edit;
                }
                updateRequest=1;
                break;
            }
        }
    }
    if(p%10==0){
        if((P1IN & 0x03)!=0x03){
            keyChanging=1;
        }
    }
    //servo
    if(p%10==0&&inited==1&&running==1&&!ir){
        errS=64-(int)RXBUF0;
        sumErrS+=errS;
        pOutS=kpS*errS;
        if(sumErrS>iLimitS){
            sumErrS=iLimitS;
        }
        if(sumErrS<-iLimitS){
            sumErrS=-iLimitS;
        }
        iOutS=kiS*sumErrS;
        dOutS=kdS*(errS-lastErrS);
        lastErrS=errS;
        servo=pOutS+iOutS+dOutS;
    }
    //speed
    if(p%2==0){
        speed=CW-CCW;
        CW=CCW=0;
    }
    //pid
    if(p%2==0&&inited==1&&running==1&&!ir){
        err=speed-setSpeed;
        sumErr+=err;
        pOut=kp*err;
        if(sumErr>iLimit){
            sumErr=iLimit;
        }
        if(sumErr<-iLimit){
            sumErr=-iLimit;
        }
        iOut=ki*sumErr;
        dOut=kd*(err-lastErr);
        lastErr=err;
        motor=pOut+iOut+dOut;
        if(motor+servo/2<0&&motor-servo/2<0)P6OUT=0x50;
        if(motor+servo/2<0&&motor-servo/2>0)P6OUT=0x60;
        if(motor+servo/2<0&&motor-servo/2==0)P6OUT=0x40;
        if(motor+servo/2>0&&motor-servo/2<0)P6OUT=0x90;
        if(motor+servo/2>0&&motor-servo/2>0)P6OUT=0xA0;
        if(motor+servo/2>0&&motor-servo/2==0)P6OUT=0x80;
        if(motor+servo/2==0&&motor-servo/2<0)P6OUT=0x10;
        if(motor+servo/2==0&&motor-servo/2>0)P6OUT=0x20;
        if(motor+servo/2==0&&motor-servo/2==0)P6OUT=0x00;
    }
    //irOffset
    if(p%2==0&&inited&&running&&ir){
        switch(irStats){
        case 0x00://www
            //offset=0;
            break;
        case 0x80://bww
            offset=-100;
            break;
        case 0x40://wbw
            offset=0;
            break;
        case 0x20://wwb
            offset=100;
            break;
        case 0xc0://bbw
            offset=-50;
            break;
        case 0xa0://bwb
            //offset=0;
            break;
        case 0x60://wbb
            offset=50;
            break;
        case 0xe0://bbb
            //offset=0;
            break;
        }
    }
    //irPID
    if(p%2==0&&inited&&running&&ir){
        errI=offset;
        sumErrI+=errI;
        pOutI=kpI*errI;
        if(sumErrI>iLimitI){
            sumErrI=iLimitI;
        }
        if(sumErrI<-iLimitI){
            sumErrI=-iLimitI;
        }
        iOutI=kiI*sumErrI;
        dOutI=kdI*(errI-lastErrI);
        lastErrI=errI;
        servo=pOutI+iOutI+dOutI;
        err=speed-setSpeed;
        sumErr+=err;
        pOut=kp*err;
        if(sumErr>iLimit){
            sumErr=iLimit;
        }
        if(sumErr<-iLimit){
            sumErr=-iLimit;
        }
        iOut=ki*sumErr;
        dOut=kd*(err-lastErr);
        lastErr=err;
        motor=pOut+iOut+dOut;
        if(motor+servo/2<0&&motor-servo/2<0)P6OUT=0x50;
        if(motor+servo/2<0&&motor-servo/2>0)P6OUT=0x60;
        if(motor+servo/2<0&&motor-servo/2==0)P6OUT=0x40;
        if(motor+servo/2>0&&motor-servo/2<0)P6OUT=0x90;
        if(motor+servo/2>0&&motor-servo/2>0)P6OUT=0xA0;
        if(motor+servo/2>0&&motor-servo/2==0)P6OUT=0x80;
        if(motor+servo/2==0&&motor-servo/2<0)P6OUT=0x10;
        if(motor+servo/2==0&&motor-servo/2>0)P6OUT=0x20;
        if(motor+servo/2==0&&motor-servo/2==0)P6OUT=0x00;
    }
}
void stepCW(){
    if(!running){
        P2OUT ^= LED1;
    }
    CW++;
    if(!running&&edit){
        switch(setPID){
        case 0:
            switch(select){
            case 1:
                kp+=dMod;OLED_ShowNum(84,1,kp*1000,5,12);break;
            case 2:
                ki+=dMod;OLED_ShowNum(84,2,ki*1000,5,12);break;
            case 3:
                kd+=dMod;OLED_ShowNum(84,3,kd*1000,5,12);break;
            case 4:
                setSpeed+=dMod*1000;OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 5:
                ETA+=dMod*10000*10;setSpeed=(301.468278843811 - 18.724619285079) / (1 + pow((ETA/5653.52908261534),1.73053649827878)) + 18.724619285079;OLED_ShowNum(84,5,ETA,5,12);OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 6:
                //setPID+=dMod*10;OLED_ShowNum(84,6,setPID,5,12);break;
            case 7:
                //ir+=dMod*10;OLED_ShowNum(84,7,ir,5,12);break;
            }
            break;
        case 1:
            switch(select){
            case 1:
                kpS+=dMod;OLED_ShowNum(84,1,kpS*1000,5,12);break;
            case 2:
                kiS+=dMod;OLED_ShowNum(84,2,kiS*1000,5,12);break;
            case 3:
                kdS+=dMod;OLED_ShowNum(84,3,kdS*1000,5,12);break;
            case 4:
                setSpeed+=dMod*1000;OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 5:
                ETA+=dMod*10000*10;setSpeed=(301.468278843811 - 18.724619285079) / (1 + pow((ETA/5653.52908261534),1.73053649827878)) + 18.724619285079;OLED_ShowNum(84,5,ETA,5,12);OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 6:
                //setPID+=dMod*10;OLED_ShowNum(84,6,setPID,5,12);break;
            case 7:
                //ir+=dMod*10;OLED_ShowNum(84,7,ir,5,12);break;
            }
            break;
        case 2:
            switch(select){
            case 1:
                kpI+=dMod;OLED_ShowNum(84,1,kpI*1000,5,12);break;
            case 2:
                kiI+=dMod;OLED_ShowNum(84,2,kiI*1000,5,12);break;
            case 3:
                kdI+=dMod;OLED_ShowNum(84,3,kdI*1000,5,12);break;
            case 4:
                setSpeed+=dMod*1000;OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 5:
                ETA+=dMod*10000*10;setSpeed=(301.468278843811 - 18.724619285079) / (1 + pow((ETA/5653.52908261534),1.73053649827878)) + 18.724619285079;OLED_ShowNum(84,5,ETA,5,12);OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 6:
                //setPID+=dMod*10;OLED_ShowNum(84,6,setPID,5,12);break;
            case 7:
                //ir+=dMod*10;OLED_ShowNum(84,7,ir,5,12);break;
            }
            break;
        }
        //updateRequest=1;
    }
}
void stepCCW(){
    if(!running){
        P2OUT ^= LED2;
    }
    CCW++;
    if(!running&&edit){
        switch(setPID){
        case 0:
            switch(select){
            case 1:
                kp-=dMod;OLED_ShowNum(84,1,kp*1000,5,12);break;
            case 2:
                ki-=dMod;OLED_ShowNum(84,2,ki*1000,5,12);break;
            case 3:
                kd-=dMod;OLED_ShowNum(84,3,kd*1000,5,12);break;
            case 4:
                setSpeed-=dMod*1000;OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 5:
                ETA-=dMod*10000*10;setSpeed=(301.468278843811 - 18.724619285079) / (1 + pow((ETA/5653.52908261534),1.73053649827878)) + 18.724619285079;OLED_ShowNum(84,5,ETA,5,12);OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 6:
                //setPID-=dMod*10;OLED_ShowNum(84,6,setPID,5,12);break;
            case 7:
                //ir-=dMod*10;OLED_ShowNum(84,7,ir,5,12);break;
            }
            break;
        case 1:
            switch(select){
            case 1:
                kpS-=dMod;OLED_ShowNum(84,1,kpS*1000,5,12);break;
            case 2:
                kiS-=dMod;OLED_ShowNum(84,2,kiS*1000,5,12);break;
            case 3:
                kdS-=dMod;OLED_ShowNum(84,3,kdS*1000,5,12);break;
            case 4:
                setSpeed-=dMod*1000;OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 5:
                ETA-=dMod*10000*10;setSpeed=(301.468278843811 - 18.724619285079) / (1 + pow((ETA/5653.52908261534),1.73053649827878)) + 18.724619285079;OLED_ShowNum(84,5,ETA,5,12);OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 6:
                //setPID-=dMod*10;OLED_ShowNum(84,6,setPID,5,12);break;
            case 7:
                //ir-=dMod*10;OLED_ShowNum(84,7,ir,5,12);break;
            }
            break;
        case 2:
            switch(select){
            case 1:
                kpI-=dMod;OLED_ShowNum(84,1,kpI*1000,5,12);break;
            case 2:
                kiI-=dMod;OLED_ShowNum(84,2,kiI*1000,5,12);break;
            case 3:
                kdI-=dMod;OLED_ShowNum(84,3,kdI*1000,5,12);break;
            case 4:
                setSpeed-=dMod*1000;OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 5:
                ETA-=dMod*10000*10;setSpeed=(301.468278843811 - 18.724619285079) / (1 + pow((ETA/5653.52908261534),1.73053649827878)) + 18.724619285079;OLED_ShowNum(84,5,ETA,5,12);OLED_ShowNum(84,4,setSpeed,5,12);break;
            case 6:
                //setPID-=dMod*10;OLED_ShowNum(84,6,setPID,5,12);break;
            case 7:
                //ir-=dMod*10;OLED_ShowNum(84,7,ir,5,12);break;
            }
            break;
        }
        //updateRequest=1;
    }
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	unsigned char i;
	  BCSCTL1&=~XT2OFF;     //开启XT2振荡器
	  BCSCTL2|=SELM_2+SELS;    //选择MCLK时钟源为XT2CLK；选择SMCLK时钟源为XT2CLK

	  do
	  {
	    IFG1&=~OFIFG;
	    for(i=0;i<100;i++)
	      _NOP();
	  }
	  while((IFG1&OFIFG)!=0);
	  IFG1&=~OFIFG;
    P2DIR |= LED1 + LED2 + LED3 + LED4 + BIT4;
    P2OUT &= ~(LED1 + LED2 + LED3 + LED4 + BIT4);
    P4DIR |= BIT1 + BIT2;
    P4SEL |= BIT1 + BIT2;
    P5DIR |= BIT6 + BIT7;
    P5OUT &= (~BIT6 + BIT7);
    P6DIR |= BIT4 + BIT5 + BIT6 + BIT7;
    P6OUT = 0x50;

    if(ir)inited=1;
    encoderInit();
    TACCR0=10000;//1kHzInterrupt:1000
    TACTL|=TASSEL_2+ID_3+MC_1;
    TACCTL0|=CCIE;
    TBCTL|=TBSSEL_2+TBCLGRP_1+ID_0+MC_3+TBCLR;
    //TBCCTL0|=CCIE;
    TBCCR0=255;
    TBCCTL1=OUTMOD_2;
    //TBCCTL2=OUTMOD_7+CLLD_1;
    TBCCTL2=OUTMOD_2;
    P3SEL |= 0x30;
    ME1 |= UTXE0 + URXE0;
    UCTL0 |= CHAR;
    UTCTL0 |= SSEL0;
    UBR00 = 0x03;
    UBR10 = 0x00;
    UMCTL0 = 0x4A;
    UCTL0 &= ~SWRST;
    IE1 |= URXIE0;
    //_BIS_SR(GIE);
    OLED_Init();
    while(flag){
        TBCCR1=(int)abs(motor+servo/2)<0?0:((int)abs(motor+servo/2)>speedLimit?speedLimit:(int)abs(motor+servo/2));
        TBCCR2=(int)abs(motor-servo/2)<0?0:((int)abs(motor-servo/2)>speedLimit?speedLimit:(int)abs(motor-servo/2));
    }
	return 0;
}
