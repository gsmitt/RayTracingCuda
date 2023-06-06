#include "vetor3.h"


Vetor3::Vetor3(double a, double b, double c) : a{a},b{b},c{c}
{
}

Vetor3::Vetor3() : a{1},b{0},c{0}
{
}

Vetor3& Vetor3::operator+=(const Vetor3 &v)
{
    a += v.a;
    b += v.b;
    c += v.c;
    return *this;
}

Vetor3& Vetor3::operator*=(const double t)
{
    a *= t;
    b *= t;
    c *= t;
    return *this;
}

Vetor3& Vetor3::operator/=(const double t) 
{
    return *this *= 1/t;
}

double Vetor3::modulo_quadrado() const
{
    return a*a + b*b + c*c;
}

double Vetor3::modulo() const
{
    return sqrt(modulo_quadrado());
}
