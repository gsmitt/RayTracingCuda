#include <iostream>
#include "esfera.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

Esfera esfera_a(Vetor3(0,0,-2), 0.5);
Camera cameraa;


Vetor3 cor_pixel(Raio *r)
{
    hit_record registro;

    //Verifica onde o raio r encontra a esfera
    esfera_a.hit(r,-100,100,registro);
    double t = registro.t;
    // if (t > 0)
    // {
    //     //Vetor normal é o ponto de encontro menos o centro da esfera
    //     Vetor3 vetor_normal = r->at(t) - esfera_a.centro;
    //     //Transforma o normal em unitário, para colorir a esfera
    //     vetor_normal = vetor_normal.unit_vector();
    //     //Retorna uma cor dependendo do vetor normal, como é unitario entao esta entre -1 e 1.
    //     //Pega o absoluto, vetores que apontam pro mesmo z tem a mesma cor.
    //     return Cor(abs(vetor_normal.a),abs(vetor_normal.b),abs(vetor_normal.c));
    // }
    if (t > 0)
    {
        Vetor3 vetor_normal = registro.normal;
        vetor_normal = vetor_normal.unit_vector();
        return 0.5*Cor(vetor_normal.a+1,vetor_normal.b+1, vetor_normal.c+1);
    }
    
    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = r->direcao/r->direcao.modulo();
    //Definindo a cor do fundo
    t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(1.0, 1.0, 0.2) + t*Cor(0.5, 0.7, 1.0);
}

void write_color(std::ostream &out, Vetor3 pixel_color) 
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.a) << ' '
        << static_cast<int>(255.999 * pixel_color.b) << ' '
        << static_cast<int>(255.999 * pixel_color.c) << '\n';
}

int main()
{
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            double u = double(i) / (image_width-1);
            double v = double(j) / (image_height-1);
            Raio r(cameraa.get_raio(u,v));
            Vetor3 pixel_color = cor_pixel(&r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}