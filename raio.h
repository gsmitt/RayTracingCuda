#pragma once

#include "vetor3.h"
//#include "esfera.h"
#include <iostream>

class Raio
{
public:

    Vetor3 origem, direcao;

    //Ponto3d + (vetor direcao * t)
    Ponto3 at(double t) const;

    Cor cor_raio() const;

    Raio(const Ponto3& origem, const Vetor3& direcao);
    Raio();
    ~Raio();
};


