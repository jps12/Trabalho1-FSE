#include <stdio.h>
#include <crc.h>
#include <uart.h>
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <algorithm>


void encerra_execucao(int exit_code){
    fecha_UART();
    printf("Encerrando a execução do programa...\n");
    exit_code = std::min(exit_code, 1);
    exit(exit_code);
}


int conta_bytes_lidos(unsigned char *buffer){
    int i = 0;
    while (buffer[i] != '\0' && i != 256) i++;
    return i < 256? i : -1;
}

void solicita_int(){
    unsigned char buffer[5];
    unsigned char *p_buffer;
    p_buffer = &buffer[0];
    *p_buffer++ = (char) 0x01;
    *p_buffer++ = (char) 0x23;
    *p_buffer++ = (char) 0xA1;

    configura_CRC(buffer, 5);

    printf("Buffers de memória criados!\n");
    
    escreve_mensagem(&buffer[0], 5);

    sleep(1);

    unsigned char *resposta_buffer = recebe_resposta();
    // int resposta_tam = conta_bytes_lidos(resposta_buffer);
    int resposta ;
    memcpy(&resposta, resposta_buffer+3, 4);

    printf("O inteiro recebido foi: %d\n", resposta);
}

void menu(){
    while (1)
    {
        int opcao = -1;
        printf("Escolha o numero da opção que deseja:\n");
        printf("[1] Receber um int:\n");
        printf("[2] Receber um float:\n");
        printf("[3] Receber uma string:\n");
        printf("[4] Enviar um int:\n");
        printf("[5] Enviar um float:\n");
        printf("[6] Enviar uma string:\n");
        printf("[7] Encerrar o programa:\n");

        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 1:
            solicita_int();
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
    menu();
    encerra_execucao(0);
}
