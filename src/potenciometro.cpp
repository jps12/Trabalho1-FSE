#include <uart.h>
#include <pid.h>
#include <gpio.h>
#include <bme_aux.h>
#include <display.h>
#include <logger.h>
#include <unistd.h>

#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2
#define COMANDO_USUARIO 0xC3

void controle_potenciometro(){
    double TI, TR, TE, intensidade = 0;
    while (1){
        TI = solicita_uart<float>(TEMP_INTERNA);
        intensidade = pid_controle(TI);

        controle_temperatura(intensidade);

        TR = solicita_uart<float>(TEMP_POTENCIOMETRO);

        pid_atualiza_referencia(TR);

        TE = get_current_temperature();
        imprime_temp_display(TI, TR, TE, "PID ");
        escreve_temp_log(TI, TR, TE);
        sleep(1);
    }
}