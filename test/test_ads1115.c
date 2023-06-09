#include "unity.h"
#include "ADS1115.h"
#include "mock_ADS1115_port.h"

#define CONVERSION_VALUE_READ   0x5FFF
#define MASK_BITS_HIGH          0xFF00
#define MASK_BITS_LOW           0x00FF
#define UMBRAL_LOW              0x000F
#define UMBRAL_HIGH             0x00FF
#define SHIF_BITS_TO_RIGHT      8

//--------------------Funciones auxiliares--------------------
bool auxiliar_ads1115_transmit(uint8_t slaveAddres, uint8_t *word, uint16_t nroBytes){
    return FUNCTION_OK;
}

void auxiliar_ads1115_receive(uint8_t slaveAddres, uint8_t *word, uint16_t nroBytes){
    word[0] = (CONVERSION_VALUE_READ & MASK_BITS_HIGH) >> SHIF_BITS_TO_RIGHT;
    word[1] = CONVERSION_VALUE_READ & MASK_BITS_LOW;
}

bool auxiliar_ads1115_gpioReadyInit(uint8_t gpio_port, uint8_t gpio_pin){
    return FUNCTION_OK;
}

bool auxiliar_ads1115_gpioReadyRead(uint8_t gpio_port, uint8_t gpio_pin){
    return FUNCTION_OK;
}

//--------------------Tests--------------------

// Inicializar configuración del canal A0 del adc en modo single - lectura periodica
void test_iniciar_canal_modo_single(void){
    signalADS1115 channelCero;

    ADS1115_channelInit(&channelCero, SINGLE_MODE_A0);
    TEST_ASSERT_EQUAL(SINGLE_MODE_A0, channelCero.channel);
}

// Inicializar configuración de canal A1 en modo single - lectura por consulta
void test_iniciar_canal_modo_single_por_consulta(void){
    signalADS1115 channelUno;

    ADS1115_Transmit_fake.custom_fake = auxiliar_ads1115_transmit;

    TEST_ASSERT_TRUE(ADS1115_channelInitPolled(&channelUno, SINGLE_MODE_A1, SLAVE_ADDRES_GND));
}

// Inicializar pedido de lectura por consulta
void test_iniciar_lectura_por_consulta(void){
    signalADS1115 channelUno;

    ADS1115_Transmit_fake.custom_fake = auxiliar_ads1115_transmit;

    ADS1115_channelInitPolled(&channelUno, SINGLE_MODE_A1, SLAVE_ADDRES_GND);
    TEST_ASSERT_TRUE(ADS1115_startConversionPolled(&channelUno, SLAVE_ADDRES_GND));
}

// Recuperar lectura del ads1115 - lectura por consulta
void test_recuperar_un_valor_de_lectura_de_adc_por_consulta(void){
    signalADS1115 channelUno;

    ADS1115_Transmit_fake.custom_fake = auxiliar_ads1115_transmit;
    ADS1115_Receive_fake.custom_fake = auxiliar_ads1115_receive;
    ADS1115_gpioReadyInit_fake.custom_fake = auxiliar_ads1115_gpioReadyInit;
    ADS1115_gpioReadyRead_fake.custom_fake = auxiliar_ads1115_gpioReadyRead;

    TEST_ASSERT_TRUE(ADS1115_gpioReadyInit(READY_port, READY_pin));
    ADS1115_channelInitPolled(&channelUno, SINGLE_MODE_A1, SLAVE_ADDRES_GND);
    ADS1115_startConversionPolled(&channelUno, SLAVE_ADDRES_GND);
    TEST_ASSERT_EQUAL_HEX16(CONVERSION_VALUE_READ, ADS1115_getConversionPolled(&channelUno, SLAVE_ADDRES_GND));
}

// Convertir lectura obtenida en binario a lectura flotante (decimal con unidades en volts)
void test_convertir_lectura_binaria_a_flotante(void){
    signalADS1115 channelUno;
    float result = (FULL_RANGE_PG0 / CUENTA_MAXIMA_ADC) * CONVERSION_VALUE_READ;

    ADS1115_Transmit_fake.custom_fake = auxiliar_ads1115_transmit;
    ADS1115_Receive_fake.custom_fake = auxiliar_ads1115_receive;
    ADS1115_gpioReadyRead_fake.custom_fake = auxiliar_ads1115_gpioReadyRead;

    ADS1115_channelInitPolled(&channelUno, SINGLE_MODE_A1, SLAVE_ADDRES_GND);
    ADS1115_startConversionPolled(&channelUno, SLAVE_ADDRES_GND);
    ADS1115_getConversionPolled(&channelUno, SLAVE_ADDRES_GND);
    TEST_ASSERT_EQUAL_FLOAT(result, ADS1115_getValueVoltage(&channelUno));
}

// Cambiando los parámetros de configuración del canal de conversión - configurando umbral de ads1115.
void test_cambiar_parametros_de_configuracion_de_un_canal(void){
    signalADS1115 channelCero;

    ADS1115_Transmit_fake.custom_fake = auxiliar_ads1115_transmit;

    ADS1115_channelInit(&channelCero, SINGLE_MODE_A0);
    TEST_ASSERT_TRUE(ADS1115_updateThreshold(SLAVE_ADDRES_GND, UMBRAL_LOW, UMBRAL_HIGH));
    ADS1115_updateComparatorMode(&channelCero, COMPARATOR_WINDOW);
    TEST_ASSERT_EQUAL(COMPARATOR_WINDOW, channelCero.comparadorMode);
    ADS1115_updateComparatorPolarity(&channelCero, ALERT_ACTIVE_LOW);
    TEST_ASSERT_EQUAL(ALERT_ACTIVE_LOW, channelCero.comparadorPolarity);
}