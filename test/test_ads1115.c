#include "unity.h"
#include "ADS1115.h"
#include "mock_ADS1115_port.h"

//Inicializar configuración del canal A0 del adc en modo single - lectura periodica
void test_iniciar_canal_modo_single(void) {
    signalADS1115 channelCero;

    ADS1115_channelInit(&channelCero,SINGLE_MODE_A0);
    TEST_ASSERT_EQUAL(SINGLE_MODE_A0,channelCero.channel);
}
