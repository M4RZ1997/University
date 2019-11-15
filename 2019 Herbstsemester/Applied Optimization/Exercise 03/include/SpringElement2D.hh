#pragma once

#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================

    class SpringElement2D : public FunctionBase {
    public:
        // E_ab(x) = 1/2 * k * ((x[0] - x[2])^2 + (x[1] - x[3])^2)
        // constructor
        SpringElement2D() : FunctionBase() {}

        // number of unknowns
        inline virtual int n_unknowns() override { return 4; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) override {
            return 0.;
        }

        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            //------------------------------------------------------//
            //Todo: implement the function f(x) = 1/2 * k * ((x[0] - x[2])^2 + (x[1] - x[3])^2)

            // First k then l in coeff
            return 0.5 * _coeffs[0] * (std::pow((_x[0] - _x[2]),2) + std::pow((_x[1]-_x[3]),2));

            //------------------------------------------------------//
            //return 0.;
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {}

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {
            //------------------------------------------------------//
            //Todo: implement the gradient and store in _g
            _g[0] = _coeffs[0] * (_x[0] - _x[2]); //x1
            _g[1] = _coeffs[0] * (_x[1] - _x[3]); // y1
            _g[2] = _coeffs[0] * (_x[2] - _x[0]); // x2
            _g[3] = _coeffs[0] * (_x[3] - _x[1]); //y2
            //------------------------------------------------------//
        }

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {
            //------------------------------------------------------//
            //Todo: implement the hessian matrix and store in _H
            _H(0,0) = _coeffs[0];
            _H(0,1) = 0.0;
            _H(0,2) = -_coeffs[0];
            _H(0,3) = 0.0;

            _H(1,0) = 0.0;
            _H(1,1) = _coeffs[0];
            _H(1,2) = 0.0;
            _H(1,3) = -_coeffs[0];

            _H(2,0) = -_coeffs[0];
            _H(2,1) = 0.0;
            _H(2,2) = _coeffs[0];
            _H(2,3) = 0.0;

            _H(3,0) = 0.0;
            _H(3,1) = -_coeffs[0];
            _H(3,2) = 0.0;
            _H(3,3) = _coeffs[0];

            //------------------------------------------------------//
        }

        // initial point for optimization
        inline virtual void initial_x(Vec &_x) override {}

        // callback that is called after optimization finished
        inline virtual void handle_result(const Vec &_x) override {
            std::cout << "solution converged to x = " << std::endl << _x << std::endl;
        }
    };

//=============================================================================
}


