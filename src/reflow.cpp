#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <tuple>

#include <app.h>
#include "gpio.h"
#include "logger.h"
#include "uart.h"
#include "pid.h"
#include "display.h"
#include "bme_aux.h"

#define LIMITE_TEMPO 60
#define TEMP_INTERNA 0xC1
#define TEMP_POTENCIOMETRO 0xC2
#define COMANDO_USUARIO 0xC3

std::ifstream arquivo;
std::string nome_arquivo = "./assets/reflow.csv";
std::vector<std::tuple<int, float>> valores;
static int mod;

void inicia_arquivo_reflow(){
    arquivo.open(nome_arquivo);

    if (!arquivo.is_open()) {
        printf("Erro ao abrir o arquivo\n");
        encerra_execucao(1);
    }

    std::string linha,temperatura,tempo;

    arquivo >> linha;

    while (arquivo >> linha){
        std::stringstream ss(linha);
        std::getline(ss, tempo, ',');
        std::getline(ss, temperatura, '\n');
        valores.push_back(std::make_tuple(stoi(tempo), stof(temperatura)));
    }
    arquivo.close();
    std::sort(valores.begin(), valores.end());
    float _t;
    std::tie(mod, _t) = valores.back();
}

bool atualiza_tr(float& TR, int& tempo_atual){
    int index = std::upper_bound(valores.begin(), valores.end(), std::make_tuple(tempo_atual % mod, 0.0)) - valores.begin();
    index--;
    int _tempo;
    std::tie(_tempo, TR) = valores[index];
    
    tempo_atual++;

    if (tempo_atual > 3600) 
        return false;

    return true;
}

void controle_reflow(){
    inicia_arquivo_reflow();
    float TR, TI, TE;
    double intensidade;
    int tempo_atual = 0;
    while(atualiza_tr(TR, tempo_atual)){
        TI = solicita_uart<float>(TEMP_INTERNA);
        intensidade = pid_controle(TI);

        controle_temperatura(intensidade);

        pid_atualiza_referencia(TR);

        TE = get_current_temperature();

        imprime_temp_display(TI, TR, TE, "PID ");
        escreve_temp_log(TI, TR, TE);

        std::string message = "Tempo: " + std::to_string(tempo_atual) + ",TR: " + std::to_string(TR);
        escreve_string_log(message);
        sleep(1);
    }
}