#pragma once

#include "forma.h"


/**
 * @brief Classe Esfera
 * 
 */
class Esfera: public Forma
{
public:
    Vetor3 centro;
    double raio;
    shared_ptr<Material> mat_ptr;
    Cor cor;

    Esfera(Ponto3 centro, double raio, Cor cor = Cor(0,1,0));

    Esfera(Ponto3 centro, double raio, shared_ptr<Material> m);


    bool hit(Raio &r, double t_min, double t_max, hit_record& rec);
};
