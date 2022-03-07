#include <uart.h>
#include <pid.h>
#include <gpio.h>
#include <bme_aux.h>
#include <display.h>
#include <logger.h>
#include <unistd.h>

#define TEMP_INTERNA 0xC1

static float TR = 0;

void terminal_atualiza_tr(float _TR)
{
    TR = _TR;
}

void terminal_controle()
{
    double TI, TE, intensidade = 0;
    while (1)
    {
        TI = UART_solicita<float>(TEMP_INTERNA);
        intensidade = pid_controle(TI);

        gpio_controle_temperatura(intensidade);

        pid_atualiza_referencia(TR);

        TE = bme_temperatura_atual();
        display_imprime_temp(TI, TR, TE, "PID ");
        logger_escreve_temp(TI, TR, TE);
        sleep(1);
    }
}