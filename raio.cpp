#include "raio.h"


Raio::Raio(const Ponto3& origem, const Vetor3& direcao):
origem(origem),direcao(direcao)
{
}

Vetor3 Raio::at(double t) const{
    return origem + t*direcao;
}

Raio::~Raio()
{
}