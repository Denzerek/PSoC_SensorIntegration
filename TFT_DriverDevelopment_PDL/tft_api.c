/*
 * tft_api.c
 *
 *  Created on: 27-Aug-2022
 *      Author: AustinA
 */


#include "tft_api.h"
#include "tft_driver.h"

#define VERBOSE_REG_DEBUG	1

void tft_getDisplayStatus(uint8_t* data)
{
	st7789v_pdl_write_command(0x09);
	st7789v_pdl_read_data_stream(data,5);
}



void tft_printDisplayStatus()
{
	tft_display_status_t tft_display_status;
	tft_getDisplayStatus(&tft_display_status);
	printf("\r\n[ TFT ] : Display Status : %X\r\n",tft_display_status);
#if VERBOSE_REG_DEBUG
	printf(" BSTON : %X\t ST23 : %X\t ST15 : %X\t GCS1 : %X\r\n",tft_display_status.BSTON,tft_display_status.ST23,tft_display_status.ST15,tft_display_status.GCS1);
	printf("   MY  : %X\tIFPF2 : %X\t ST14 : %X\t GCS0 : %X\r\n",tft_display_status.MY,tft_display_status.IFPF2,tft_display_status.ST14,tft_display_status.GCS0);
	printf("   MX  : %X\tIFPF1 : %X\tINVON : %X\t TEM : %X\r\n",tft_display_status.MX,tft_display_status.IFPF1,tft_display_status.INVON,tft_display_status.TEM);
	printf("   MV  : %X\tIFPF0 : %X\t ST12 : %X\r\n",tft_display_status.MV,tft_display_status.IFPF0,tft_display_status.ST12);
	printf("   ML  : %X\tIDMON : %X\t ST11 : %X\r\n",tft_display_status.ML,tft_display_status.IDMON,tft_display_status.ST11);
	printf("   RGB : %X\tPTLON : %X\tDISON : %X\r\n",tft_display_status.RGB,tft_display_status.PTLON,tft_display_status.DISON);
	printf("   MH  : %X\tSLOUT : %X\t TEON : %X\r\n",tft_display_status.MH,tft_display_status.SLOUT,tft_display_status.TEON);
	printf("  ST24 : %X\tNORON : %X\t GCS2 : %X\r\n",tft_display_status.ST24,tft_display_status.NORON,tft_display_status.GCS2);

#else
	if(tft_display_status.BSTON)
		printf("\tBooster Voltage ON\r\n");
	else
		printf("\tBooster Voltage OFF\r\n");


	if(tft_display_status.MY)
		printf("\tRow Address order Decrement\r\n");
	else
		printf("\tRow Address order Increment\r\n");

	if(tft_display_status.MX)
		printf("\tColumn Address order Decrement\r\n");
	else
		printf("\tColumn Address order Increment\r\n");

	if(tft_display_status.MX)
		printf("\tRow/Column Exchange \r\n");
	else
		printf("\tRow/Column Exchange Normal\r\n");

	if(!tft_display_status.ML)
		printf("\tScan Address Order Decrement\r\n");
	else
		printf("\tScan Address Order Increment\r\n");

	if(tft_display_status.RGB)
		printf("\tBGR Format\r\n");
	else
		printf("\tRGB Format\r\n");

	if(tft_display_status.MH)
		printf("\tLCD refresh Right to Left, when MADCTL (36h) D2=’1’\r\n");
	else
		printf("\tLCD refresh Left to Right, when MADCTL (36h) D2=’0’\r\n");

	switch(tft_display_status.IFPF0 | tft_display_status.IFPF1 << 1 |
			tft_display_status.IFPF2 << 2){
	case 3:
		printf("\t12 bit / pixel \r\n");
		break;
	case 5:
		printf("\t16 bit / pixel \r\n");
		break;
	case 6:
		printf("\t18 bit / pixel \r\n");
		break;
	case 7:
		printf("\t16M truncated \r\n");
		break;
	default:
		printf("\tInterface Color Pixel Format Not Defined\r\n");
		break;
	}

	if(tft_display_status.IDMON)
		printf("\t[ WARNING ] : IDLE Mode ON\r\n");
	else
		printf("\tIDLE Mode OFF\r\n");

	if(tft_display_status.PTLON)
		printf("\tPartial Mode ON\r\n");
	else
		printf("\tPartial Mode OFF\r\n");

	if(tft_display_status.SLOUT)
		printf("\tDisplay Awake\r\n");
	else
		printf("\t[ WARNING ] : Display in Sleep\r\n");

	if(tft_display_status.NORON)
		printf("\tNormal Display\r\n");
	else
		printf("\t[ WARNING ] : Partial Display\r\n");

	if(tft_display_status.ST15)
		printf("\tVertical Scrolling ON\r\n");
	else
		printf("\tVertical Scrolling OFF\r\n");

	if(tft_display_status.INVON)
		printf("\t[ WARNING ] : Inversion ON\r\n");
	else
		printf("\tInversion OFF\r\n");

	if(tft_display_status.DISON)
		printf("\tDisplay ON\r\n");
	else
		printf("\t[ WARNING ] : Display OFF\r\n");

	if(tft_display_status.TEON)
		printf("\tTearing effect line ON\r\n");
	else
		printf("\tTearing effect line OFF\r\n");

	if(tft_display_status.TEM)
		printf("\tTearing effect line mode 1\r\n");
	else
		printf("\tTearing effect line mode 2\r\n");
#endif
}

void tft_memoryDataAccessControl(madctl_t* madctl )
{
	st7789v_pdl_write_command(0x36);
	st7789v_pdl_write_data(*((uint8_t*)madctl));
}

void tft_turn_on_display()
{
	st7789v_pdl_write_command(0x29);
}
void tft_turn_off_display()
{
	st7789v_pdl_write_command(0x28);
}

