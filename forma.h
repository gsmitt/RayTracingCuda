#pragma once

#include "registro.h"
#include "vetor3.h"


/**
 * @brief Classe abstrata forma
 *
 */
class Forma
{
public:
    inline virtual bool hit(Raio &r, double t_min, double t_max, hit_record& registro) = 0;
};

