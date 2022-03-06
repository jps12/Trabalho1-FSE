#include <stdio.h>
#include <crc.h>
#include <uart.h>
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <algorithm>

#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2


void encerra_execucao(int exit_code){
    printf("Encerrando a execução do programa...\n");
    fecha_UART();
    exit( std::min(exit_code, 1) );
}


int conta_bytes_lidos(unsigned char *buffer){
    int i = 0;
    while (buffer[i] != '\0' && i != 256) i++;
    return i < 256? i : -1;
}

void solicita_temperatura(int sub_codigo){
    unsigned char buffer[9];
    unsigned char *p_buffer;
    p_buffer = &buffer[0];
    *p_buffer++ = (char) 0x01;
    *p_buffer++ = (char) 0x23;
    *p_buffer++ = (char) sub_codigo;
    *p_buffer++ = (char) 3;
    *p_buffer++ = (char) 7;
    *p_buffer++ = (char) 4;
    *p_buffer++ = (char) 3;

    configura_CRC(buffer, 9);

    printf("Buffers de memória criados!\n");
    
    escreve_mensagem(&buffer[0], 9);

    sleep(1);

    unsigned char *resposta_buffer = recebe_resposta();
    // int resposta_tam = conta_bytes_lidos(resposta_buffer);
    float resposta;
    memcpy(&resposta, resposta_buffer+3, 4);

    printf("A temperatura recebida foi: %f\n", resposta);

    free(resposta_buffer);
}

void menu(){
    while (1)
    {
        int opcao = -1;
        printf("Escolha o modo de controle:\n");
        printf("[1] Solicita temperatura interna:\n");
        printf("[2] Solicita temperatura do potenciometro:\n");
        printf("[3] Potenciometro:\n");
        printf("[4] Enviar um int:\n");
        printf("[5] Enviar um float:\n");
        printf("[6] Enviar uma string:\n");
        printf("[7] Encerrar o programa:\n");

        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 1:
            solicita_temperatura(TEMP_INTERNA);
            break;
        case 2:
            solicita_temperatura(TEMP_POTENCIOMETRO);
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
