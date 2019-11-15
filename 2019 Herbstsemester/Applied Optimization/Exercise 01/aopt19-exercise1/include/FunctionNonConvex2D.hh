#pragma once

#include <iostream>
#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class FunctionNonConvex2D : public FunctionBase {
    public:
        // f(x,y) = (y-x^2)^2+cos^2(4*y)*(1-x)^2+x^2+y^2

        // constructor
        FunctionNonConvex2D() {}

        // number of unknowns
        inline virtual int n_unknowns() { return 2; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) {
            //------------------------------------------------------//
            //Todo: implement the function f(x,y) = (y-x^2)^2+cos^2(4*y)*(1-x)^2+x^2+y^y
            return std::pow(_x[1] - _x[0]*_x[0], 2) + std::pow(cos(4*_x[1]), 2) * (1-_x[0])*(1-_x[0]) + _x[0]*_x[0] + _x[1]*_x[1];
            //------------------------------------------------------//
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
            std::cerr << "solution converged to x = " << std::endl << _x << std::endl;
        }

    };

//=============================================================================
}


