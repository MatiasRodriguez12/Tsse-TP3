/*
 * ADS1115.h
 *
 *  Created on: 1 oct. 2022
 *      Author: MATIAS
 */

#ifndef API_INC_ADS1115_H_
#define API_INC_ADS1115_H_
#include <stdint.h>
#include <stdbool.h>

/*----------------------------------*/
/*-----------SLAVE ADDRESS----------*/
#define SLAVE_ADRRES_GND	0x48
#define SLAVE_ADRRES_VDD	0x49
#define SLAVE_ADRRES_SDA	0x4A
#define SLAVE_ADRRES_SCL	0x4B
/*----------------------------------*/

/*----------------------------------*/
/*---------REGISTER ADDRESS---------*/
#define CONVERSION_REG		0x00
#define CONFIG_REG			0x01
#define LO_THRESH_REG		0x02
#define HI_THRESH_REG		0x03
/*----------------------------------*/


/*---------------------------------*/
/*---------CONFIG REGISTER---------*/
#define OS_BIT_UPLOAD			0
#define OS_BIT_CONVERSION		1
/*---------MUX---------*/
#define DIFERENTIAL_MODE_A0_A1	0
#define DIFERENTIAL_MODE_A0_A3	1
#define DIFERENTIAL_MODE_A1_A3	2
#define DIFERENTIAL_MODE_A2_A3	3
#define SINGLE_MODE_A0			4
#define SINGLE_MODE_A1			5
#define SINGLE_MODE_A2			6
#define SINGLE_MODE_A3			7
/*---PROGRAMMABLE GAIN AMPLIFIER---*/
#define PGA_0	0	//FULL SCALE: 6.144V
#define PGA_1	1 //FULL SCALE: 4.096V
#define PGA_2	2 //FULL SCALE: 2.048V
#define PGA_3	3 //FULL SCALE: 1.024V
#define PGA_4	4 //FULL SCALE: 0.512V
#define PGA_5	5 //FULL SCALE: 0.256V
#define PGA_6	6 //FULL SCALE: 0.256V
#define PGA_7	7 //FULL SCALE: 0.256V
/*---OPERATING MODE---*/
#define MODE_CONTINUOUS_CONVERSION	0
#define MODE_SINGLE_SHOT			1
/*---DATA RATE---*/
#define DATA_RATE_8_SPS			0
#define DATA_RATE_16_SPS		1
#define DATA_RATE_32_SPS		2
#define DATA_RATE_64_SPS		3
#define DATA_RATE_128_SPS		4
#define DATA_RATE_250_SPS		5
#define DATA_RATE_475_SPS		6
#define DATA_RATE_860_SPS		7
/*---COMPARATOR MODE---*/
#define COMPARATOR_WITH_HYSTEREIS	0
#define COMPARATOR_WINDOW			1
/*---COMPARATOR POLARITY---*/
#define ALERT_ACTIVE_LOW			0
#define ALERT_ACTIVE_HIGH			1
/*---LATCHING COMPARATOR---*/
#define NON_LATCHING_COMP		0
#define LATCHING_COMP			1
/*---COMPARATOR QUEUE OR DISSABLE COMP---*/
#define AFTER_ONE_CONVERSION	0
#define AFTER_TWO_CONVERSION	1
#define AFTER_FOUR_CONVERSION	2
#define DISABLE_COMPARATOR		3
/*---------------------------------*/

/*-----------------------------------*/
/*--UMBRALES PARA ACTIVAR PIN READY--*/
#define READY_UMBRAL_LOW		0
#define READY_UMBRAL_HIGH 		32768
/*-----------------------------------*/

/*----------------------------------*/
/*------ESCALAS DE CONVERSION-------*/
#define FULL_RANGE_PG0		6.144
#define FULL_RANGE_PG1		4.096
#define FULL_RANGE_PG2		2.048
#define FULL_RANGE_PG3		1.024
#define FULL_RANGE_PG4		0.512
#define FULL_RANGE_PG5		0.256
#define FULL_RANGE_PG6		0.256
#define FULL_RANGE_PG7		0.256
#define	CUENTA_MAXIMA_ADC	32768
/*----------------------------------*/

/*----------------------------------*/
/*--------------OTROS---------------*/
#define FUNCTION_OK     true
#define FUNCTION_FALLED false
#define GPIOF           6
#define GPIOB           2
#define GPIO_PIN_8      8
#define GPIO_PIN_9      9
#define GPIO_PIN_13     13
/*Puertos SDA y SCL a utilizar*/
#define D14_port 	      GPIOB
#define D14_pin  	      GPIO_PIN_9
#define D15_port     	GPIOB
#define D15_pin  	      GPIO_PIN_8
#define i2c_SDA_port    D14_port
#define i2c_SDA_pin     D14_pin
#define i2c_SCL_port    D15_port
#define i2c_SCL_pin     D15_pin
/*Puerto de entrada por donde se leerá la señal READY*/
#define READY_port	   D7_port
#define READY_pin	      D7_pin
#define D7_port 	      GPIOF
#define D7_pin  	      GPIO_PIN_13
/*----------------------------------*/

/*--------------------------------------------------------------------------*/
/*----Estructura que almacenará los parametros de un canal a utilizar-------*/
typedef struct{
   uint8_t channel;
   uint8_t pga;
   uint8_t operationMode;
   uint8_t dataRate;
   uint8_t comparadorMode;
   uint8_t comparadorPolarity;
   uint8_t latchingComparador;
   uint8_t stateComparator;
   uint16_t countConversion;

} signalADS1115;
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------*/
/*--------------------INIT FUNCTIONS----------------------*/

