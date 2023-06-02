#pragma once

#include "raio.h"
#include "material.h"

struct hit_record
{
    //Ponto de colisao
    Ponto3 p;
    //Vetor normal da colisao com a forma
    Vetor3 normal;
    //altura onde o raio colide com a forma
    double t;
    //Verificar se a visao da forma nao vai ser obstruida por outro objeto.
    bool frente;
    //Cor do objeto
    Cor cor;

    Material *mat_ptr;

    //Verifica se o raio esta fora ou dentro da forma
    inline void normal_face(Raio *r, Vetor3 fora)
    {
        frente = r->direcao.prod_escalar(fora) < 0;
        if (frente)
        {
            normal = fora;
        }
        else
        {
            //Se estiver dentro, usar o vetor oposto ao normal.
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
    inline virtual bool hit(Raio *r, double t_min, double t_max, hit_record& registro) = 0;
};