#pragma once

#include <cmath>
#include <iostream>

class Vetor3
{
public:
    double a, b, c;

    Vetor3();
    Vetor3(double a, double b, double c);

    double modulo();
    double prod_escalar(const Vetor3 &v);

    Vetor3& operator+=(const Vetor3 &v);
    Vetor3& operator*=(const double t);

    friend Vetor3 operator*(double t, const Vetor3 &v);
    friend Vetor3 operator/(const Vetor3 &v, double t);
    Vetor3 operator+(const Vetor3 &v) const;
    Vetor3 operator-(const Vetor3 &v) const;

    Vetor3 unit_vector();
};

using Ponto3 = Vetor3;
using Cor = Vetor3;