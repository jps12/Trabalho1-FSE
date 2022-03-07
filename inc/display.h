#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADDR 0x27

#define LCD_CHR 1
#define LCD_CMD 0

#define LINE1 0x80
#define LINE2 0xC0

#define LCD_BACKLIGHT 0x08

#define ENABLE 0b00000100

void inicia_display(void);
void imprime_temp_display(float TI,float TR, float TE, std::string titulo);
void imprime_string_display(std::string linha1, std::string linha2 = "");


#endif