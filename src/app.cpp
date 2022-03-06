#include <stdio.h>
#include <string.h> 
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <algorithm>

#include <gpio.h>
#include <uart.h>
#include <crc.h>
#include <display.h>

#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2
#define COMANDO_USUARIO 0xC3


void encerra_execucao(int exit_code){
    printf("Encerrando a execução do programa...\n");
    fecha_UART();
    exit( std::min(exit_code, 1) );
}

std::string convert_to_string(int value){
    unsigned char *p = (unsigned char *) &value;
    std::string retorno = "****";
    for (int i=0;i<4;i++){
        retorno[i] = *p;
        p++;
    } 
    return retorno;
}

template<typename tipo_retorno>
tipo_retorno solicita_uart(int sub_codigo){
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
    tipo_retorno resposta;
    memcpy(&resposta, resposta_buffer+3, 4);
    free(resposta_buffer);
    return resposta;
}



int envia_uart(int sub_codigo, std::string value){
    unsigned char buffer[13];
    unsigned char *p_buffer;
    p_buffer = &buffer[0];
    *p_buffer++ = (char) 0x01;
    *p_buffer++ = (char) 0x16;
    *p_buffer++ = (char) sub_codigo;
    *p_buffer++ = (char) 3;
    *p_buffer++ = (char) 7;
    *p_buffer++ = (char) 4;
    *p_buffer++ = (char) 3;
    *p_buffer++ = value[0];
    *p_buffer++ = value[1];
    *p_buffer++ = value[2];
    *p_buffer++ = value[3];

    configura_CRC(buffer, 13);

    printf("Buffers de memória criados!\n");
    
    escreve_mensagem(&buffer[0], 13);

    sleep(1);

    unsigned char *resposta_buffer = recebe_resposta();
    int resposta;
    memcpy(&resposta, resposta_buffer+3, 4);
    free(resposta_buffer);
    return resposta;
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
        printf("[1] Liga placa:\n");
        printf("[3] Potenciometro:\n");
        printf("[4] Enviar um int:\n");
        printf("[5] Enviar um float:\n");
        printf("[6] Enviar uma string:\n");
        printf("[7] Encerrar o programa:\n");

        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 1:
            envia_uart(0xD3, convert_to_string(1));
            break;
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
