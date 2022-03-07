#include <unistd.h>

#include "logger.h"
#include "display.h"
#include "bme_aux.h"
#include "uart.h"
#include "gpio.h"
#include "pid.h"

#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2

void potenciometro_controle()
{
    double TI, TR, TE, intensidade = 0;
    while (1)
    {
        TI = UART_solicita<float>(TEMP_INTERNA);
        
        intensidade = pid_controle(TI);
        gpio_controle_temperatura(intensidade);

        TR = UART_solicita<float>(TEMP_POTENCIOMETRO);

        pid_atualiza_referencia(TR);

        TE = bme_temperatura_atual();

        display_imprime_temp(TI, TR, TE, "PID ");
        logger_escreve_temp(TI, TR, TE);
        
        sleep(1);
    }
}