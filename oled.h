#ifndef __OLED_H
#define __OLED_H
#include <msp430.h>
//#include	<io430.h>
//#include	<in430.h>
//#include	<intrinsics.h>
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据



#define		Set_Bit(val, bitn)		(val |= (/*1 <<*/(bitn)))		//竚琘 1
#define		Clr_Bit(val, bitn)		(val &= ~(/*1<<*/(bitn)))		//竚琘 0
#define		Get_Bit(val, bitn)		(val & (1<<(bitn)) )		//眔琘
//----------------------------------------------------------------------------------
//OLED SSD1306 SPI  时钟D0
//#define		OLED_SSD1306_SCLK_DIR			(P1DIR)
//#define		OLED_SSD1306_SCLK_OUT			(P1OUT)
#define		OLED_SSD1306_SCLK_PIN_NUM		(BIT4)

#define		OLED_SSD1306_SCLK_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_SCLK_PIN_NUM))
#define		OLED_SCLK_Set()			        (Set_Bit(P5OUT,OLED_SSD1306_SCLK_PIN_NUM))
#define		OLED_SCLK_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_SCLK_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 SPI 数据D1
//#define		OLED_SSD1306_SDIN_DIR			(P1DIR)
//#define		OLED_SSD1306_SDIN_OUT			(P1OUT)
#define		OLED_SSD1306_SDIN_PIN_NUM		(BIT3)

#define		OLED_SSD1306_SDIN_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_SDIN_PIN_NUM))
#define		OLED_SDIN_Set()				(Set_Bit(P5OUT,OLED_SSD1306_SDIN_PIN_NUM))
#define		OLED_SDIN_Clr()			        (Clr_Bit(P5OUT,OLED_SSD1306_SDIN_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 复位/RES
//#define		OLED_SSD1306_RST_DIR			(P1DIR)
//#define		OLED_SSD1306_RST_OUT			(P1OUT)
#define		OLED_SSD1306_RST_PIN_NUM		(BIT2)

#define		OLED_SSD1306_RST_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_RST_PIN_NUM))
#define		OLED_RST_Set()				(Set_Bit(P5OUT,OLED_SSD1306_RST_PIN_NUM))
#define		OLED_RST_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_RST_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 数据/命令DC
//#define		OLED_SSD1306_DC_DIR				(P1DIR)
//#define		OLED_SSD1306_DC_OUT				(P1OUT)
#define		OLED_SSD1306_DC_PIN_NUM			(BIT1)

#define		OLED_SSD1306_DC_IO_INIT			(Set_Bit(P5DIR,OLED_SSD1306_DC_PIN_NUM))
#define		OLED_DC_Set()				(Set_Bit(P5OUT,OLED_SSD1306_DC_PIN_NUM))
#define		OLED_DC_Clr() 				(Clr_Bit(P5OUT,OLED_SSD1306_DC_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 片选CS
//#define		OLED_SSD1306_CE_DIR				(P1DIR)
//#define		OLED_SSD1306_CE_OUT				(P1OUT)
#define		OLED_SSD1306_CE_PIN_NUM			(BIT0)

#define		OLED_SSD1306_CE_IO_INIT			(Set_Bit(P5DIR,OLED_SSD1306_CE_PIN_NUM))
#define		OLED_CS_Set()				(Set_Bit(P5OUT,OLED_SSD1306_CE_PIN_NUM))
#define		OLED_CS_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_CE_PIN_NUM))
//----------------------------------------------------------------------------------
//开发板LED灯

#define		LED_PIN_NUM			(BIT0)

#define		LED_IO_INIT		        	(Set_Bit(P2DIR,LED_PIN_NUM))
#define		LED_Set()				(Set_Bit(P2OUT,LED_PIN_NUM))
#define		LED_Clr()				(Clr_Bit(P2OUT,LED_PIN_NUM))
//----------------------------------------------------------------------------------
					   

void delay_ms(unsigned int ms);
//OLED控制用函数
void delay_ms(unsigned int ms);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Char_Size);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
void OLED_ShowChinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
void OLED_Init(void);
#endif  
	 



