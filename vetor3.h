#pragma once
#include <stdio.h>
#include <iostream>
#include <cmath>


class Vetor3
{
public:
    Vetor3(double a, double b, double c);
    Vetor3();

    Vetor3& operator+=(const Vetor3 &v);
    Vetor3& operator*=(const double t);
    Vetor3& operator/=(const double t);

    double modulo() const;

    double modulo_quadrado() const;



public:
    double a, b, c;

};

inline std::ostream& operator<<(std::ostream &out, const Vetor3 &v) 
{
    return out << v.a << ' ' << v.b << ' ' << v.c;
}

inline Vetor3 operator+(const Vetor3 &u, const Vetor3 &v) 
{
    return Vetor3(u.a + v.a, u.b + v.b, u.c + v.c);
}

inline Vetor3 operator-(const Vetor3 &u, const Vetor3 &v) 
{
    return Vetor3(u.a - v.a, u.b - v.b, u.c - v.c);
}

inline Vetor3 operator*(const Vetor3 &u, const Vetor3 &v) 
{
    return Vetor3(u.a * v.a, u.b * v.b, u.c * v.c);
}

inline Vetor3 operator*(double t, const Vetor3 &v) 
{
    return Vetor3(t*v.a, t*v.b, t*v.c);
}

inline Vetor3 operator*(const Vetor3 &v, double t)
{
    return t * v;
}

inline Vetor3 operator/(Vetor3 v, double t) 
{
    return (1/t) * v;
}

inline double produto_escalar(const Vetor3 &u, const Vetor3 &v) 
{
    return u.a * v.a + u.b * v.b + u.c * v.c;
}

inline Vetor3 produto_vetorial(const Vetor3 &u, const Vetor3 &v) 
{
    return Vetor3(u.b * v.c - u.c * v.b,
                u.c * v.a - u.a * v.c,
                u.a * v.b - u.b * v.a);
}

inline Vetor3 unit_vector(Vetor3 v) {
    return v / v.modulo();
}

using Ponto3 = Vetor3;  
using Cor = Vetor3;    
