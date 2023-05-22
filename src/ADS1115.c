/*
 * ADS1115.c
 *
 *  Created on: 1 oct. 2022
 *      Author: MATIAS
 */

#include "ADS1115.h"
#include "ADS1115_port.h"
#include <stdint.h>

/*--------------------------------------------------------*/
/*--------------------INIT FUNCTIONS----------------------*/

/*ADS1115_channelInit:
 *Inicializa un canal de conversión, el cual funciona sin el puerto READY del sensor.
 *Channel:SINGLE_MODE_AO,SINGLE_MODE_A1,SINGLE_MODE_A2, SINGLE_MODE_A3
 *        DIFERENTIAL_MODE_A0_A1,DIFERENTIAL_MODE_A0_A3,DIFERENTIAL_MODE_A1_A3,DIFERENTIAL_MODE_A2_A3*/
void ADS1115_channelInit(signalADS1115 * signalADS1115_port,uint8_t channel){
	signalADS1115_port->channel=channel;
	signalADS1115_port->pga=PGA_0;
	signalADS1115_port->operationMode=MODE_SINGLE_SHOT;
	signalADS1115_port->dataRate=DATA_RATE_128_SPS;
	signalADS1115_port->comparadorMode=COMPARATOR_WITH_HYSTEREIS;
	signalADS1115_port->comparadorPolarity=ALERT_ACTIVE_HIGH;
	signalADS1115_port->latchingComparador=NON_LATCHING_COMP;
	signalADS1115_port->stateComparator=DISABLE_COMPARATOR;
	signalADS1115_port->countConversion=0;
}

/*ADS1115_channelInitPolled:
 *Inicializa un canal de conversión, el cual funciona con el puerto READY del sensor.
 *Se carga los valores de umbrales adecuados para activar el funcionamiento del pin READY.
 *Channel:SINGLE_MODE_AO,SINGLE_MODE_A1,SINGLE_MODE_A2, SINGLE_MODE_A3
 *        DIFERENTIAL_MODE_A0_A1,DIFERENTIAL_MODE_A0_A3,DIFERENTIAL_MODE_A1_A3,DIFERENTIAL_MODE_A2_A3*/
void ADS1115_channelInitPolled(signalADS1115 * signalADS1115_port,uint8_t channel,uint8_t slaveAddres){
	signalADS1115_port->channel=channel;
	signalADS1115_port->pga=PGA_0;
	signalADS1115_port->operationMode=MODE_SINGLE_SHOT;
	signalADS1115_port->dataRate=DATA_RATE_128_SPS;
	signalADS1115_port->comparadorMode=COMPARATOR_WITH_HYSTEREIS;
	signalADS1115_port->comparadorPolarity=ALERT_ACTIVE_HIGH;
	signalADS1115_port->latchingComparador=NON_LATCHING_COMP;
	signalADS1115_port->stateComparator=AFTER_ONE_CONVERSION;
	signalADS1115_port->countConversion=0;

	ADS1115_updateThreshold(slaveAddres,READY_UMBRAL_LOW,READY_UMBRAL_HIGH);
}

/*--------------------------------------------------------*/
/*-----------------CONVERSION FUNCTIONS-------------------*/

/*ADS1115_signalConversion:
 *Función que inicia la conversion y devuelve el valor de la misma.
 *Es utilizada cuando se usa el sensor sin la intervencion del pin READY.*/
