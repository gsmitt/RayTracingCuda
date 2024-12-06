#include <iostream>
#include "imagem.h"
#include "camera.h"
#include "esfera.h"
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <vector>
#include "material.h"

#define CENA_ALEATORIA 1
#define MSAA 100
#define THREADS 4

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

pthread_mutex_t mutex; // Mutex global para proteção de escrita

struct ThreadArgs {
    int start_row;
    int end_row;
    int largura;
    int altura;
};

Camera camera(Ponto3(2,1.5,2.5), Ponto3(0,0,-0.5), Ponto3(0,1,0), RATIO_16_9); // Global camera

Imagem imagem(800);

std::ofstream arquivo("imagem.ppm");

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
            luz = (luz + registro_perto.material->emitir()) * contribuicao;
            registro_perto.material->dispersar(r,registro_perto,contribuicao,r);
        }
    }
    return luz;
}

std::vector<std::vector<Cor>> resultados_threads(imagem.altura);
// Função que será executada por cada thread
void* processar_linhas(void* args) {
    // Converte o argumento de volta para o tipo correto
    ThreadArgs* arg = (ThreadArgs*)args;

    int start_row = arg->start_row;
    int end_row = arg->end_row;
    int largura = arg->largura;
    int altura = arg->altura;
    
    
    for (int i = start_row; i < end_row; i++) 
    {
        std::vector<Cor> lista_pixels;
        for (int j = 0; j < largura; j++) 
        {
            
            Cor corfinal(0,0,0);
            for (int k = 0; k < MSAA; k++)
            {
                double u = (j + random_double()) / (largura-1);
                double v = (i + random_double()) / (altura-1);
                Raio r(camera.get_raio(u,v));
                corfinal += cor_raio(r, 3);
            }

            lista_pixels.push_back(corfinal);
        }
        pthread_mutex_lock(&mutex);
            resultados_threads[i] = lista_pixels;
        pthread_mutex_unlock(&mutex);
    
    }
    
    return nullptr;
}

void processar_imagem() 
{
    pthread_t threads[THREADS]; // Número de threads a serem usadas

    int rows_per_thread = imagem.altura / THREADS; // Dividindo as linhas da imagem em 4 partes

    // Criar e iniciar threads
    for (int i = 0; i < THREADS; ++i) 
    {
        ThreadArgs* args = new ThreadArgs;
        args->start_row = i * rows_per_thread;
        if(i == THREADS-1){
            args->end_row = imagem.altura;
        } else {
            args->end_row = (i + 1) * rows_per_thread;
        }
        args->largura = imagem.largura;
        args->altura = imagem.altura;
        pthread_create(&threads[i], nullptr, processar_linhas, (void*)args);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }


    for (size_t i = resultados_threads.size(); i > 0; --i) {
    size_t idx = i - 1;
    for (size_t j = 0; j < resultados_threads[idx].size(); j++) {
        Cor& pixel = resultados_threads[idx][j]; 
        imagem.escreve_pixel(arquivo, pixel, MSAA);
    }
}

}

int main() {
    srand(time(0));

    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(1,2,-3), 0.5, luzfoda));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-1,0,-1), 0.5, difuso_laranja));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-1,0,-2), 0.5, difuso_rosa));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(2,2,-3), 2, metal_rosa));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3(-2,2,0), 0.5, luzfoda));
    mundo.emplace_back(std::make_shared<Esfera>(Ponto3( 0.0, -100.5, -1.0), 100, difuso_verde));

    if (arquivo.is_open()) 
    {
        arquivo << "P6\n" << imagem.largura << ' ' << imagem.altura << "\n255\n";
        
        // Call the new function to process the image
        processar_imagem();

        arquivo.close();
        std::cout << "Arquivo PPM gerado com sucesso." << std::endl;
    } 
    else 
    {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }

    return 0;
}
