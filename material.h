#pragma once
#include "registro.h"
class Material
{
public:
    virtual bool dispersar(const Raio &raio, const hit_record& registro, Cor &atenuada, Raio &disperso) const = 0;
};



class Lambertian : public Material {
    public:
        Lambertian(const Cor& a) : albedo(a) {}

        virtual bool dispersar(
            const Raio& r_in, const hit_record& registro, Cor& atenuada, Raio& disperso
        ) const override {
            Vetor3 scatter_direction = registro.normal + random_unit_vector();
            disperso = Raio(registro.p, scatter_direction);
            atenuada = albedo;
            return true;
        }

    public:
        Cor albedo;
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