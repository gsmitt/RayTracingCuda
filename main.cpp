#include <iostream>
#include "raio.h"
#include "camera.h"
#include <stdio.h>

using namespace std;

Esfera esfera_a(Vetor3(0,0,-2), 0.5);
Camera cameraa;


Vetor3 cor_pixel(Raio *r)
{
    double t = r->hit_esfera(&esfera_a);
    if (t > 0)
    {
        Vetor3 n = r->at(t)- Vetor3(0,0,-1);
        n = n.unit_vector();
        return 0.5*Vetor3(n.a+1,n.b+1,n.c+1);
    }
    Vetor3 unit_direction = r->direcao/r->direcao.modulo();
    t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Vetor3(1.0, 1.0, 1.0) + t*Vetor3(0.5, 0.7, 1.0);
    // if (r->hit_esfera( &esfera_a)) return Vetor3(0.5,0.5,0.5);
    // Vetor3 unit_direction = r->direcao/r->direcao.modulo();
    // double t = 0.5*(unit_direction.b + 1.0);
    // return Vetor3(((1.0-t)*2.0)+0.5*t, ((1.0-t)*1.0)+0.7*t, ((1.0-t)*1.0)+1.0*t);
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
    const int image_width = 3840;
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