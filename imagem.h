#pragma once
#include "vetor3.h"
#include "raio.h"
#include <fstream>
#include "utils.h"

/**
 * @brief Classe Imagem que define as dimensoes e propriedades da imagem
 * 
 */
class Imagem
{
public:
    Imagem(const int largura);

    void escreve_pixel(std::ofstream& arquivo, Vetor3 cor_pixel, int amostras_por_pixel);

    const double aspect_ratio = RATIO_16_9;
    const int largura;
    const int altura = largura/aspect_ratio;
};

