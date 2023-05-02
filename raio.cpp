#include "raio.h"


Raio::Raio(const Vetor3& origem, const Vetor3& direcao):
origem(origem),direcao(direcao)
{
}

bool Raio::hit_esfera(Esfera *a)
{
    Vetor3 d_o = origem - a->centro;
    double aa = direcao.prod_escalar(direcao);
    double b = 2.0 * d_o.prod_escalar(direcao);
    double c = d_o.prod_escalar(d_o) - (a->raio*a->raio);
    double discriminant = b*b - 4*aa*c;

    if (discriminant > 0)
    {
        //std::cout << "discriminante" << discriminant << "\n";
    }
    

    return (discriminant > 0);
}

Raio::~Raio()
{
}