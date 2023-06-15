#pragma once
#include "registro.h"
class Material
{
public:
    Cor albedo;
    float poder_emissao;
    Cor cor_emissao;
    virtual bool dispersar(const Raio &raio, const hit_record& registro, Cor &atenuada, Raio &disperso) const = 0;
    virtual Cor emitir() const {return cor_emissao*poder_emissao;};
};



class Lambertian : public Material {
    public:
        Lambertian(const Cor& a, float poder_emissao = 0, Cor cor_emissao = Cor(0,0,0)) : albedo(a), poder_emissao(poder_emissao), cor_emissao(cor_emissao) {}

        virtual Cor emitir() const override {return cor_emissao*poder_emissao;};

        virtual bool dispersar(const Raio& r_in, const hit_record& registro, Cor& atenuada, Raio& disperso) const override 
        {
            Vetor3 scatter_direction = registro.normal + random_unit_vector();
            disperso = Raio(registro.p, scatter_direction);
            atenuada = atenuada*albedo;
            return true;
        }

    public:
        Cor albedo;
        float poder_emissao;
        Cor cor_emissao;
};

class Metal : public Material {
    public:
        Metal(const Cor& a) : albedo(a) {}

        virtual bool dispersar(
            const Raio& r_in, const hit_record& registro, Cor& atenuada, Raio& disperso
        ) const override {
            Vetor3 reflected = refletir(unit_vector(r_in.direcao), registro.normal);
            disperso = Raio(registro.p, reflected);
            atenuada = albedo;
            return (produto_escalar(disperso.direcao, registro.normal) > 0);
        }

    public:
        Cor albedo;
};