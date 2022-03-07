#ifndef GPIO_H_
#define GPIO_H_

void liga_resistencia(int valor_resistencia);
void desliga_resistencia();
void liga_ventoinha(int new_fan_value);
void desliga_ventoinha();
void controle_temperatura(int intensidade);

#endif /* GPIO_H_ */