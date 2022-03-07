#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <wiringPi.h>
#include <wiringPiI2C.h>

void display_inicia(void);
void display_imprime_temp(float TI, float TR, float TE, std::string titulo);
void display_imprime_string(std::string linha1, std::string linha2 = "");

#endif // DISPLAY_H_