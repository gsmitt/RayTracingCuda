#include "imagem.h"

Imagem::Imagem(const int largura):largura(largura){}


/**
 * @brief Transforma a cor do pixel para rgb e escreve ele no arquivo ppm, em formato binario
 * 
 * @param arquivo Arquivo final ppm
 * @param cor_pixel Cor do pixel calculada por outras funcoes
 * @param amostras_por_pixel Quantidade de antialiasing
 */
void Imagem::escreve_pixel(std::ofstream& arquivo, Vetor3 cor_pixel, int amostras_por_pixel)
{
    double r = cor_pixel.a;
    double g = cor_pixel.b;
    double b = cor_pixel.c;

    double scale = 1.0 / amostras_por_pixel;

    r = sqrt(scale*r);
    g = sqrt(scale*g);
    b = sqrt(scale*b);


    arquivo 
            << static_cast<uint8_t>(256 * clamp(r, 0.0, 0.999))
            << static_cast<uint8_t>(256 * clamp(g, 0.0, 0.999)) 
            << static_cast<uint8_t>(256 * clamp(b, 0.0, 0.999));
}
