#include <iostream>
#include "imagem.h"
#include "camera.h"
#include "esfera.h"
#include <fstream>
#include <sstream>


using namespace std;

std::vector<std::shared_ptr<Forma>> mundo;


Cor cor_pixel(Raio &r, int depth = 1)
{
    hit_record registro;
    hit_record registro_perto;
    bool intersecao_frente = false;
    double t_proximo = INFINITO;

    if (depth <= 0)
        return Cor(1,1,1);

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
        Ponto3 alvo = registro_perto.p + registro_perto.normal + reflexao_aleatoria();
        Raio r_temp = Raio(registro_perto.p, alvo - registro_perto.p);
        return 0.5 * cor_pixel(r_temp,(depth-1));

        // Mostrar os vetores normais por cores
        // return 0.5 * Cor(registro_perto.normal.a + 1,registro_perto.normal.b + 1,registro_perto.normal.c + 1);
    }
    
    //Define o vetor unitario do raio dependendo de qual parte do espaço ele esta sendo atirado
    Vetor3 unit_direction = unit_vector(r.direcao);
    //Definindo a cor do fundo
    double t = 0.5*(unit_direction.b + 1.0);
    return (1.0-t)*Cor(0.2, 0, 0.7) + t*Cor(1,1,1);
}



int main() {
    Imagem imagem(800);

    Camera camera;

    std::ofstream arquivo("imagem.ppm");

    hit_record registro;

    srand(time(0));

    std::ifstream arquivo_texto("cena.txt");
    if (arquivo_texto.is_open()) {
        std::string linha;
        while (std::getline(arquivo_texto, linha)) {
            std::istringstream iss(linha);
            Ponto3 centro;
            double raio;
            Cor cor;
            if (!(iss >> centro.a >> centro.b >> centro.c >> raio >> cor.a >> cor.b >> cor.c)) {
                std::cerr << "Erro ao ler linha do arquivo: " << linha << std::endl;
                continue;
            }
            mundo.emplace_back(std::make_shared<Esfera>(centro, raio, cor));
        }
        arquivo_texto.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo de texto." << std::endl;
        return 1;
    }


    // mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0,0,-1), 0.5, Cor(1,1,0)));
    // mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0,1,-1), 0.5, Cor(1,1,1)));

    const int MSAA = 1;



    if (arquivo.is_open()) 
    {
        arquivo << "P6\n" << imagem.largura << ' ' << imagem.altura << "\n255\n";
        for (int i = imagem.altura - 1; i >= 0; i--) 
        {
            for (int j = 0; j < imagem.largura; j++) 
            {
                Cor corfinal;

                for (int k = 0; k < MSAA; k++)
                {

                    double u = (j + random_double()) / (imagem.largura-1);
                    double v = (i + random_double()) / (imagem.altura-1);
                    Raio r(camera.get_raio(u,v));
                    corfinal += cor_pixel(r,100);
                    imagem.escreve_pixel(arquivo, corfinal, 1);
                }
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
