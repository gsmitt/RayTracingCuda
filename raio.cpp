#include "raio.h"


Raio::Raio(const Vetor3& origem, const Vetor3& direcao):
origem(origem),direcao(direcao)
{
}

double Raio::hit_esfera(Esfera *a)
{
    Vetor3 d_o = origem - a->centro;
    double aa = direcao.prod_escalar(direcao);
    double b = 2.0 * d_o.prod_escalar(direcao);
    double c = d_o.prod_escalar(d_o) - (a->raio*a->raio);
    double discriminant = b*b - 4*aa*c;

    if (discriminant < 0)
    {
        return -1.0;   
    }else
    {
        return (-b - sqrt(discriminant)) / 2.0  * aa;
    }
    
    

    return (discriminant > 0);
}

Vetor3 Raio::at(double t) const{
    return origem + t*direcao;
}

Raio::~Raio()
{
}