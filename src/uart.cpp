#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <crc.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <uart.h>

template int solicita_uart<int>(int sub_codigo);
template float solicita_uart<float>(int sub_codigo);

static int uart0_filestream = -1; 

void check_UART(){
    if(uart0_filestream == -1)
    {
        printf("UART não configurada... Terminando execução\n");
        fecha_UART();
        exit(1);
    }
}

void configura_UART(){

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    
    check_UART();
    printf("UART inicializada!\n");

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void escreve_mensagem(unsigned char *buffer, int size){
    check_UART();

    printf("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &buffer[0], size);

    if (count < 0)
    {
        printf("UART TX error\n");
    }
    else
    {
        printf(" %d bytes escrito.\n", count);
    }

}

unsigned char *recebe_resposta(){

    check_UART();

    //----- CHECK FOR ANY RX BYTES -----
    // Read up to 255 characters from the port if they are there
    unsigned char *rx_buffer;

    rx_buffer = (unsigned char *) malloc(256 * sizeof(unsigned char));

    int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0)
    {
        printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
    }
    else if (rx_length == 0)
    {
        printf("Nenhum dado disponível.\n"); //No data waiting
    }
    else
    {
        //Bytes received
        rx_buffer[rx_length] = '\0';
    }

    return rx_buffer;
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

void fecha_UART(){
    close(uart0_filestream);
}