#include <softPwm.h>
#include <wiringPi.h>
#include <stdlib.h>

#define RESISTOR_PIN 4
#define FAN_PIN 5

void gpio_liga_resistencia(int valor_resistencia)
{
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, valor_resistencia);
}

void gpio_desliga_resistencia()
{
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, 0);
}

void gpio_liga_ventoinha(int new_fan_value)
{
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, new_fan_value);
}

void gpio_desliga_ventoinha()
{
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, 0);
}

void gpio_controle_temperatura(int intensidade)
{
  if (intensidade > 0)
  {
    gpio_liga_resistencia(intensidade);
    gpio_desliga_ventoinha();
  }
  else if (intensidade >= -40)
  {
    gpio_desliga_ventoinha();
    gpio_desliga_resistencia();
  }
  else
  {
    gpio_liga_ventoinha(std::abs(intensidade));
    gpio_desliga_resistencia();
  }
}