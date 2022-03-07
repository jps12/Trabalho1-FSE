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

// #define TEST_LOCAL

void encerra_execucao(int exit_code)
{
    printf("Encerrando a execução do programa...\n");

#ifndef TEST_LOCAL

    gpio_desliga_resistencia();
    gpio_desliga_ventoinha();
    UART_encerra();
    display_imprime_string("Desligado.");

#endif

    logger_encerra();
    exit(std::min(exit_code, 1));
}

void preconfigure_constantes()
{
    int opcao = -1;
    while (opcao != 1 and opcao != 0)
    {
        printf("Digite 0 para usar os valores padrões:\nKP:50\nKI:0.2\nKD:400\n");
        printf("Digite 1 para configurar os valores\n");
        scanf(" %d", &opcao);
        switch (opcao)
        {
        case 0:
            return;
        case 1:
            break;
        default:
            printf("Escolha uma opção válida\n");
            sleep(5);
            system("clear");
            break;
        }
    }

    system("clear");

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

void menu(int value = 0)
{

#ifndef TEST_LOCAL

    gpio_desliga_resistencia();
    gpio_desliga_ventoinha();

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
            printf("Pressione ctrl + \\ ou ctrl + c para sair desse modo encerrar o programa\n");
            sleep(5);
            potenciometro_controle();
            break;
        case 2:
            printf("Pressione ctrl + \\ ou ctrl + c para sair desse modo encerrar o programa\n");
            sleep(5);
            reflow_controle();
            break;
        case 3:
            printf("Pressione ctrl + \\ ou ctrl + c para sair desse modo encerrar o programa\n");
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

void app_inicia()
{

#ifndef TEST_LOCAL

    display_imprime_string("Carregando...");
    wiringPiSetup();
    UART_configura();
    bme_conecta();

#endif

    signal(SIGINT, encerra_execucao);
    signal(SIGQUIT, encerra_execucao);
    logger_inicia();
    preconfigure_constantes();
    menu();
    encerra_execucao(0);
}
