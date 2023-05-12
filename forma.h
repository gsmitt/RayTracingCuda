#pragma once

#include "raio.h"

struct hit_record
{
    Ponto3 p;
    Vetor3 normal;
    double t;
    bool frente;

    inline void normal_face(Raio *r, Vetor3 fora)
    {
        frente = r->direcao.prod_escalar(fora) < 0;
        if (frente)
        {
            normal = fora;
        }
        else
        {
            normal = fora/(-1);
        }
    }
};

/**
 * @brief Classe abstrata forma
 *
 */
class Forma
{
public:
    virtual bool hit(Raio *r, double t_min, double t_max, hit_record& registro) = 0;
};