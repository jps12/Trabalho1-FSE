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
void imprime_display(float tempI, float tempR, float tempE, std::string titulo);

// added by Lewis
void typeFloat(float myFloat);
void displayLoc(int line);  //move cursor
void typeln(const char *s);

#endif