#include <gpio.h>
#include <softPwm.h>
#include <wiringPi.h>
#include <stdlib.h>

#define RESISTOR_PIN 4
#define FAN_PIN 5

void liga_resistencia(int valor_resistencia) {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, valor_resistencia);
}

void desliga_resistencia() {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, 0);
}

void liga_ventoinha(int new_fan_value) {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, new_fan_value);
}

void desliga_ventoinha() {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, 0);
}

void controle_temperatura(int intensidade) {
  if (intensidade > 0) {
    liga_resistencia(intensidade);
    desliga_ventoinha();
  } 
  else if (intensidade >= -40){
    desliga_ventoinha();
    desliga_resistencia();
  }
  else {
    liga_ventoinha(std::abs(intensidade));
    desliga_resistencia();
  }
}