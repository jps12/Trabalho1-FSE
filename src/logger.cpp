#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


static std::string nome_arquivo = "../log/log.csv";
static std::ofstream arquivo;
static std::string tempo_agora;

void get_tempo(){
    time_t time_now;
    time(&time_now);
    tempo_agora = ctime(&time_now);
}

void inicia_log(){
    arquivo.open(nome_arquivo);
}

void escreve_temp_log(int TI, int TR, int TE){
    get_tempo();
    arquivo << tempo_agora << ',' << TI << ',' << TR << ',' << TE << '\n';
}

void fecha_log(){
    arquivo.close();
}