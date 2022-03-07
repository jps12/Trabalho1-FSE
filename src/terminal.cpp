#include <uart.h>
#include <pid.h>
#include <gpio.h>
#include <bme_aux.h>
#include <display.h>
#include <logger.h>
#include <unistd.h>

#define TEMP_INTERNA 0xC1

static float TR = 0;

void terminal_le_tr()
{
    printf("Insira a temperatura de referência\n");
    scanf(" %f", &TR);
}

void terminal_controle()
{
    terminal_le_tr();
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