#include "esfera.h"
#include "camera.h"
#include "utils.h"

using namespace std;

Camera cameraa;
std::vector<std::unique_ptr<Forma>> mundo;



Vetor3 cor_pixel(Raio *r)
{
    hit_record registro;
    hit_record registro_perto;
    bool intersecao_frente = false;
    double t_proximo = infinito;

    for (const auto& forma : mundo) 
    {
        if (forma->hit(r,0,infinito,registro))
        {
            if (registro.t < t_proximo)
            {
                intersecao_frente = true;
                t_proximo = registro.t;
                registro_perto = registro;
            }
        }
    }

    if (intersecao_frente)
    {   
        return 0.5*Cor(registro.normal + Vetor3(1,1,1));
    }
    
    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = r->direcao.unit_vector();
    //Definindo a cor do fundo
    double t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(1.0, 1.0, 0.2) + t*Cor(0.5, 0.7, 1.0);
}

void write_color(std::ostream &out, Vetor3 pixel_color, int amostras_por_pixel) 
{
    double r = pixel_color.a;
    double g = pixel_color.b;
    double b = pixel_color.c;

    double scale = 1.0 / amostras_por_pixel;

    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

int main()
{
    mundo.emplace_back(make_unique<Esfera>(Vetor3(0, 0, -0.9), 0.5));
    mundo.emplace_back(make_unique<Esfera>(Vetor3(0,-100.5,-1), 100));



    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int amostra_por_pixel = 10;

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (double j = image_height-1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (double i = 0; i < image_width; ++i) 
        {
            Cor cor_pixel_final(0,0,0);
            for (double k = 0; k < amostra_por_pixel; k++)
            {
                double u = (i + random_double()) / (image_width-1);
                double v = (j + random_double()) / (image_height-1);
                Raio r(cameraa.get_raio(u,v));
                cor_pixel_final += cor_pixel(&r);
            }
            write_color(std::cout, cor_pixel_final, amostra_por_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}