#include <iostream>
#include "imagem.h"
#include "camera.h"
#include "esfera.h"
#include <fstream>
#include <sstream>
#include "material.h"


using namespace std;

std::vector<std::shared_ptr<Forma>> mundo;
std::vector<std::unique_ptr<Ponto3>> luzes;
auto metal_rosa = make_shared<Metal>(Cor(1, 0.5, 0.5));
auto difuso_amarelo = make_shared<Lambertian>(Cor(1, 1, 0));
auto metal_preto = make_shared<Metal>(Cor(0.2, 0.2, 0.2));
auto difuso_laranja = make_shared<Lambertian>(Cor(1, 0.5, 0));
auto difuso_verde = make_shared<Lambertian>(Cor(0, 1, 0));
auto chao = make_shared<Lambertian>(Cor(0.807, 0.894, 0.815));

Cor cor_raio(Raio& r, int depth) {
    hit_record registro;
    hit_record registro_perto;
    bool intersecao_frente = false;
    double t_proximo = INFINITO;

    if (depth <= 0)
        return Cor(0,0,0);

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
        Cor atenuada;
        if (registro_perto.material->dispersar(r,registro_perto, atenuada, disperso))
        {
            return atenuada * cor_raio(disperso, depth-1);
        }
        else
        {
            return Cor(0,0,0);
        }
    }
    
    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = unit_vector(r.direcao);
    //Definindo a cor do fundo
    double t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(1.0, 1.0, 1.0) + t*Cor(0.5, 0.7, 1.0);
}


Cor cor_pixel(Raio &r, int depth = 1)
{
    hit_record registro;
    hit_record registro_perto;
    bool intersecao_frente = false;
    double t_proximo = INFINITO;

    if (depth <= 0)
        return Cor(0,0,0);

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
        Ponto3 alvo = registro_perto.p + registro_perto.normal + unit_vector(reflexao_aleatoria());
        Raio r_temp = Raio(registro_perto.p, alvo - registro_perto.p);
        return registro_perto.cor * cor_pixel(r_temp,(depth-1));

        // Mostrar os vetores normais por cores
        // return 0.5 * Cor(registro_perto.normal.a + 1,registro_perto.normal.b + 1,registro_perto.normal.c + 1);
    }
    
    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = unit_vector(r.direcao);
    //Definindo a cor do fundo
    double t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(1.0, 1.0, 1.0) + t*Cor(0.5, 0.7, 1.0);
}


Cor cor_phon(Raio &r, int intensidade_luz, int coef_p)
{
    hit_record registro, rs;
    Cor corFinal = Cor(0,0,0);
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

            for (const auto& ponto : luzes)
            {
                Vetor3 l = *ponto - registro_perto.p;
                
                double distancia_ao_quadrado = l.modulo_quadrado();
                l = l / sqrt(distancia_ao_quadrado); // Normalizar o vetor l
                double intensidade_atenuada = intensidade_luz / (distancia_ao_quadrado);
                for (const auto& forma2 : mundo)
                {
                    Raio raioSombra = Raio(registro_perto.p, l);
                    if (forma2->hit(raioSombra, 0.00001, INFINITO, rs))
                    {
                    }
                    else
                    {
                        Vetor3 v = unit_vector(r.direcao);
                        Vetor3 h = v+l;
                        h = h/h.modulo();
                        double nl = produto_escalar(l,registro_perto.normal);
                        Cor corlamb;
                        if (nl > 0)
                        {
                            corlamb = (intensidade_atenuada*nl)*registro_perto.cor;
                        }
                        else
                        {
                            corlamb = Cor(0,0,0);
                        }
                        
                        //int coeficiente_spec;
                        double nh = produto_escalar(h,registro_perto.normal);
                        if(nh > 0)
                        {
                            corFinal += Cor((0.1*intensidade_atenuada*nh),(0.1*intensidade_atenuada*nh),(0.1*intensidade_atenuada*nh)) + corlamb;
                        }
                        else
                        {
                            corFinal += corlamb;
                        }
                    }
                }
            }
            return corFinal;
        }
    }

    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = unit_vector(r.direcao);
    //Definindo a cor do fundo
    double t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(1.0, 1.0, 1.0) + t*Cor(0.5, 0.7, 1.0);
}


int main() {
    Imagem imagem(1200);

    Camera camera;
    // Camera camera(Ponto3(5,1.5,2.5), Ponto3(0,0,-0.5), Ponto3(0,1,0), RATIO_16_9);

    std::ofstream arquivo("imagem.ppm");

    hit_record registro;
    //2 = sem material com ilumincao
    //3 = com material
    //4 = Cena aleatoria
    int MODO = 2;

    const int MSAA = 32;

    srand(time(0));


    if (MODO == 2)
    {
        std::ifstream arquivo_texto("cena.txt");
        if (arquivo_texto.is_open()) {
            std::string linha;
            while (std::getline(arquivo_texto, linha)) {
                if (linha == "ESFERA") {
                    // Adicionar esfera
                    Ponto3 centro;
                    double raio;
                    Cor cor;
                    if (!(std::getline(arquivo_texto, linha) &&
                            std::istringstream(linha) >> centro.a >> centro.b >> centro.c) ||
                        !(std::getline(arquivo_texto, linha) &&
                            std::istringstream(linha) >> raio >> cor.a >> cor.b >> cor.c)) {
                        std::cerr << "Erro ao ler dados da esfera." << std::endl;
                        break;
                    }
                    mundo.emplace_back(std::make_shared<Esfera>(centro, raio, cor));
                } else if (linha == "PONTO DE LUZ") {
                    // Adicionar ponto de luz
                    Ponto3 posicao;
                    if (!(std::getline(arquivo_texto, linha) &&
                            std::istringstream(linha) >> posicao.a >> posicao.b >> posicao.c)) {
                        std::cerr << "Erro ao ler dados do ponto de luz." << std::endl;
                        break;
                    }
                    luzes.emplace_back(std::make_unique<Ponto3>(posicao));
                }
            }
            arquivo_texto.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo de texto." << std::endl;
            return 1;
        }
    }




    if (MODO == 3)
    {
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3(1,0.5,-1), 0.5, difuso_amarelo));
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-1,0.6,-1), 0.5, difuso_laranja));
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3( 0.0, -100.5, -1.0), 100, metal_preto));
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0,0.6,-1.5), 0.5, metal_rosa));
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0.5,0.2,-0.5), 0.1, difuso_laranja));
        mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-0.5,0.2,-0.5), 0.1, difuso_amarelo));
    }

    if (MODO == 4)
    {
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
    }


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
                    if (MODO == 2)
                    {
                        corfinal +=cor_phon(r,10,1);
                    }
                    if (MODO == 3 || MODO==4)
                    {
                        corfinal += cor_raio(r,50);
                    }
                    
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
