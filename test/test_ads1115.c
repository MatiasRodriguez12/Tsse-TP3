#include "unity.h"
#include "ADS1115.h"
#include "mock_ADS1115_port.h"

//--------------------Funciones auxiliares--------------------
bool auxiliar_ads1115_transmit(uint8_t slaveAddres, uint8_t *word,uint16_t nroBytes) {
   return FUNCTION_OK;
}


//--------------------Tests--------------------

//Inicializar configuración del canal A0 del adc en modo single - lectura periodica
void test_iniciar_canal_modo_single(void) {
    signalADS1115 channelCero;

    ADS1115_channelInit(&channelCero,SINGLE_MODE_A0);
    TEST_ASSERT_EQUAL(SINGLE_MODE_A0,channelCero.channel);
}

//Inicializar configuración de canal A1 en modo single - lectura por consulta
void test_iniciar_canal_modo_diferential(void){
    signalADS1115 channelUno;

    ADS1115_Transmit_fake.custom_fake = auxiliar_ads1115_transmit;
    TEST_ASSERT_TRUE(ADS1115_channelInitPolled(&channelUno,SINGLE_MODE_A1,SLAVE_ADRRES_GND));
}