uint16_t ADS1115_signalConversion(signalADS1115 * signalADS1115_port,uint8_t slaveAddres){
	uint8_t wordWrite [3]={0};
	uint8_t wordReading[2];
	uint16_t wordRead;

	wordWrite[0]=CONFIG_REG;
	wordWrite[1]=((OS_BIT_CONVERSION<<7)|signalADS1115_port->channel<<4|signalADS1115_port->pga <<1|signalADS1115_port->operationMode);
	wordWrite[2]=((signalADS1115_port->dataRate<<5)|(signalADS1115_port->comparadorMode<<4)|(signalADS1115_port->comparadorPolarity<<3)|(signalADS1115_port->latchingComparador<<2)|signalADS1115_port->stateComparator);
	ADS1115_Transmit(slaveAddres, wordWrite,3);

	ADS1115_delay(20);
	wordWrite[0] =CONVERSION_REG;
	ADS1115_Transmit(slaveAddres, wordWrite,1);

	ADS1115_Receive(slaveAddres,wordReading,2);
	wordRead=((wordReading[0]<<8)|wordReading[1]);

	  if (wordRead<0){
		  wordRead=0;
	 	  }

	signalADS1115_port->countConversion=wordRead;
	return wordRead;
}

/*ADS1115_startConversionPolled:
 *Funcion que inicia la conversion.
 *Es utilizada cuando se usa el sensor con la intervencion del pin READY.*/
void ADS1115_startConversionPolled(signalADS1115 * signalADS1115_port,uint8_t slaveAddres){
	uint8_t wordWrite [3]={0};

	wordWrite[0]=CONFIG_REG;
	wordWrite[1]=((OS_BIT_CONVERSION<<7)|signalADS1115_port->channel<<4|signalADS1115_port->pga <<1|signalADS1115_port->operationMode);
	wordWrite[2]=((signalADS1115_port->dataRate<<5)|(signalADS1115_port->comparadorMode<<4)|(signalADS1115_port->comparadorPolarity<<3)|(signalADS1115_port->latchingComparador<<2)|signalADS1115_port->stateComparator);
	ADS1115_Transmit(slaveAddres, wordWrite,3);
}

/*ADS1115_getConversionPolled:
 *Función que devuelve el valor de la conversión.
 *Se utiliza cuando el sensor funciona con la intervención del pin READY.*/
uint16_t ADS1115_getConversionPolled(signalADS1115 * signalADS1115_port,uint8_t slaveAddres){
	uint8_t wordWrite [3]={0};
	uint16_t wordRead;
	uint8_t wordReading[2];
	bool conversionReady=false;

	  //-----------------------------------
	  //-------CONSULTANDO PIN READY-------

	  while(conversionReady==false){

		  if(ADS1115_gpioReadyRead()==true){
			  conversionReady=true;
		  }
	  }
	  //-----------------------------------

	wordWrite[0]=CONVERSION_REG;
	ADS1115_Transmit(slaveAddres, wordWrite,1);

	ADS1115_Receive(slaveAddres,wordReading,2);
	wordRead=((wordReading[0]<<8)|wordReading[1]);

	  if (wordRead<0){
		  wordRead=0;
	 	  }

	signalADS1115_port->countConversion=wordRead;
	return wordRead;
}

/*ADS1115_getValueVoltage:
 *Función devuelve el valor de conversion.expresado en VOLTS (Modulo).
 *El valor de cuenta(signo incluido) se almacena en la estructura del canal.*/
float ADS1115_getValueVoltage(signalADS1115 * signalADS1115_port){
	float maxVoltage=0.0;
	float voltage=0.0;
	uint16_t cuenta=0;

	switch(signalADS1115_port->pga){
	case PGA_0:
		maxVoltage=FULL_RANGE_PG0;
		break;
	case PGA_1:
		maxVoltage=FULL_RANGE_PG1;
		break;
	case PGA_2:
		maxVoltage=FULL_RANGE_PG2;
		break;
	case PGA_3:
		maxVoltage=FULL_RANGE_PG3;
		break;
	case PGA_4:
		maxVoltage=FULL_RANGE_PG4;
		break;
	case PGA_5:
		maxVoltage=FULL_RANGE_PG5;
		break;
	case PGA_6:
		maxVoltage=FULL_RANGE_PG6;
		break;
	case PGA_7:
		maxVoltage=FULL_RANGE_PG7;
		break;
	default:
		maxVoltage=0.0;
		break;
	}

	cuenta=signalADS1115_port->countConversion;

	if(signalADS1115_port->countConversion>>15){
		cuenta=~cuenta;
	}

	voltage=(maxVoltage/CUENTA_MAXIMA_ADC)*cuenta;
	return voltage;
}

