#pragma once
#include "vetor3.h"
#include "raio.h"
#include "utils.h"

class Camera {
    public:
        Camera();
        Camera(Ponto3 origem, Vetor3 canto_esq_inf, Vetor3 horizontal, Vetor3 vertical);

        Vetor3 origem;
        Vetor3 canto_esq_inf;
        Vetor3 horizontal;
        Vetor3 vertical;

        Raio get_raio(double u, double v) const;
};