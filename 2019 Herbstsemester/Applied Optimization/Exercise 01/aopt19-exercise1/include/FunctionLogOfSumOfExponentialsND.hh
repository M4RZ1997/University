#pragma once

#include <iostream>
#include <cmath>

#include "FunctionBase.hh"


//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================

/**
 * @brief The FunctionLogOfSumOfExponentialsND class
 * f(x) = log ( \sum_{i=0}^{n-1} exp(x_i) ) for i = 0, ..., n-1
 */
class FunctionLogOfSumOfExponentialsND : public FunctionBase {
public:
    FunctionLogOfSumOfExponentialsND(const int _n)
        : n_(_n)
    {}

    inline virtual int n_unknowns() { return n_; }

    // funcion evaluation
    inline virtual double f(const Vec &_x) {
        //-------------------------------------------------------------------------------//
        //Todo: implement
        double sv = 0.0;
        for (int i = 0; i < n_; i++) {
            sv += std::exp(_x[i]);
        }
        return std::log(sv);
        // return 0.0
        //-------------------------------------------------------------------------------//
    }

    // gradient evaluation
    inline virtual void grad_f(const Vec &_x, Vec &_g) {
    }

    // hessian matrix evaluation
    inline virtual void hess_f(const Vec &_x, Mat &_H) {
    }

    // initial point for optimization
    inline virtual void initial_x(Vec &_x) {
    }

    // callback that is called after optimization finished
    inline virtual void handle_result(const Vec &_x) {
    }


private:
    int n_;
};

//== NAMESPACES ===============================================================
}
