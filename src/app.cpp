#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <algorithm>

#include <gpio.h>
#include <util.h>
#include <pid.h>
#include <bme.h>
#include <bme_aux.h>
#include <uart.h>
#include <display.h>
#include <logger.h>

#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2
#define COMANDO_USUARIO 0xC3

struct bme280_dev bme;



void encerra_execucao(int exit_code){
    printf("Encerrando a execução do programa...\n");
    fecha_log();
    desliga_resistencia();
    desliga_ventoinha();
    fecha_UART();
    exit( std::min(exit_code, 1) );
}

void controle_potenciometro(){
    double TI, TR, TE, intensidade = 0;
    pid_configura_constantes(30.0, 0.2, 400.0);
    while (1){
        TI = solicita_uart<float>(TEMP_INTERNA);
        intensidade = pid_controle(TI);

        controle_temperatura(intensidade);

        TR = solicita_uart<float>(TEMP_POTENCIOMETRO);

        pid_atualiza_referencia(TR);

        TE = get_current_temperature(&bme);
        imprime_display(TI, TR, TE, "PID ");
        escreve_temp_log(TI, TR, TE);
        sleep(1);
        
    }
}

void menu(){
    while (1)
    {
        int opcao = -1;
        printf("Escolha o modo de controle:\n");
        printf("[3] Potenciometro:\n");
        printf("[4] Enviar um int:\n");
        printf("[5] Enviar um float:\n");
        printf("[6] Enviar uma string:\n");
        printf("[7] Encerrar o programa:\n");

        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 3:
            controle_potenciometro();
            break;
        case 7:
            puts("Finalizando o programa\n");
            return;
            break;
        default:
            puts("Escolha uma opção válida...\n");
            sleep(5);
            break;
        }
    }
}

void init_APP(){
    signal(SIGINT, encerra_execucao);
    wiringPiSetup();
    configura_UART();
    desliga_resistencia();
    desliga_ventoinha();
    inicia_display();
    inicia_log();
    bme = conecta_bme();
    menu();
    encerra_execucao(0);
}
