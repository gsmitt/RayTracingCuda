#pragma once
#include "vetor3.h"
#include "raio.h"

class Material;

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
    //Material do hit
    shared_ptr<Material> material;


    //Verifica se o raio esta fora ou dentro da forma
    inline void normal_face(Raio &r, Vetor3 fora)
    {
        frente = produto_escalar(r.direcao,fora) < 0;
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