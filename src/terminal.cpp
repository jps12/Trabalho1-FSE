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
        TI = solicita_uart<float>(TEMP_INTERNA);
        intensidade = pid_controle(TI);

        controle_temperatura(intensidade);

        pid_atualiza_referencia(TR);

        TE = get_current_temperature();
        imprime_temp_display(TI, TR, TE, "PID ");
        escreve_temp_log(TI, TR, TE);
        sleep(1);
    }
}