/*
 * tft_driver.c
 *
 *  Created on: 26-Aug-2022
 *      Author: AustinA
 */


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "tft_driver.h"


#define GPIOC	TFT_12_PORT
#define GPIOB	0x123
#define INVERT_TFT_GPIO	1

//#define RS	TFT_DISP_RST_L  //Not present
#define RES	TFT_DISP_RST_L_NUM
#define nRD	TFT_DISP_RD_L_NUM
#define nWR	TFT_DISP_WR_L_NUM
#define D_C	TFT_DISP_D_C_NUM



#define GPIO_ResetBits(port,pinNum) 	//Cy_GPIO_Clr(port, pinNum)
#define GPIO_SetBits(port,pinNum) 		//Cy_GPIO_Set(port, pinNum)



void tftGpioDebugRead()
{
	printf("[ DEBUG TFT ] TFT WRITE PORT : %X\r\n",
			Cy_GPIO_Read(TFT_9_PORT, TFT_DISP_DB8_NUM)  |
			( Cy_GPIO_Read(TFT_9_PORT, TFT_DISP_DB9_NUM)  << 1 ) |
			( Cy_GPIO_Read(TFT_9_PORT, TFT_DISP_DB10_NUM) << 2 ) |
			( Cy_GPIO_Read(TFT_9_PORT, TFT_DISP_DB11_NUM) << 3 ) |
			( Cy_GPIO_Read(TFT_9_PORT, TFT_DISP_DB12_NUM) << 4 ) |
			( Cy_GPIO_Read(TFT_5_PORT, TFT_DISP_DB13_NUM) << 5 ) |
			( Cy_GPIO_Read(TFT_7_PORT, TFT_DISP_DB14_NUM) << 6 ) |
			( Cy_GPIO_Read(TFT_7_PORT, TFT_DISP_DB15_NUM) << 7 )  );

}


void GPIO_Write(uint32_t port , uint16_t data)
{

	if(port != GPIOB) return;

//	printf("[DRBUG TFT] : TFT Write Data : %X\r\n",data);

#ifdef INVERT_TFT_GPIO
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB8_NUM, ( data >> 0 ) & 0x01);	// bit 8
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB9_NUM, ( data >> 1 ) & 0x01);	// bit 9
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB10_NUM, ( data >> 2 ) & 0x01);	// bit 10
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB11_NUM, ( data >> 3 ) & 0x01);	// bit 11
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB12_NUM, ( data >> 4 ) & 0x01);	// bit 12
	Cy_GPIO_Write(TFT_5_PORT, TFT_DISP_DB13_NUM, ( data >> 5 ) & 0x01);	// bit 13
	Cy_GPIO_Write(TFT_7_PORT, TFT_DISP_DB14_NUM, ( data >> 6 ) & 0x01);	// bit 14
	Cy_GPIO_Write(TFT_7_PORT, TFT_DISP_DB15_NUM, ( data >> 7 ) & 0x01);	// bit 15


#else
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB8_NUM, ( data >> 7 ) & 0x01);	// bit 8
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB9_NUM, ( data >> 6 ) & 0x01);	// bit 9
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB10_NUM, ( data >> 5 ) & 0x01);	// bit 10
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB11_NUM, ( data >> 4 ) & 0x01);	// bit 11
	Cy_GPIO_Write(TFT_9_PORT, TFT_DISP_DB12_NUM, ( data >> 3 ) & 0x01);	// bit 12
	Cy_GPIO_Write(TFT_5_PORT, TFT_DISP_DB13_NUM, ( data >> 2 ) & 0x01);	// bit 13
	Cy_GPIO_Write(TFT_7_PORT, TFT_DISP_DB14_NUM, ( data >> 1 ) & 0x01);	// bit 14
	Cy_GPIO_Write(TFT_7_PORT, TFT_DISP_DB15_NUM, ( data >> 0 ) & 0x01);	// bit 15
#endif

//	tftGpioDebugRead();
}

