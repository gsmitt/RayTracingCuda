#include "esfera.h"

Esfera::Esfera(Ponto3 centro, double raio, Cor cor):centro(centro),raio(raio),cor(cor){}

//Construtor com mateiral
Esfera::Esfera(Ponto3 centro, double raio, Material* material):centro(centro),raio(raio),material(material){}

bool Esfera::hit( Raio *r, double t_min, double t_max, hit_record& registro)
{
    Vetor3 d_o = r->origem - centro;
    double a = (r->direcao.a*r->direcao.a) + (r->direcao.b*r->direcao.b) + (r->direcao.c*r->direcao.c);
    double b = d_o.prod_escalar(r->direcao);
    double c = (d_o.a*d_o.a + d_o.b*d_o.b + d_o.c*d_o.c) - (raio*raio);
    double delta = b*b - a*c;

    //Se o delta for negativo, a t quadrada não é real, não colide
    if (delta < 0) return false;

    //Busca a raiz mais proxima no range de visao.
    double t = (-b - sqrt(delta)) / (a);
    if (t < t_min || t_max < t)
    {
        t = (-b + sqrt(delta)/(a));
        if (t < t_min || t_max < t)
        {
            return false;
        }
    }

    registro.cor = cor;
    //Adiciona o t necessario para a colisao no registro
    registro.t = t;
    //Calcula o ponto de colisao
    registro.p = r->at(t);
    //Calcula o vetor normal do ponto da esfera
    Vetor3 fora = (registro.p - centro) / raio;
    registro.normal_face(r,fora);
    registro.mat_ptr = material;

    return true;
}