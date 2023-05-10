#include "esfera.h"

Esfera::Esfera(Ponto3 centro, double raio):centro(centro),raio(raio){}

bool Esfera::hit( Raio *r, double t_min, double t_max, hit_record& registro)
{
    Vetor3 d_o = r->origem - centro;
    double a = r->direcao.prod_escalar(r->direcao);
    double b = 2.0 * d_o.prod_escalar(r->direcao);
    double c = d_o.prod_escalar(d_o) - (raio*raio);
    double discriminant = b*b - 4*a*c;

    //Se o delta for negativo, a t quadrada não é real, não colide
    if (discriminant < 0)return false;

    double t = (-b - sqrt(discriminant)/(2*a));
    if (t < t_min || t_max < t)
    {
        t = (-b - sqrt(discriminant)/(2*a));
        if (t < t_min || t_max < t)
        {
            return false;
        }
    }

    registro.t = t;
    registro.p = r->at(t);
    Vetor3 fora = (registro.p - centro)/ raio;
    registro.normal_face(r,fora);
    return true;
}