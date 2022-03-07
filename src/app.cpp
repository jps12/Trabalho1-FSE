#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <algorithm>

#include <gpio.h>
#include <util.h>
#include <uart.h>
#include <display.h>

#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2
#define COMANDO_USUARIO 0xC3


void encerra_execucao(int exit_code){
    printf("Encerrando a execução do programa...\n");
    fecha_UART();
    exit( std::min(exit_code, 1) );
}

void controle_potenciometro(){
    while (1){
        sleep(1);
        int comando = solicita_uart<int>(COMANDO_USUARIO);
        printf("Comando recebido: %d\n", comando);
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
    configura_UART();
    desliga_resistencia();
    desliga_ventoinha();
    lcd_init();
    menu();
    encerra_execucao(0);
}