void tft_turn_on_inversion()
{
	st7789v_pdl_write_command(0x21);
}
void tft_turn_off_inversion()
{
	st7789v_pdl_write_command(0x20);
}

void tft_turn_on_idle_mode()
{
	st7789v_pdl_write_command(0x39);
}
void tft_turn_off_idle_mode()
{
	st7789v_pdl_write_command(0x38);
}

void tft_interface_pixel_format(rgb_intr_format_e rgb_intr_format,bits_per_pixel_e bits_per_pixel)
{
	st7789v_pdl_write_command(0x3A);
	st7789v_pdl_write_data(rgb_intr_format << 4 | bits_per_pixel);
}

void tft_sw_reset()
{
	st7789v_pdl_write_command(0x01);
}

void tft_go_to_sleep()
{
	st7789v_pdl_write_command(0x10);
}

void tft_wake_up_from_sleep()
{
	st7789v_pdl_write_command(0x11);
}

//void tft_col_addr_set(uint8_t XS, uint8_t XE)
void tft_col_addr_set(uint16_t XS, uint16_t XE)
{
//	uint8_t datastream[] = {0x00, XS, 0x00, XE};
	uint8_t datastream[] = {
			((uint8_t*)&XS)[0],((uint8_t*)&XS)[1],
			((uint8_t*)&XE)[0],((uint8_t*)&XE)[1]
	};
	st7789v_pdl_write_command(0x2A);
	st7789v_pdl_write_data_stream(datastream,4);
}

void tft_row_addr_set(uint16_t YS, uint16_t YE)
{
	printf("YS0 : %X \t YS1 : %X \r\n",((uint8_t*)&YS)[0],((uint8_t*)&YS)[1]);
	printf("YE0 : %X \t YE1 : %X \r\n",((uint8_t*)&YE)[0],((uint8_t*)&YE)[1]);
	uint8_t datastream[] = {
			((uint8_t*)&YS)[0],((uint8_t*)&YS)[1],
			((uint8_t*)&YE)[0],((uint8_t*)&YE)[1]
	};
	st7789v_pdl_write_command(0x2B);
	st7789v_pdl_write_data_stream(datastream,4);
}

void tft_ram_write(uint8_t* data,uint32_t size)
{
	st7789v_pdl_write_data_stream(data,size);
}
void tft_selfTest()
{
#define SCREEN_MAX_SIZE	0x12C00			//Screen MAX = 240 *320 = 0x12C00
	st7789v_pdl_write_command(0x2C);
	for(uint32_t i =0;i<SCREEN_MAX_SIZE/2;i++){
	st7789v_pdl_write_data(0xFF);
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);}
	for(uint32_t i =0;i<SCREEN_MAX_SIZE/2;i++){
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0xFF);}
	for(uint32_t i =0;i<SCREEN_MAX_SIZE/2;i++){
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0xFF);
	st7789v_pdl_write_data(0x00);}
}

void tft_brightness(uint8_t displayBrightness)
{
	st7789v_pdl_write_command(0x51);
	st7789v_pdl_write_data(displayBrightness);
}

void tft_brightness_read()
{
	st7789v_pdl_write_command(0x52);
	printf("Brightness : %X\r\n",st7789v_pdl_read_data());
}


uint8_t tft_read_display_ctrl()
{
	st7789v_pdl_write_command(0x54);
	return st7789v_pdl_read_data();
}

typedef struct{
	uint8_t res3:2;
	uint8_t BL: 1;
	uint8_t DD: 1;
	uint8_t res2:1;
	uint8_t BCTRL:1;
	uint8_t res1:2;
}dctrl_t;

void tft_write_display_ctrl(dctrl_t dctrl)
{
	st7789v_pdl_write_command(0x53);
//	st7789v_pdl_write_data(*((uint8_t*)&dctrl));
	st7789v_pdl_write_data(0xFF);
}
void tft_print_display_ctrl()
{
	dctrl_t res_dctrl;
	*((uint8_t*)&res_dctrl) = tft_read_display_ctrl();
	printf("[ TFT ] : Brightness : %X \r\n",*((uint8_t*)&res_dctrl));
#if !VERBOSE_REG_DEBUG
	if(res_dctrl.BCTRL)
		printf("Brightness Control Block ON\r\n");
	else
		printf("Brightness Control Block OFF\r\n");

	if(res_dctrl.DD)
		printf("Display Dimming ON\r\n");
	else
		printf("Brightness Control Block OFF\r\n");

	if(res_dctrl.BL)
		printf("Backlight Control ON\r\n");
	else
		printf("Backlight Control OFF\r\n");

#else
	printf(" BCTRL : %X\t DD : %X\t BL : %X\r\n",res_dctrl.BCTRL,res_dctrl.DD,res_dctrl.BL);
#endif
}
void tft_init_sequence()
{
    madctl_t mad ={
    		.MX = 0,
			.MY = 1,
			.MH = 0,
			.RGB = 0,
			.MV = 1,
			.ML = 0
    };

    dctrl_t dctrl={
    		.BCTRL = 1,
			.BL = 1,
			.DD = 1,
    };
    tft_memoryDataAccessControl(&mad);
    tft_turn_off_inversion();
    tft_wake_up_from_sleep();
    tft_interface_pixel_format(TFT_RGB_262K,TFT_BIT_PER_PIXEL_18);
    tft_turn_on_display();
    tft_turn_off_idle_mode();

    tft_col_addr_set(0,0xFFFF);
    tft_row_addr_set(0,0xFFFF);


    tft_print_display_ctrl();
    tft_write_display_ctrl(dctrl);
    tft_brightness(0x00);
    tft_brightness_read();
    tft_print_display_ctrl();

    tft_selfTest();
}