/*--------------------------------------------------------*/
/*------------------UPDATE FUNCTIONS----------------------*/

/*ADS1115_updatePGA:
 *Función que actualiza la ganancia del canal(cambiando el FULL SCALE).
 *Opciones: PGA_0, PGA_1, PGA_2, PGA_3, PGA_4, PGA_5, PGA_6, PGA_7*/
void ADS1115_updatePGA(signalADS1115 * signalADS1115_port,uint8_t state){
	signalADS1115_port->pga=state;
}

/*ADS1115_updateOperationMode:
 *Función que actualiza el modo de operacion.
 *Opciones: MODE_CONTINUOUS_CONVERSION,MODE_SINGLE_SHOT*/
void ADS1115_updateOperationMode(signalADS1115 * signalADS1115_port,uint8_t operationMode){
	signalADS1115_port->operationMode=operationMode;
}

/*ADS1115_updateDataRate:
 *Función que actualiza la ganancia del canal(cambiando el FULL SCALE).
 *Opciones: DATA_RATE_8_SPS, DATA_RATE_16_SPS, DATA_RATE_32_SPS, DATA_RATE_64_SPS, DATA_RATE_128_SPS, DATA_RATE_250_SPS, DATA_RATE_475_SPS, DATA_RATE_860_SPS*/
void ADS1115_updateDataRate(signalADS1115 * signalADS1115_port,uint8_t dataRate){
	signalADS1115_port->dataRate=dataRate;
}

/*ADS1115_updateComparatorMode:
 *Función que actualiza el valor de tipo de comparador a utilizar en el canal.
 *Opciones: COMPARATOR_WITH_HYSTEREIS, COMPARATOR_WINDOW*/
void ADS1115_updateComparatorMode(signalADS1115 * signalADS1115_port,uint8_t comparator){
	signalADS1115_port->comparadorMode=comparator;
}

/*ADS1115_updateComparatorPolarity:
 *Función que actualiza la polaridad del puerto READY/ALERT.
 *Opciones: ALERT_ACTIVE_LOW, ALERT_ACTIVE_HIGH*/
void ADS1115_updateComparatorPolarity(signalADS1115 * signalADS1115_port,uint8_t polarity){
	signalADS1115_port->comparadorPolarity=polarity;
}

/*ADS1115_updateLatchingComparador:
 *Función que habilita la funcion de latch.
 *Opciones: NON_LATCHING_COMP, LATCHING_COMP*/
void ADS1115_updateLatchingComparador(signalADS1115 * signalADS1115_port,uint8_t latchingComparador){
	signalADS1115_port->latchingComparador=latchingComparador;
}

/*ADS1115_updateStateComparator:
 *Función que habilita el comparador, y setea el instante en que se ejecuta el mismo.
 *Opciones: AFTER_ONE_CONVERSION, AFTER_TWO_CONVERSION, AFTER_FOUR_CONVERSION, DISABLE_COMPARATOR*/
void ADS1115_updateStateComparator(signalADS1115 * signalADS1115_port,uint8_t state){
	signalADS1115_port->stateComparator=state;
}

/*ADS1115_updateThreshold:
 *Función que actualiza los valores de umbrales para la comparacion.*/
void ADS1115_updateThreshold(uint8_t slaveAddres,uint16_t umbralLow, uint16_t	umbralHigh){
	  uint8_t wordWrite[3]={0};
	  wordWrite[0]=LO_THRESH_REG;
	  wordWrite[1]=(umbralLow>>8);
	  wordWrite[2]=umbralLow;
	  ADS1115_Transmit(slaveAddres, wordWrite,3);

	  wordWrite[0]=HI_THRESH_REG;
	  wordWrite[1]=(umbralHigh>>8);
	  wordWrite[2]=umbralHigh;
	  ADS1115_Transmit(slaveAddres, wordWrite,3);
}
