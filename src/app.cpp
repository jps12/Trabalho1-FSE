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
#include "reflow.h"
#include "terminal.h"

#define TEST_LOCAL

void encerra_execucao(int exit_code){
    printf("Encerrando a execução do programa...\n");

#ifndef TEST_LOCAL

    imprime_string_display("Desligando...");
    desliga_resistencia();
    desliga_ventoinha();
    fecha_UART();
    imprime_string_display("Recompilando.");

#endif

    fecha_log();
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

#ifndef TEST_LOCAL

    desliga_resistencia();
    desliga_ventoinha();

#endif

    while (1)
    {
        system("clear");
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
            printf("Pressione ctrl + \\ para sair desse modo e voltar ao menu\n");
            sleep(5);
            controle_potenciometro();
            break;
        case 2:
            printf("Pressione ctrl + \\ para sair desse modo e voltar ao menu\n");
            sleep(5);
            controle_reflow();
            break;
        case 3:
            printf("Pressione ctrl + \\ para sair desse modo e voltar ao menu\n");
            sleep(5);
            terminal_controle();
            break;
        case 9:
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

#ifndef TEST_LOCAL

    imprime_string_display("Carregando...");
    wiringPiSetup();
    configura_UART();
    conecta_bme();

#endif
    
    signal(SIGINT, encerra_execucao);
    signal(SIGQUIT, menu);
    inicia_log();
    // preconfigure_constantes();
    menu();
    encerra_execucao(0);
}
