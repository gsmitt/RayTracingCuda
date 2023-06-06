#include "raio.h"


Raio::Raio(const Ponto3& origem, const Vetor3& direcao):
origem(origem),direcao(direcao)
{
}

Raio::Raio() {}

/**
 * @brief Funcao que retorna o vetor do raio com o tamanho t
 * 
 * @param t Escalar que multiplica o vetor direcao
 * @return Vetor3 
 */
Vetor3 Raio::at(double t) const{
    return origem + t*direcao;
}

/**
 * @brief Retorna a cor que o raio encontrou
 * 
 * @return Cor 
 */
Cor Raio::cor_raio() const
{
    Cor unit_direction = unit_vector(direcao);
    auto t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(1.0, 1.0, 1.0) + t*Cor(0.5, 0.7, 1.0);
}


Raio::~Raio()
{
}
