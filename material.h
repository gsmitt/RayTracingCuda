#pragma once
#include "utils.h"
#include "raio.h"

struct hit_record;


class Material
{
public:
    virtual bool dispersar(
        const Raio& r_in, const hit_record& rec, Cor& attenuation, Raio& scattered
    ) const = 0;

};

