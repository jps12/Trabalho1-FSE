#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "app.h"

static std::string nome_arquivo = "./log/log.csv";
static std::ofstream arquivo;
static std::string tempo_agora;

void logger_get_tempo()
{
    time_t time_now;
    time(&time_now);
    tempo_agora = ctime(&time_now);
    tempo_agora.pop_back();
}

void logger_inicia()
{
    arquivo.open(nome_arquivo);
    if (!arquivo.is_open())
    {
        printf("Erro ao abrir o arquivo\n");
        encerra_execucao(1);
    }
}

void logger_escreve_temp(float TI, float TR, float TE)
{
    logger_get_tempo();
    arquivo << tempo_agora << ',' << TI << ',' << TR << ',' << TE << '\n';
}

void logger_escreve_string(std::string mensagem)
{
    logger_get_tempo();
    arquivo << tempo_agora << ',' << mensagem << '\n';
}

void logger_encerra()
{
    arquivo.close();
}