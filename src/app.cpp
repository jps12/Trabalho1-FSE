#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "gpio.h"
#include "potenciometro.h"
#include "bme_aux.h"
#include "uart.h"
#include "display.h"
#include "logger.h"
#include "pid.h"
#include "app.h"

void encerra_execucao(int exit_code){
    printf("Encerrando a execução do programa...\n");
    imprime_string_display("Desligando...");
    fecha_log();
    desliga_resistencia();
    desliga_ventoinha();
    fecha_UART();
    imprime_string_display("Desligado.");
    exit( std::min(exit_code, 1) );
}

void preconfigure_constantes(){
    double kp, ki, kd;
    printf("Configure os parâmetros do PID\n");
    printf("Digite a constante de controle proporcional (kp): \n");
    scanf(" %f", &kp);
    printf("Digite a constante de controle integral (ki): \n");
    scanf(" %f", &ki);
    printf("Digite a constante de controle derivativo (kd): \n");
    scanf(" %f", &kd);

    pid_configura_constantes(kp, ki, kd);
    system("clear");
}


void menu(int value = 0){

    desliga_resistencia();
    desliga_ventoinha();

    while (1)
    {
        int opcao = -1;
        printf("Escolha o modo de controle:\n");
        printf("[1] Potenciometro:\n");
        printf("[2] Curva reflow:\n");
        printf("[3] Terminal:\n");
        printf("[9] Terminar execução:\n");


        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 1:
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

        system("clear");
    }
}

void init_APP(){
    imprime_string_display("Carregando...");
    signal(SIGINT, encerra_execucao);
    signal(SIGQUIT, menu);
    wiringPiSetup();
    configura_UART();
    inicia_log();
    conecta_bme();
    // preconfigure_constantes();
    menu();
    encerra_execucao(0);
}
