#pragma once

#include "forma.h"
#include "vetor3.h"


class Esfera: public Forma
{
public:
    Vetor3 centro;
    double raio;

    Esfera(Ponto3 centro, double raio);

    virtual bool hit(Raio *r, double t_min, double t_max, hit_record& rec) override;
};
