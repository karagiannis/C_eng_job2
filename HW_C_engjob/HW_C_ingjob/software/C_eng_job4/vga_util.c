///*
// /*
// * vga_util.c
// *
// *  Created on: 28 dec 2016
// *      Author: root
// */
#include <alt_types.h>
#include "vga_util.h"

unsigned int i2bcd(unsigned int i) {
	//Integer to bcd
	//Found algoritm online on discussioforum. Didn't work, had to rework it with some trials.
    unsigned int binaryShift = 1;
    unsigned int digit;
    unsigned int bcd = 0;
    //alt_printf("%d\n",i);
    while (i > 0) {
        digit = i % 10;
        bcd += (digit << binaryShift);
        binaryShift += 4;
        i /= 10;
    }
    bcd = bcd >> 1; //Added this
#ifdef DEBUG
    unsigned int digit,n,mask;
    for (n = 0,mask = 0x80000000;mask != 0;mask>>= 1){
            if((n==4) || (n==8)|| (n == 12)|| n== 16 ||n==20|| n==24 ||n==28)
                putchar(' ');
            putchar((bcd & mask)? '1':'0');
            n++;
        }
    putchar('\n');
#endif

    return bcd;
}


void update_time(unsigned int i){
	//Used function argument earlier. Not doing it know

	static int hours = 0;
	static int minutes = 0;
	static int seconds = 0;
	//static int sub_seconds = 0;
	unsigned int bcd = 0;
	//unsigned int time[3]={hours, minutes,seconds};


		seconds++;


	if(seconds == 60)
	{
		seconds = 0;
		minutes++;
		if (minutes == 60)
		{
			minutes = 0;
			hours++;
		}
	}

	char c;
	short s;
	bcd = i2bcd(hours);   							//Hours
	c = (char)((bcd & 0x0F0) >>4);
	s = (short) c +48;
	print_char(2*320/3-50 +30 +46, 130,4,0,(char)s);

	c = (char)(bcd & 0x0F);
	s = (short) c +48;
	print_char(2*320/3 -50+30 +54,130,4,0,(char)s);

	print_char(2*320/3-50 +30 +54+8,130,4,0,':'); 	// :

	bcd = i2bcd(minutes);							//Minutes
	c = (char)((bcd & 0x0F0) >>4);
	s = (short) c +48;
	print_char(2*320/3-50 +30 +54+8+8, 130,4,0,(char)s);

	c = (char)(bcd & 0x0F);
	s = (short) c +48;
	print_char(2*320/3-50 +30 +54+8+8+8,130,4,0,(char)s);

	print_char(2*320/3-50 +30 +54+8+8+8+8,130,4,0,':');// :


	bcd = i2bcd(seconds);								//seconds
		c = (char)((bcd & 0x0F0) >>4);
		s = (short) c +48;
		print_char(2*320/3-50 +30 +54+40, 130,4,0,(char)s);

		c = (char)(bcd & 0x0F);
		s = (short) c +48;
		print_char(2*320/3-50 +30 +54+48,130,4,0,(char)s);
}
extern void high_lite_dot2Hz_sampling_on_screen(void)
{
	//F�rga texten f�r den samplingsfrekvens som valts
						print_str(2*320/3 +5,210-60,5,"1st btn 5Hz");
						print_str(2*320/3 +5,210-30,5,"2nd btn 1Hz");
						print_str(2*320/3 +5,210,2,"3d btn .2Hz");
						for(int i = 0; i < 5; i++)
						{
							TIMER_RESET;
							TIMER_START;

							while(TIMER_READ < 50000000);
							update_time(1);
						}

}

extern void high_lite_1Hz_sampling_on_screen(void)
{
	//F�rga texten f�r den samplingsfrekvens som valts
				print_str(2*320/3 +5,210-60,5,"1st btn 5Hz");
				print_str(2*320/3 +5,210-30,2,"2nd btn 1Hz");
				print_str(2*320/3 +5,210,5,"3d btn .2Hz");
				TIMER_RESET;
				TIMER_START;
					while(TIMER_READ < 50000000);
					update_time(1);
					TIMER_RESET;
					TIMER_START;
}

void high_lite_5Hz_sampling_on_screen(void)
{
			int sub_seconds = 0;
	//F�rga texten f�r den samplingsfrekvens som valts
			print_str(2*320/3 +5,210-60,2,"1st btn 5Hz");
			print_str(2*320/3 +5,210-30,5,"2nd btn 1Hz");
			print_str(2*320/3 +5,210,5,"3d btn .2Hz");
			TIMER_RESET;
			TIMER_START;
			while(TIMER_READ < 100000);
			sub_seconds++;
			if(sub_seconds == 5)
			{
				update_time(1);
				sub_seconds = 0;
			}
}

extern void draw_sampling_frequecy_sub_screen(void)
{
	for(int i = 0; i < 78600; i++)
				set_address_pixel(i, 0);

			print_hline(0,120,320,6);
			print_vline(320/3,0,240,6);
			print_vline(2*320/3,0,240,6);

			print_str(2*320/3 +5,210-60,5,"1st btn 5Hz");
			print_str(2*320/3 +5,210-30,5,"2nd btn 1Hz");
			print_str(2*320/3 +5,210,5,"3d btn .2Hz");
			print_str(2*320/3 +5,230,5,"L.Karagiannis");

}


