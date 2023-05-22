/*
 * ADS1115_port.h
 *
 *  Created on: 3 oct. 2022
 *      Author: MATIAS
 */

#ifndef API_INC_ADS1115_PORT_H_
#define API_INC_ADS1115_PORT_H_

#include <stdint.h>
#include <stdbool.h>

/*I2CGpio_init:
 *Función que inicializa la configuración de los puertos SDA y SCL.*/
void I2CGpio_init(void);

/*gpioADS1115Ready_init:
 *Función que inicializa la configuración del puerto de entrada por donde se leerá la señal READY.*/
bool ADS1115_gpioReadyInit(uint8_t gpio_port, uint8_t gpio_pin);

/*gpioADS1115Ready_init:
 *Función lee el estado del puerto de entrada.*/
bool ADS1115_gpioReadyRead(uint8_t gpio_port, uint8_t gpio_pin);

/*ADS1115_Transmit:
 *Función que envia datos desde el Master al Esclavo.
 *Parametros: slaveAddres - Dirección del esclavo.
 *			  word - datos a enviar.
 *			  nroBytes - Cantidad de bytes a enviar*/
bool ADS1115_Transmit(uint8_t slaveAddres, uint8_t *word,uint16_t nroBytes);

/*ADS1115_Receive:
 *Función que lee los datos del esclavo.
 *Parametros: slaveAddres - Dirección del esclavo.
 *			  word - Lugar donde se va a almacenar la información.
 *			  nroBytes - Cantidad de bytes a recibir*/
void ADS1115_Receive(uint8_t slaveAddres,uint8_t *word,uint16_t nroBytes);

/*ADS1115_delay:
 *Función que realiza un delay bloqueante.
 *Parametro: delay - Tiempo expresado en ms.*/
void ADS1115_delay(uint8_t delay);

#endif /* API_INC_ADS1115_PORT_H_ */
