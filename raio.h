#pragma once

#include "vetor3.h"
//#include "esfera.h"
#include <iostream>

class Raio
{
public:

    Vetor3 origem, direcao;

    //Vetor do ponto + (vetor direcao * t)
    Vetor3 at(double t) const;


    //double hit_esfera(Esfera *a);

    Raio(const Ponto3& origem, const Vetor3& direcao);
    ~Raio();
};


