#ifndef LOG_H_
#define LOG_H_

#include<string>

void escreve_temp_log(float TI, float TR, float TE);
void escreve_string_log(std::string mensagem);
void fecha_log();
void inicia_log();

#endif /* LOG_H_ */