#pragma once

#include <random>
#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

class RandomNumberGenerator
{
public:
    RandomNumberGenerator(double _vmin = -1000.0, double _vmax=1000.0)
        : vmin_{_vmin}, vmax_{_vmax}
    {
        // code adapted from en.cppreference.com: random uniform real distribution
        std::random_device rd;
        gen_ = std::mt19937(); //Standard mersenne_twister_engine can be seeded with rd()
        dis_ = std::uniform_real_distribution<>(vmin_, vmax_);
    }

    /**
     * @brief get_random_nd_vector generates a vector of n numbers sampled from a
     * uniform distribution in range [vmin, vmax)
     */
    FunctionBase::Vec get_random_nd_vector(int _n)
    {
        FunctionBase::Vec v(_n);
        for (int i = 0; i < _n; i++) {
            v[i] = dis_(gen_);
        }
        return v;
    }
private:
    double vmin_;
    double vmax_;

    std::mt19937 gen_;
    std::uniform_real_distribution<> dis_;
};

}
