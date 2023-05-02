#include <iostream>
#include "raio.h"
#include <stdio.h>

using namespace std;

Esfera esfera_a(Vetor3(0,0,-5), 0.5);



Vetor3 cor_pixel(Raio *r)
{
    if (r->hit_esfera( &esfera_a)) return Vetor3(1,0,0);
    Vetor3 unit_direction = r->direcao/r->direcao.modulo();
    double t = 0.5*(unit_direction.b + 1.0);
    return Vetor3(((1.0-t)*1.0)+0.5*t, ((1.0-t)*1.0)+0.7*t, ((1.0-t)*1.0)+1.0*t);
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
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    Vetor3 origin = Vetor3(0, 0, 0);
    Vetor3 horizontal = Vetor3(viewport_width, 0, 0);
    Vetor3 vertical = Vetor3(0, viewport_height, 0);
    Vetor3 lower_left_corner = origin - horizontal/2 - vertical/2 - Vetor3(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            double u = double(i) / (image_width-1);
            double v = double(j) / (image_height-1);
            Raio r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            Vetor3 pixel_color = cor_pixel(&r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}