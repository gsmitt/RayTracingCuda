#include <iostream>
#include "imagem.h"
#include "camera.h"
#include "esfera.h"
#include <fstream>

using namespace std;




int main() {
    Imagem imagem(800);

    Camera camera;

    std::ofstream arquivo("imagem.ppm");

    hit_record registro;

    srand(time(0));

    std::vector<std::shared_ptr<Forma>> mundo;

    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0,0,-1), 0.5, Cor(1,1,0)));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(0,1,-1), 0.5, Cor(1,1,1)));

    const int MSAA = 1;



    if (arquivo.is_open()) 
    {
        arquivo << "P6\n" << imagem.largura << ' ' << imagem.altura << "\n255\n";
        for (int i = imagem.altura - 1; i >= 0; i--) 
        {
            for (int j = 0; j < imagem.largura; j++) 
            {
                Cor corfinal = Cor(0,0,0);

                for (int k = 0; k < MSAA; k++)
                {

                    double u = (j + random_double()) / (imagem.largura-1);
                    double v = (i + random_double()) / (imagem.altura-1);
                    Raio r(camera.get_raio(u,v));
                    bool objeto_atingido = false;
                    double t_max = INFINITO;

                    for (const auto &esfera : mundo) {
                        if (esfera->hit(r,  0.001, t_max, registro)) {
                            objeto_atingido = true;
                            t_max = registro.t;
                            corfinal += registro.cor;
                        }
                    }

                    if (objeto_atingido) 
                    {
                        imagem.escreve_pixel(arquivo, corfinal, MSAA);
                    } 
                    else 
                    {
                        imagem.escreve_pixel(arquivo, corfinal, MSAA);
                    }
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
