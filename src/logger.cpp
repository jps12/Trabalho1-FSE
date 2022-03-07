#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "app.h"

static std::string nome_arquivo = "./log/log.csv";
static std::ofstream arquivo;
static std::string tempo_agora;

void get_tempo(){
    time_t time_now;
    time(&time_now);
    tempo_agora = ctime(&time_now);
    tempo_agora.pop_back();
}

void inicia_log(){
    arquivo.open(nome_arquivo);
    if (!arquivo.is_open()){
        printf("Erro ao abrir o arquivo\n");
        encerra_execucao(1);
    }
}

void escreve_temp_log(float TI, float TR, float TE){
    get_tempo();
    arquivo << tempo_agora << ',' << TI << ',' << TR << ',' << TE << '\n';
}

void escreve_string_log(std::string mensagem){
    get_tempo();
    arquivo << tempo_agora << ',' << mensagem << '\n';
}

void fecha_log(){
    arquivo.close();
}