void print_pix(alt_u32 x,alt_u32 y,alt_u32 rgb)
{
	if(rgb <= 7 && rgb >= 0)
		set_pixel(x,y,rgb);
	else
		set_pixel(x,y,0);
}
void print_hline(alt_u32 x_start,alt_u32 y_start, alt_u32 len,alt_u32 RGB){



	if(x_start + len <= 320){
		for(int i = 0; i < len; i++){
			print_pix(x_start + i,y_start, RGB);
		}
	}
	else{

		for(int i = 0; i < (320-x_start); i++){
					print_pix(x_start + i,y_start, RGB);
		}
	}

}
void print_vline(alt_u32 x_start,alt_u32 y_start, alt_u32 len, alt_u32 RGB)
{
	if(y_start + len <= 240){
			for(int i = 0; i < len; i++){
				print_pix(x_start,y_start + i, RGB);
			}
		}
		else{

			for(int i = 0; i < (240-y_start); i++){
						print_pix(x_start,y_start +i , RGB);
			}
		}
}

void  print_char(alt_u32 x,alt_u32 y,alt_u32 rgb,alt_u32 BG_RGB,char Character)
{
	 alt_u8 font8x8_basic[128][8] = {
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
	    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
	    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
	    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
	    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
	    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
	    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
	    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
	    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
	    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
	    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
	    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
	    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
	    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
	    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
	    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
	    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
	    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
	    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
	    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
	    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
	    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
	    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
	    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
	    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
	    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
	    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
	    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
	    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
	    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
	    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
	    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
	    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
	    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
	    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
	    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
	    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
	    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
	    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
	    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
	    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
	    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
	    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
	    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
	    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
	    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
	    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
	    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
	    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
	    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
	    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
	    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
	    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
	    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
	    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
	    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
	    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
	    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
	    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
	    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
	    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
	    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
	    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
	    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
	    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
	    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
	    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
	    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
	    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
	    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
	    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
	    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
	    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
	    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
	    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
	    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
	    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
	    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
	    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
	    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
	    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
	    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
	    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
	    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
	    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
	    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
	    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
	    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
	    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
	    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
	    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
	    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
	};
	alt_u8 temp;
	int row;

	alt_u8  token = (alt_u8)Character;
	for (row = 0; row <8; row++)
	{
			temp =  (alt_u8)font8x8_basic[token][row];

			for (int pos = 0 ; pos != 8 ; pos++) {
			    if (temp & (1 << pos)) {
			        // Current bit is set to 1
			    	print_pix(x+pos,y+row,rgb);
			    } else {
			        // Current bit is set to 0
			    	print_pix(x+pos,y+row,BG_RGB);
			    }
			}
	}
}
void print_str(alt_u32 x_start, alt_u32 y_start,alt_u32 rgb, const char *str)
{
	alt_u16 len = (alt_u16) strlen(str);
	alt_u16 i;
	for(i=0; i< len; i++){
		print_char(x_start + i*8,y_start,rgb,0,str[i]);
	}
}

void print_symmetry_dots_circle(alt_u32 x, alt_u32 y, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb)
{
		print_pix(x+x_centrum,y + y_centrum,rgb);
		print_pix(x+x_centrum,-y + y_centrum,rgb);
		print_pix(-x+x_centrum,y + y_centrum,rgb);
		print_pix(-x+x_centrum,-y + y_centrum,rgb);

		print_pix(y + x_centrum,x+y_centrum,rgb);
		print_pix(-y + x_centrum,x+y_centrum,rgb);
		print_pix(y + x_centrum,-x+y_centrum,rgb);
		print_pix(-y + x_centrum,-x+y_centrum,rgb);

}
void print_empty_circle(alt_u32 radie, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb)
{
		//unsigned int value;
		//unsigned int radius = radie;
		int x = 0;
		int y = radie;

		print_symmetry_dots_circle(x,y,x_centrum,y_centrum,rgb);
		int h = 1- radie;
		while(y > x){
			if (h < 0){//east point

				print_symmetry_dots_circle(x+1,y,x_centrum,y_centrum,rgb);
				h = h +2*x +3;
				x= x+1;

			}
			else{
				print_symmetry_dots_circle(x+1,y-1,x_centrum,y_centrum,rgb);
				h= h+2*(x-y)+5;
				y=y-1;
				x=x+1;
			}

		}
}
void print_circle(alt_u32 radie, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb)
{
	unsigned int radius = radie;

	for(radius = radie; radius > 0; radius--)
		print_empty_circle(radius,x_centrum, y_centrum, rgb);
}
void clear_screen(alt_u32 rgb){
	for(int y = 0 ;y <240;y++)
		for(int x = 0; x < 320; x++)
			print_pix(x,y,rgb);
}

pixel_data read_pixel_ram_int(alt_u32 x_start, alt_u32 y_start)
{
	alt_u32 i = read_pixel(x_start, y_start);
	return (alt_u8) i;
}



void print_welcome_screen(){
	clear_screen(0);

	print_circle(10,  320/2, 240/2, 3);
	print_str(50,50,2,"Welcome to measurement station!");
	print_str(50,150,2,"Press any key to continue");
	alt_u32 key = 0;
	key = IORD_32DIRECT(KEY_INPUT_BASE,0);
	//alt_printf("key = %d", key);
	while(key == 7)
		key = IORD_32DIRECT(KEY_INPUT_BASE,0);


}
