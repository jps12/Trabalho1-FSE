#ifndef UART_H_
#define UART_H_


void configura_UART();
void escreve_mensagem(unsigned char *buffer, int size);
unsigned char *recebe_resposta();
template<typename tipo_retorno> tipo_retorno solicita_uart(int sub_codigo);
void fecha_UART();

#endif /* UART_H_ */
