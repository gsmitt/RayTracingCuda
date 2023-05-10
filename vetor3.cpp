#include "vetor3.h"

Vetor3::Vetor3() : a(0), b(0), c(0) {}

Vetor3::Vetor3(double a, double b, double c) : a(a), b(b), c(c) {}

double Vetor3::modulo() {
    return sqrt(a * a + b * b + c * c);
}

double Vetor3::prod_escalar(const Vetor3 &v) {
    return a * v.a + b * v.b + c * v.c;
}

Vetor3 Vetor3::unit_vector()
{
    double mod = modulo();
    return Vetor3(a/mod, b/mod, c/mod);
}


Vetor3& Vetor3::operator*=(const double t) {
    a *= t;
    b *= t;
    c *= t;
    return *this;
}

Vetor3& Vetor3::operator+=(const Vetor3 &v) {
    a += v.a;
    b += v.b;
    c += v.c;
    return *this;
}

Vetor3 operator*(double t, const Vetor3 &v) {
    return Vetor3(t * v.a, t * v.b, t * v.c);
}

Vetor3 Vetor3::operator+(const Vetor3 &v) const {
    return Vetor3(a + v.a, b + v.b, c + v.c);
}

Vetor3 Vetor3::operator-(const Vetor3 &v) const {
    return Vetor3(a - v.a, b - v.b, c - v.c);
}

Vetor3 operator/(const Vetor3 &v, double t) {
    return Vetor3(v.a / t, v.b / t, v.c / t);
}

