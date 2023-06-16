#pragma once
#include "registro.h"
class Material
{
public:
    Cor albedo;
    double poder_emissao;
    Cor cor_emissao;
    virtual bool dispersar(const Raio &raio, const hit_record& registro, Cor &atenuada, Raio &disperso) const = 0;
    virtual Cor emitir() const {return Cor(0,0,0);};
};



class Lambertian : public Material {
    public:
        Lambertian(const Cor& a) : albedo(a){}

        virtual Cor emitir() const override {return cor_emissao*poder_emissao;};

        virtual bool dispersar(const Raio& r_in, const hit_record& registro, Cor& atenuada, Raio& disperso) const override 
        {
            Vetor3 scatter_direction = registro.normal + random_unit_vector();
            disperso = Raio(registro.p, scatter_direction);
            atenuada = albedo;
            return true;
        }

    public:
        Cor albedo;
        double poder_emissao;
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

class Luz : public Material
{
    public:
        Luz(const Cor& albedo, const Cor& cor_emissao, double poder_emissao)
        {
            this->albedo = albedo;
            this->cor_emissao = cor_emissao;
            this->poder_emissao = poder_emissao;
        }
        
        virtual bool dispersar(const Raio& r_in, const hit_record& registro, Cor& atenuada, Raio& disperso) const override 
        {
            return true;
        }

        virtual Cor emitir() const override {return cor_emissao*poder_emissao;};

};