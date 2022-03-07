#ifndef UART_H_
#define UART_H_

void UART_configura();
template <typename tipo_retorno>
tipo_retorno UART_solicita(int sub_codigo);
void UART_encerra();

#endif /* UART_H_ */
