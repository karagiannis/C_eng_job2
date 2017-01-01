/*
 * sensor.h

 *Header-file for SENSOR_OBJECT and its methods,
 *the queue and its utility-functions
 *  Created on: 28 dec 2016
 *      Author: Lasse Karagiannis
 *      Company: TEIS AB
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define QUEUESIZE 10  // Ville ha 50 m�tv�rden, men funkar bara att k�ra en graf d�.
#include <system.h>
#include <stddef.h>
#include <alt_types.h>
#include <io.h>
#include <altera_avalon_spi.h>
#include <altera_avalon_spi_regs.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "vga_util.h"

#define WRITE_COMMAND 0x0A 	// Used for acc SPI
#define READ_COMMAND 0x0B	// Used for acc SPI

#define ADC_INIT IOWR_32DIRECT(MODULAR_ADC_0_SEQUENCER_CSR_BASE,0,0x1) // Start continuous sampling
#define ADC_READ_PHOTO IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 0)  // Read ADC value
#define ADC_READ_TEMP IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 4)  // Read ADC value






typedef struct mystruct {
		alt_8 x;
		alt_8 y;
		alt_8 z;
} ACCELEROMETER;

//ACCELEROMETER accel_data;



typedef struct myQUEUE
{
    alt_32 items[QUEUESIZE];  //m�tv�rdesvektor
    int rindex;   //Anv�nds ej
    int windex;		//Anv�nds ej
    alt_u32 numitems;
}QUEUE;

extern void queue_init(QUEUE *q);
extern alt_32 queue_enqueue(QUEUE *q,alt_32 item);
extern alt_32 queue_dequeue(QUEUE *q);
extern void queue_print(QUEUE *q);



typedef struct SENSOR_CLASS SENSOR_OBJECT;


struct SENSOR_CLASS {
    char description[80];			//Description of sensor preseneted ON VGA-display
    alt_u32 x_origo;				//x-origo of sensors coordinate axes on VGA-screen
    alt_u32 y_origo;				//y-origo of sensors coordinate axes on VGA-screen
    alt_u32 time_base;				//Which samplingfreq currently used. Un necessary. But done anyway.
    alt_u32 normalization_factor; 	//Scaling measurement data, so it fits into display
    alt_u32 offset;					//Offset to present graph around the middle of y-axis
    alt_u32 rgb;					//Graph-color
    QUEUE queue;					//Sensor data array
    QUEUE *q;						//not necessary, but used anyway
    //void *this;						//pointer to itself. Used only sometimes for estetic reasons
    void (*configure_time_base)(alt_u32, SENSOR_OBJECT*); // Pointer to function which  records sampling freq. Unnecessary but used.
    void (*init_measurement)(SENSOR_OBJECT*);  //Pointer to function which initializes queue to zero, and draws graph
    void (*read_sensor)(QUEUE*);  	//Pointer to function which read the sensor in particular
    void (*update_graph)(SENSOR_OBJECT*);  	//Pointer to function which draws the graph
    void (*draw_graph)(SENSOR_OBJECT*);		//Pointer to function which draws coordinate axes
};

extern void config_time_base(alt_u32, SENSOR_OBJECT*);// Records the timebase
extern void init_measurement(SENSOR_OBJECT*);			//Initializes the queue
extern void read_accelerometerX(QUEUE *);
//void read_accelerometerY(QUEUE *q);
//void read_accelerometerZ(QUEUE *q);
extern void update_graph(SENSOR_OBJECT*);
extern void draw_graph(SENSOR_OBJECT*);				//Draws the graph axes
extern void read_temp(QUEUE *q);
extern void read_light(QUEUE *q);
extern void queue_print_screen(QUEUE *q, alt_u32 x_origo, alt_u32 y_origo, alt_32 normalization, alt_32 offset, alt_u32 rgb, SENSOR_OBJECT *sensor_obj);

extern unsigned int i2bcd(unsigned int i);//utility function for presenting time integer to bcd
extern void update_time(unsigned int i);  //Prints the time

#endif /* SENSOR_H_ */