/*ADS1115_channelInit:
 *Inicializa un canal de conversion, el cual funciona sin el puerto READY del sensor.
 *Channel:SINGLE_MODE_AO,SINGLE_MODE_A1,SINGLE_MODE_A2, SINGLE_MODE_A3
 *        DIFERENTIAL_MODE_A0_A1,DIFERENTIAL_MODE_A0_A3,DIFERENTIAL_MODE_A1_A3,DIFERENTIAL_MODE_A2_A3*/
void ADS1115_channelInit(signalADS1115 * signalADS1115_port,uint8_t channel);

/*ADS1115_channelInitPolled:
 *Inicializa un canal de conversion, el cual funciona con el puerto READY del sensor.
 *Se carga los valores de umbrales adecuados para activar el funcionamiento del pin READY.
 *Channel:SINGLE_MODE_AO,SINGLE_MODE_A1,SINGLE_MODE_A2, SINGLE_MODE_A3
 *        DIFERENTIAL_MODE_A0_A1,DIFERENTIAL_MODE_A0_A3,DIFERENTIAL_MODE_A1_A3,DIFERENTIAL_MODE_A2_A3*/
bool ADS1115_channelInitPolled(signalADS1115 * signalADS1115_port,uint8_t channel,uint8_t slaveAddres);

/*--------------------------------------------------------*/
/*-----------------CONVERSION FUNCTIONS-------------------*/

/*ADS1115_signalConversion:
 *Función que inicia la conversión y devuelve el valor de la misma.
 *Es utilizada cuando se usa el sensor sin la intervención del pin READY.*/
uint16_t ADS1115_signalConversion(signalADS1115 * signalADS1115_port,uint8_t slaveAddres);

/*ADS1115_startConversionPolled:
 *Función que inicia la conversión.
 *Es utilizada cuando se usa el sensor con la intervención del pin READY.*/
void ADS1115_startConversionPolled(signalADS1115 * signalADS1115_port,uint8_t slaveAddres);

/*ADS1115_getConversionPolled:
 *Función que devuelve el valor de la conversión.
 *Es utilizada cuando se usa el sensor con la intervención del pin READY.*/
uint16_t ADS1115_getConversionPolled(signalADS1115 * signalADS1115_port,uint8_t slaveAddres);

/*ADS1115_getValueVoltage:
 *Función devuelve el valor de conversion expresado en VOLTS (Modulo).
 *El valor de cuenta(signo incluido) se almacena en la estructura del canal.*/
float ADS1115_getValueVoltage(signalADS1115 * signalADS1115_port);

/*--------------------------------------------------------*/
/*------------------UPDATE FUNCTIONS----------------------*/

/*ADS1115_updatePGA:
 *Función que actualiza la ganancia del canal(cambiando el FULL SCALE).
 *PGA_0, PGA_1, PGA_2, PGA_3, PGA_4, PGA_5, PGA_6, PGA_7*/
void ADS1115_updatePGA(signalADS1115 * signalADS1115_port,uint8_t state);

/*ADS1115_updateOperationMode:
 *Función que actualiza el modo de operacion.
 *MODE_CONTINUOUS_CONVERSION,MODE_SINGLE_SHOT*/
void ADS1115_updateOperationMode(signalADS1115 * signalADS1115_port,uint8_t operationMode);

/*ADS1115_updateDataRate:
 *Función que actualiza la ganancia del canal(cambiando el FULL SCALE).
 *Opciones: DATA_RATE_8_SPS, DATA_RATE_16_SPS, DATA_RATE_32_SPS, DATA_RATE_64_SPS, DATA_RATE_128_SPS, DATA_RATE_250_SPS, DATA_RATE_475_SPS, DATA_RATE_860_SPS*/
void ADS1115_updateDataRate(signalADS1115 * signalADS1115_port,uint8_t dataRate);

/*ADS1115_updateComparatorMode:
 *Función que actualiza el valor de tipo de comparador a utilizar en el canal.
 *Opciones: COMPARATOR_WITH_HYSTEREIS, COMPARATOR_WINDOW*/
void ADS1115_updateComparatorMode(signalADS1115 * signalADS1115_port,uint8_t comparator);

/*ADS1115_updateComparatorPolarity:
 *Función que actualiza la polaridad del puerto READY/ALERT.
 *Opciones: ALERT_ACTIVE_LOW, ALERT_ACTIVE_HIGH*/
void ADS1115_updateComparatorPolarity(signalADS1115 * signalADS1115_port,uint8_t polarity);

/*ADS1115_updateLatchingComparador:
 *Función que habilita la funcion de latch.
 *Opciones: NON_LATCHING_COMP, LATCHING_COMP*/
void ADS1115_updateLatchingComparador(signalADS1115 * signalADS1115_port,uint8_t latchingComparador);

/*ADS1115_updateStateComparator:
 *Función que habilita el comparador, y setea el instante en que se ejecuta el mismo.
 *Opciones: AFTER_ONE_CONVERSION, AFTER_TWO_CONVERSION, AFTER_FOUR_CONVERSION, DISABLE_COMPARATOR*/
void ADS1115_updateStateComparator(signalADS1115 * signalADS1115_port,uint8_t state);

/*ADS1115_updateThreshold:
 *Función que actualiza los valores de umbrales para la comparacion.*/
bool ADS1115_updateThreshold(uint8_t slaveAddres,uint16_t umbralLow, uint16_t	umbralHigh);

#endif /* API_INC_ADS1115_H_ */
