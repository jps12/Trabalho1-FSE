#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <uart.h>

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

void fecha_UART(){
    close(uart0_filestream);
}