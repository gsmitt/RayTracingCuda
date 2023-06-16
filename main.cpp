#include <iostream>
#include "imagem.h"
#include "camera.h"
#include "esfera.h"
#include <fstream>
#include <sstream>
#include "material.h"

#define CENA_ALEATORIA 1
using namespace std;

std::vector<std::shared_ptr<Forma>> mundo;
std::vector<std::unique_ptr<Ponto3>> luzes;
auto metal_rosa = make_shared<Metal>(Cor(1, 0.5, 0.5));
auto metal_preto = make_shared<Metal>(Cor(0.2, 0.2, 0.2));
auto difuso_laranja = make_shared<Lambertian>(Cor(1, 0.5, 0));
auto difuso_verde = make_shared<Lambertian>(Cor(0, 1, 0));
auto chao = make_shared<Lambertian>(Cor(0.807, 0.894, 0.815));
auto difuso_rosa = make_shared<Lambertian>(Cor(1, 0.5, 0.5));
auto luzfoda = make_shared<Luz>(Cor(1,1,1),Cor(1,1,1),100);



Cor cor_raio(Raio& r, int depth) 
{
    Cor luz = Cor(0,0,0);
    Cor contribuicao = Cor(1,1,1);

    for (int i = 0; i < depth; i++)
    {
        hit_record registro;
        hit_record registro_perto;
        bool intersecao_frente = false;
        double t_proximo = INFINITO;

        for (const auto& forma : mundo) 
        {
            if (forma->hit(r, 0.001, INFINITO, registro))
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
            Raio disperso;
            
            //luz += registro_perto.material->albedo;
            // cout << luz.a << luz.b << luz.c << "\n";
            luz = (luz + registro_perto.material->emitir())*contribuicao;
            registro_perto.material->dispersar(r,registro_perto,contribuicao,r);
            
                
        }
        else
        {
            //luz = Cor(0.2,0.2,0.2);
        }
    }
    return luz;
}

int main() {
    Imagem imagem(800);

    //Camera camera;
    // Camera camera(Ponto3(5,1.5,2.5), Ponto3(0,0,-0.5), Ponto3(0,1,0), RATIO_16_9);
    Camera camera(Ponto3(2,1.5,2.5), Ponto3(0,0,-0.5), Ponto3(0,1,0), RATIO_16_9);

    std::ofstream arquivo("imagem.ppm");

    hit_record registro;
    const int MSAA = 2000;

    srand(time(0));

#if CENA_ALEATORIA

    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(1,2,-3), 0.5, luzfoda));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-1,0,-1), 0.5, difuso_laranja));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-1,0,-2), 0.5, difuso_rosa));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(2,2,-3), 2, metal_rosa));
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-2,2,0), 0.5, luzfoda));


    mundo.emplace_back(std::make_shared<Esfera>(Ponto3( 0.0, -100.5, -1.0), 100, difuso_verde));
    // mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0,0.6,-1.5), 0.5, metal_rosa));
    // mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0.5,0.2,-0.5), 0.1, difuso_laranja));
    // mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-0.5,0.2,-0.5), 0.1, difuso_amarelo));

#else
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(1,2,-3), 0.5, luz));
    mundo.emplace_back(make_shared<Esfera>(Ponto3(0,-1000,0), 1000, chao));
    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto choose_mat = random_double();
            Ponto3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            if ((center - Ponto3(4, 0.2, 0)).modulo() > 0.9) {
                shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Ponto3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)) * Ponto3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    sphere_material = make_shared<Lambertian>(albedo);
                    mundo.emplace_back((make_shared<Esfera>(center, 0.2, sphere_material)));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Ponto3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    sphere_material = make_shared<Metal>(albedo);
                    mundo.emplace_back((make_shared<Esfera>(center, 0.2, sphere_material)));
                } else {
                    auto albedo = Ponto3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)) ;
                    sphere_material = make_shared<Metal>(albedo);
                    mundo.emplace_back((make_shared<Esfera>(center, 0.2, sphere_material)));
                }
            }
        }
    }
    auto material1 = make_shared<Metal>(Ponto3(0.7, 0.6, 0.5));
    mundo.emplace_back((make_shared<Esfera>(Ponto3(0, 1, 0), 1.0, material1)));
    auto material2 = make_shared<Lambertian>(Ponto3(0.4, 0.2, 0.1));
    mundo.emplace_back((make_shared<Esfera>(Ponto3(-4, 1, 0), 1.0, material2)));
    auto material3 = make_shared<Metal>(Ponto3(0.7, 0.6, 0.5));
    mundo.emplace_back((make_shared<Esfera>(Ponto3(4, 1, 0), 1.0, material3)));

#endif

    if (arquivo.is_open()) 
    {
        arquivo << "P6\n" << imagem.largura << ' ' << imagem.altura << "\n255\n";
        for (int i = imagem.altura - 1; i >= 0; i--) 
        {
            for (int j = 0; j < imagem.largura; j++) 
            {
                Cor corfinal(0,0,0);
                for (int k = 0; k < MSAA; k++)
                {
                    double u = (j + random_double()) / (imagem.largura-1);
                    double v = (i + random_double()) / (imagem.altura-1);
                    Raio r(camera.get_raio(u,v));
                    corfinal += cor_raio(r,3);
                }
                imagem.escreve_pixel(arquivo, corfinal, MSAA);
            }
        }
        arquivo.close();
        std::cout << "Arquivo PPM gerado com sucesso." << std::endl;
    } 
    else 
    {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }

    return 0;
}
