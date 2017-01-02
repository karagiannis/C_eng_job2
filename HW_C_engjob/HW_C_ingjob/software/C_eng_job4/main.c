/*
 * main.c
 *
 *  Created on: 31 dec 2016
 *      Author: root
 */

/*
 * TEIS
 * C-eng job
 * Run this code on a BeMicro10 card
 *
 * Use a VGA-screen 640 x 480. You need a connector
 * with three 75 Ohm resistors connected. See report
 * Lasse_Karagiannis_C_ingenjors_rapport_5.doc page 18
 *
 * Used sensors are the accelerometer, thermistor and photosensor
 *
 * Code is built on Eclipe Nios II Software Built Tools 16.0 from Altera
 *For building from scratch. Use unpack HW_C_eng_job.zip, for .sopcinfo-file
 *and for .sof-file. Copy the Timer-folder from this build  into IP-folder
 *and add files main.c, sensor.c, vga_utils.c, sensor.h, vga_utils.h
 *
 * Programmerare: Lasse Karagiannis
 * lasse.l.karagiannis@gmail.com
 * 2016-12-29
 *
 */


#include <stdio.h>
#include <system.h>
#include <io.h>
#include <alt_types.h>
//#include <BeMicro_VGA_IP_Driver.h>
#include "vga_util.h"
#include "sensor.h"

//#define DEBUG

#ifdef DEBUG
#include <string.h>
#endif


enum sampling_state  {five_Hz_sampling, one_Hz_sampling,dot2_Hz_sampling};


int main(void)
{
	print_welcome_screen(); 	//"Splashscreen"
	enum sampling_state state = one_Hz_sampling;
	static QUEUE q1; //Before I had static the program could suddenly restart, after it had
	//QUEUE q2;      // drawn
	//QUEUE q3;
	static QUEUE q4;//Before I had static the program could suddenly restart
	static QUEUE q5;
	 QUEUE* q11 = &q1;
	//QUEUE* q22 = &q2;
	//QUEUE* q33 = &q3;
	 QUEUE* q44 = &q4;
	 QUEUE* q55 = &q5;



	 SENSOR_OBJECT accelorometerX = //Accelerometer X
		{	"Accelerom. x",
			30, 50,1,0.1,30,4, q1,q11,
			config_time_base,
			init_measurement,
			read_accelerometerX,
			update_graph,
			//&accelorometerX,
			draw_graph
		};

	/*SENSOR_OBJECT accelorometerY =   //Accelerometer Y
					{	"Accelerom. y",
						320/3 +30, 50,1,1,20,4, q2,q22,
						config_time_base,
						init_measurement,
						read_accelerometerY,
						update_graph,
						&accelorometerY,
						draw_graph
					};

	SENSOR_OBJECT accelorometerZ =		//Accelerometer Z
				{	"Accelerom. z",
					2*320/3 +30, 50,1,1,60,4, q3,q33,
					config_time_base,
					init_measurement,
					read_accelerometerZ,
					update_graph,
					&accelorometerZ,
					draw_graph
				};
*/
	 SENSOR_OBJECT temp_sensor =				//Temperatur sensorn
					{	"Temperature",
						30, 180,1,100.0,10,4, q4,q44,
						config_time_base,
						init_measurement,
						read_temp,
						update_graph,
						//&temp_sensor,
						draw_graph
					};

	 SENSOR_OBJECT light_sensor =			//Ljussensorn
						{	"Light",
							320/3 +30, 180,1,100.0,10,4, q5,q55,
							config_time_base,
							init_measurement,
							read_light,
							update_graph,
							//&light_sensor,
							draw_graph
						};

/*SENSOR_OBJECT sensors[5]={accelorometerX,
							accelorometerY,
							accelorometerZ,
							temp_sensor,
							light_sensor};*/

	 SENSOR_OBJECT sensors[3]={accelorometerX,		//Array med sensor-objekt
								temp_sensor,
								light_sensor
							  };


//Routin which clear screen and writes possibel sampling frquency at low-right
//of the screen
	 draw_sampling_frequecy_sub_screen();


//Initiering av sensorerna
		for(int i = 0; i<3; i++)
			sensors[i].init_measurement(&sensors[i]);

	while(1)
	{

//Check which sampling frequency the user wants
		if(IORD_32DIRECT(KEY_INPUT_BASE,0) == 6)//first button
			state = five_Hz_sampling;
		if(IORD_32DIRECT(KEY_INPUT_BASE,0) == 5)//snd button
			state = one_Hz_sampling;
		if(IORD_32DIRECT(KEY_INPUT_BASE,0) == 3)//third button
					state = dot2_Hz_sampling;

		switch(state)
		{
			case five_Hz_sampling:
#ifdef DEBUG
			alt_printf("5Hz sampling\n");

#endif
//Highlite the text for the samplingfrequency 5Hz
			high_lite_5Hz_sampling_on_screen();

			for(int i = 0; i<3; i++)
			{
				sensors[i].configure_time_base(1,&sensors[i]);//För att illustrera koncept.Onödig.
				sensors[i].read_sensor(sensors[i].q);   	  //Läs sensor
				sensors[i].update_graph(&sensors[i]);		  //Updatera korresponderande graf
			}
			break;

		case one_Hz_sampling:
#ifdef DEBUG
			alt_printf("slow\n");

#endif
			high_lite_1Hz_sampling_on_screen();

			for(int i = 0; i<3; i++)
			{
				sensors[i].configure_time_base(10,&sensors[i]);//För att illustrera koncept.Onödig.
				sensors[i].read_sensor(sensors[i].q);			//Läs sensor
				sensors[i].update_graph(&sensors[i]);			//Updatera korresponderande graf
			}
           break;


		case dot2_Hz_sampling:
		#ifdef DEBUG
					alt_printf("0.5 sampling rate\n");

		#endif
					high_lite_dot2Hz_sampling_on_screen();

					for(int i = 0; i<3; i++)
					{
						sensors[i].configure_time_base(10,&sensors[i]);//För att illustrera koncept.Onödig.
						sensors[i].read_sensor(sensors[i].q);			//Läs sensor
						sensors[i].update_graph(&sensors[i]);			//Updatera korresponderande graf
					}
		           break;
		}//switch
	}//while

	return 0;
}//main

