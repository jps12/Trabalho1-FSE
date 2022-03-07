#ifndef LOG_H_
#define LOG_H_

#include <string>

void logger_escreve_temp(float TI, float TR, float TE);
void logger_escreve_string(std::string mensagem);
void logger_encerra();
void logger_inicia();

#endif /* LOG_H_ */