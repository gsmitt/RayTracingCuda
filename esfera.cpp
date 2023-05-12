#include "esfera.h"

Esfera::Esfera(Ponto3 centro, double raio):centro(centro),raio(raio){}

bool Esfera::hit( Raio *r, double t_min, double t_max, hit_record& registro)
{
    Vetor3 d_o = r->origem - centro;
    double a = (r->direcao.a*r->direcao.a) + (r->direcao.b*r->direcao.b) + (r->direcao.c*r->direcao.c);
    double b = d_o.prod_escalar(r->direcao);
    double c = (d_o.a*d_o.a + d_o.b*d_o.b + d_o.c*d_o.c) - (raio*raio);
    double discriminant = b*b - a*c;

    //Se o delta for negativo, a t quadrada não é real, não colide
    if (discriminant < 0)return false;

    double t = (-b - sqrt(discriminant)) / (a);
    if (t < t_min || t_max < t)
    {
        t = (-b - sqrt(discriminant)/(a));
        if (t < t_min || t_max < t)
        {
            return false;
        }
    }

    registro.t = t;
    registro.p = r->at(t);
    Vetor3 fora = (registro.p - centro) / raio;
    registro.normal_face(r,fora);
    return true;
}