/*******************************************************************************/
void TFT_24_7789_Write_Command(unsigned int command)
{
	GPIO_ResetBits(GPIOC, D_C);
//	GPIO_ResetBits(GPIOC, CS1);	Hardware wise set to LOW, hence no need to manipulate CS
//	GPIO_ResetBits(GPIOC, RS);
	GPIO_SetBits(GPIOC, nRD);
	GPIO_ResetBits(GPIOC, nWR);
	GPIO_Write(GPIOB, command);
//	TFT_delay(10);
	GPIO_SetBits(GPIOC, nWR);
//	TFT_delay(1);
}
/*******************************************************************************/
void TFT_24_7789_Write_Data(unsigned int data1)
{
	GPIO_SetBits(GPIOC, D_C);
	GPIO_Write(GPIOB, data1);
//	GPIO_SetBits(GPIOC, RS);
	GPIO_ResetBits(GPIOC, nWR);
//	TFT_delay(1);
	GPIO_SetBits(GPIOC, nWR);
}

void TFT_delay(uint32_t time)
{
	cyhal_system_delay_us(time);
}

/*******************************************************************************/


void TFT_24_7789_Init(void)
{
	int n;
	uint8_t readDataStream[10];

//	const st7789v_pdl_pins_t st7789v_pdl_pins = {
//			.db08 = TFT_DISP_DB8,
//			.db09 = TFT_DISP_DB9,
//			.db10 = TFT_DISP_DB10,
//			.db11 = TFT_DISP_DB11,
//			.db12 = TFT_DISP_DB12,
//			.db13 = TFT_DISP_DB13,
//			.db14 = TFT_DISP_DB14,
//			.db15 = TFT_DISP_DB15,
//		    .nrd  = TFT_DISP_RD_L,
//		    .nwr  = TFT_DISP_WR_L,
//		    .dc   = TFT_DISP_D_C,
//		    .rst  = TFT_DISP_RST_L
//	};
//#ifndef TFT_12_PORT
//	st7789v_pdl_init8(&st7789v_pdl_pins);
//#else
//	st7789v_pdl_pinsSet(&st7789v_pdl_pins);
//#endif



//	st7789v_pdl_write_command(0x04);
//	st7789v_pdl_read_data_stream(readDataStream,4);
//	printf("TFT ID : %X %X %X\r\n",readDataStream[1],readDataStream[2],readDataStream[3]);
//
//	st7789v_pdl_write_command(0x0DA);
//	printf("TFT ID : %X\r\n",st7789v_pdl_read_data());
//	st7789v_pdl_write_command(0x0DB);
//	printf("TFT ID : %X\r\n",st7789v_pdl_read_data());
//	st7789v_pdl_write_command(0x0DC);
//	printf("TFT ID : %X\r\n",st7789v_pdl_read_data());
//
//
//
//
//	st7789v_pdl_write_command(0x003A);
//	st7789v_pdl_write_data(0x55);//COLMOD: Interface Pixel format
//
//
////	st7789v_pdl_write_command(0xB5);
////	st7789v_pdl_write_data(0x0F);
//		st7789v_pdl_write_command(0x39);
//
//	tft_printDisplayStatus();
//	st7789v_pdl_write_command(0x09);
//	//	st7789v_pdl_read_data_stream(readDataStream,4);
//	//	printf("Display Status : %X %X %X %X %X \r\n",readDataStream[0],readDataStream[1],readDataStream[2],readDataStream[3],readDataStream[4]);
//		printf("Display Status : %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf("\r\n\n");
//
//
////	st7789v_pdl_write_command(0xB5);
////	st7789v_pdl_write_data(0x1F);
//	st7789v_pdl_write_command(0x38);
//
//	st7789v_pdl_write_command(0x003A);
//	st7789v_pdl_write_data(0x66);//COLMOD: Interface Pixel format
//
//
//	tft_printDisplayStatus();
//	st7789v_pdl_write_command(0x09);
//	//	st7789v_pdl_read_data_stream(readDataStream,4);
//	//	printf("Display Status : %X %X %X %X %X \r\n",readDataStream[0],readDataStream[1],readDataStream[2],readDataStream[3],readDataStream[4]);
//		printf("Display Status : %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf(" %X",st7789v_pdl_read_data());
//		printf("\r\n");


//	GPIO_ResetBits(GPIOC, CS1);	//Hardware wise set to LOW, hence no need to manipulate CS
	GPIO_SetBits(GPIOC, nRD);
//	Cy_GPIO_Set(TFT_DISP_RD_L_PORT,TFT_DISP_RD_L_NUM);
	GPIO_ResetBits(GPIOC, nWR);
	GPIO_ResetBits(GPIOC, RES);
	TFT_delay(100);
	GPIO_SetBits(GPIOC, RES);
	TFT_delay(100);
	TFT_24_7789_Write_Command(0x0011);//exit SLEEP mode
	TFT_delay(100);
	TFT_24_7789_Write_Command(0x0036);
	TFT_24_7789_Write_Data(0x0080);//MADCTL: memory data access control
	TFT_24_7789_Write_Command(0x003A);
	TFT_24_7789_Write_Data(0x0066);//COLMOD: Interface Pixel format
	TFT_24_7789_Write_Command(0x00B2);
	TFT_24_7789_Write_Data(0x000C);
	TFT_24_7789_Write_Data(0x0C);
	TFT_24_7789_Write_Data(0x00);
	TFT_24_7789_Write_Data(0x33);
	TFT_24_7789_Write_Data(0x33);//PORCTRK: Porch setting
	TFT_24_7789_Write_Command(0x00B7);
	TFT_24_7789_Write_Data(0x0035);//GCTRL: Gate Control
	TFT_24_7789_Write_Command(0x00BB);
	TFT_24_7789_Write_Data(0x002B);//VCOMS: VCOM setting
	TFT_24_7789_Write_Command(0x00C0);
	TFT_24_7789_Write_Data(0x002C);//LCMCTRL: LCM Control
	TFT_24_7789_Write_Command(0x00C2);
	TFT_24_7789_Write_Data(0x0001);
	TFT_24_7789_Write_Data(0xFF);//VDVVRHEN: VDV and VRH Command Enable
	TFT_24_7789_Write_Command(0x00C3);
	TFT_24_7789_Write_Data(0x0011);//VRHS: VRH Set
	TFT_24_7789_Write_Command(0x00C4);
	TFT_24_7789_Write_Data(0x0020);//VDVS: VDV Set
	TFT_24_7789_Write_Command(0x00C6);
	TFT_24_7789_Write_Data(0x000F);//FRCTRL2: Frame Rate control in normal mode
	TFT_24_7789_Write_Command(0x00D0);
	TFT_24_7789_Write_Data(0x00A4);
	TFT_24_7789_Write_Data(0xA1);//PWCTRL1: Power Control 1
	TFT_24_7789_Write_Command(0x00E0);
	TFT_24_7789_Write_Data(0x00D0);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x0005);
	TFT_24_7789_Write_Data(0x000E);
	TFT_24_7789_Write_Data(0x0015);
	TFT_24_7789_Write_Data(0x000D);
	TFT_24_7789_Write_Data(0x0037);
	TFT_24_7789_Write_Data(0x0043);
	TFT_24_7789_Write_Data(0x0047);
	TFT_24_7789_Write_Data(0x0009);
	TFT_24_7789_Write_Data(0x0015);
	TFT_24_7789_Write_Data(0x0012);
	TFT_24_7789_Write_Data(0x0016);
	TFT_24_7789_Write_Data(0x0019);//PVGAMCTRL: Positive Voltage Gamma control
	TFT_24_7789_Write_Command(0x00E1);
	TFT_24_7789_Write_Data(0x00D0);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x0005);
	TFT_24_7789_Write_Data(0x000D);
	TFT_24_7789_Write_Data(0x000C);
	TFT_24_7789_Write_Data(0x0006);
	TFT_24_7789_Write_Data(0x002D);
	TFT_24_7789_Write_Data(0x0044);
	TFT_24_7789_Write_Data(0x0040);
	TFT_24_7789_Write_Data(0x000E);
	TFT_24_7789_Write_Data(0x001C);
	TFT_24_7789_Write_Data(0x0018);
	TFT_24_7789_Write_Data(0x0016);
	TFT_24_7789_Write_Data(0x0019);//NVGAMCTRL: Negative Voltage Gamma control
	TFT_24_7789_Write_Command(0x002A);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x00EF);//X address set
	TFT_24_7789_Write_Command(0x002B);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x0000);
	TFT_24_7789_Write_Data(0x0001);
	TFT_24_7789_Write_Data(0x003F);//Y address set
	TFT_delay(10);

#define TEST_BYTE_R 0x0000
#define TEST_BYTE_G 0xFFFF
#define TEST_BYTE_B 0x0000
	TFT_24_7789_Write_Command(0x002C);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);
	TFT_24_7789_Write_Data(TEST_BYTE_R);
	TFT_24_7789_Write_Data(TEST_BYTE_G);
	TFT_24_7789_Write_Data(TEST_BYTE_B);


}
