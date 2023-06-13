#include "camera.h"


Camera::Camera()
{
    Ponto3 lookfrom;
    Ponto3 lookat;
    Vetor3   vup;
    const double aspect_ratio = RATIO_16_9;
    const double viewport_height = 2.0;
    const double viewport_width = aspect_ratio * viewport_height;
    const double focal_length = 1.0;

    origem = Ponto3(0, 0, 0);
    horizontal = Vetor3(viewport_width, 0.0, 0.0);
    vertical = Vetor3(0.0, viewport_height, 0.0);
    canto_esq_inf = origem - horizontal/2 - vertical/2 - Vetor3(0, 0, focal_length);
}

Camera::Camera(Ponto3 lookfrom, Ponto3 lookat, Vetor3 vup, double aspect_ratio)
{
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(produto_vetorial(vup, w));
    auto v = produto_vetorial(w, u);
    origem = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    canto_esq_inf = origem - horizontal/2 - vertical/2 - w;
}


Camera::Camera(Ponto3 origem, Vetor3 canto_esq_inf, Vetor3 horizontal, Vetor3 vertical):
origem(origem),canto_esq_inf(canto_esq_inf),horizontal(horizontal),vertical(vertical)
{}

Raio Camera::get_raio(double u, double v) const
{
    return Raio(origem, canto_esq_inf + u*horizontal + v*vertical - origem);
}