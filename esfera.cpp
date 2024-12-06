#include "esfera.h"

Esfera::Esfera(Ponto3 centro, double raio, Cor cor):centro(centro),raio(raio),cor(cor){}

//Construtor com mateiral
Esfera::Esfera(Ponto3 centro, double raio, shared_ptr<Material> m):centro(centro),raio(raio),mat_ptr(m){}

bool Esfera::hit(Raio &r, double t_min, double t_max, hit_record &registro) {
    Vetor3 d_o = r.origem - centro;
    double a = r.direcao.modulo_quadrado();
    double b = produto_escalar(d_o, r.direcao);
    double c = d_o.modulo_quadrado() - (raio * raio);
    double delta = b * b - a * c;

    // Se o delta for negativo, não há interseção
    if (delta < 0) return false;

    // Calcule as duas raízes possíveis
    double raiz_delta = sqrt(delta);
    double t1 = (-b - raiz_delta) / a;
    double t2 = (-b + raiz_delta) / a;

    // Escolha a raiz mais próxima no intervalo válido
    double t = (t1 >= t_min && t1 <= t_max) ? t1 : t2;
    if (t < t_min || t > t_max) {
        return false;
    }

    // Preenche o registro de colisão
    registro.t = t;
    registro.p = r.at(t);
    Vetor3 fora = (registro.p - centro) / raio;
    registro.normal_face(r, fora);
    registro.cor = cor;
    registro.material = mat_ptr;

    return true;
}