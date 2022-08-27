/*
 * tft_app.c
 *
 *  Created on: 27-Aug-2022
 *      Author: AustinA
 */

#include "tft_app.h"

#define SCREEN_MAX_SIZE	0x12C00			//Screen MAX = 240 *320 = 0x12C00


void tft_brightness_read()
{
	printf("Brightness : %X\r\n",tft_get_brightness());
}

void pixelSet(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint32_t size)
{
    tft_col_addr_set(0+x,size+x);
    tft_row_addr_set(0+y,size+y);
	st7789v_pdl_write_command(0x2C);
	for(uint32_t i =0;i<SCREEN_MAX_SIZE;i++){
	st7789v_pdl_write_data(r);
	st7789v_pdl_write_data(g);
	st7789v_pdl_write_data(b);}

}


void testPrint()
{

    tft_col_addr_set(0,0xFFFF);
    tft_row_addr_set(0,0xFFFF);

	st7789v_pdl_write_command(0x2C);
	for(uint32_t i =0;i<SCREEN_MAX_SIZE;i++){
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);
	}

	pixelSet(20,40,0,0xFF,0,40);
    tft_col_addr_set(120/2,(120/2)+10);
    tft_row_addr_set(240/2,(240/2)+10);
	st7789v_pdl_write_command(0x2C);
	for(uint32_t i =0;i<SCREEN_MAX_SIZE;i++){
		st7789v_pdl_write_data(0xFF);
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);
	}

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
    tft_print_disp_ID();
    tft_memoryDataAccessControl(&mad);
    tft_turn_off_inversion();
    tft_wake_up_from_sleep();
    tft_interface_pixel_format(TFT_RGB_262K,TFT_BIT_PER_PIXEL_18);
    tft_turn_on_display();
    tft_turn_off_idle_mode();

    tft_col_addr_set(0,0xFFFF);
    tft_row_addr_set(0,0xFFFF);


    tft_print_display_ctrl();
    tft_write_display_ctrl(&dctrl);
    tft_set_brightness(0x00);
    tft_brightness_read();
    tft_print_display_ctrl();

//    tft_selfTest();
    testPrint();
    tft_brightness_read();
}





void tft_print_display_ctrl()
{
	dctrl_t res_dctrl;
	*((uint8_t*)&res_dctrl) = tft_read_display_ctrl();
	printf("[ TFT ] : Display CTRL : %X \r\n",*((uint8_t*)&res_dctrl));
#if !VERBOSE_REG_DEBUG
	if(res_dctrl.BCTRL)
		printf("\tBrightness Control Block ON\r\n");
	else
		printf("\tBrightness Control Block OFF\r\n");

	if(res_dctrl.DD)
		printf("\tDisplay Dimming ON\r\n");
	else
		printf("\tBrightness Control Block OFF\r\n");

	if(res_dctrl.BL)
		printf("\tBacklight Control ON\r\n");
	else
		printf("\tBacklight Control OFF\r\n");

#else
	printf("BCTRL : %X\t DD : %X\t BL : %X\r\n",res_dctrl.BCTRL,res_dctrl.DD,res_dctrl.BL);
#endif
}


void tft_selfTest()
{
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

	for(uint32_t i =0;i<SCREEN_MAX_SIZE;i++){
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);
	st7789v_pdl_write_data(0x00);}
}


void tft_print_disp_ID()
{
	printf("Display ID : %X\r\n",tft_get_disp_ID());
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






