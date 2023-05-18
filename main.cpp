#include "esfera.h"
#include "camera.h"
#include "utils.h"

using namespace std;

Camera cameraa;
std::vector<std::unique_ptr<Forma>> mundo;

Vetor3 reflexao_aleatoria() {
    while (true) {
        Ponto3 p = Vetor3(random_double(-1,1),random_double(-1,1),random_double(-1,1));
        if (p.prod_escalar(p) >= 1) continue;
        return p;
    }
}



Cor cor_pixel(Raio *r, int depth = 1)
{
    hit_record registro;
    hit_record registro_perto;
    bool intersecao_frente = false;
    double t_proximo = infinito;

    if (depth <= 0)
        return Cor(1,1,1);

    for (const auto& forma : mundo) 
    {
        if (forma->hit(r, 0.001, infinito, registro))
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
        Ponto3 alvo = registro_perto.p + registro_perto.normal + reflexao_aleatoria();
        Raio r_temp = Raio(registro_perto.p, alvo - registro_perto.p);
        return 0.9 * cor_pixel(&r_temp,(depth-1));

        // Mostrar os vetores normais por cores
        // return 0.5 * Cor(registro_perto.normal.a + 1,registro_perto.normal.b + 1,registro_perto.normal.c + 1);
    }
    
    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = r->direcao.unit_vector();
    //Definindo a cor do fundo
    double t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(0.2, 0, 0.7) + t*Cor(1,1,1);
}

void write_color(std::ostream &out, Vetor3 pixel_color, int amostras_por_pixel) 
{
    double r = pixel_color.a;
    double g = pixel_color.b;
    double b = pixel_color.c;

    double scale = 1.0 / amostras_por_pixel;

    r = sqrt(scale*r);
    g = sqrt(scale*g);
    b = sqrt(scale*b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

int main()
{
    // int quantidadebola = 100;

    // for (int i = 0; i < quantidadebola; i++)
    // {
    //     mundo.emplace_back(make_unique<Esfera>(Vetor3(random_double(-4,4), random_double(-3,3), random_double(-2,-10)),random_double(0.1,1)));
    // }
    

    
    mundo.emplace_back(make_unique<Esfera>(Ponto3(-0.7, 0, -1.2), 0.5));
    mundo.emplace_back(make_unique<Esfera>(Ponto3(0.7, 0, -0.9), 0.5));
    // mundo.emplace_back(make_unique<Esfera>(Vetor3(0, 0.9, -0.9), 0.5));

    mundo.emplace_back(make_unique<Esfera>(Ponto3(0,-100.5,-1), 100));


    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);


    const int amostra_por_pixel = 16;

    const int max_depth = 1000;

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
                cor_pixel_final += cor_pixel(&r, max_depth);
            }
            write_color(std::cout, cor_pixel_final, amostra_por_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}