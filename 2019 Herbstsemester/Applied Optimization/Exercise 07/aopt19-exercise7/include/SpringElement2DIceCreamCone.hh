#pragma once

#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================

    class SpringElement2DIceCreamCone : public FunctionBase {
    public:
        // E_ab(x) = 1/2 * k * ((x[0] - x[2])^2 + (x[1] - x[3])^2)
        // E_ab(x) = k *( sin(sqrt(x[0]^2+x[1]^2)+2atan(x[1]/(sqrt(x[0]^2+x[1]^2)+x[0]+exp(-100*(x[0]^2+x[1]^2))))) + log(1+x[0]^2+x[1]^2)
        //         - (sin(sqrt(x[2]^2+x[3]^2)+2atan(x[2]/(sqrt(x[2]^2+x[3]^2)+x[2]+exp(-100*(x[2]^2+x[3]^2))))) + log(1+x[2]^2+x[3]^2)))

        // constructor
        SpringElement2DIceCreamCone() : FunctionBase() {}

        // number of unknowns
        inline virtual int n_unknowns() override { return 2; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) override {
            return sin(sqrt(_x[0]*_x[0]+_x[1]*_x[1])+2*atan(_x[1]/(sqrt(_x[0]*_x[0]+_x[1]*_x[1])+_x[0]+exp(-100*(_x[0]*_x[0]+_x[1]*_x[1]))))) + log(1+_x[0]*_x[0]+_x[1]*_x[1]);
        }

        // k and l are stored in _coeffs, i.e._coeffs[0] is k, _coeffs[1] is l
        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            //------------------------------------------------------//
            //Todo: implement the function f(x) = 1/2 * k * ((x[0] - x[2])^2 + (x[1] - x[3])^2)
            return 0;
            //------------------------------------------------------//
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {
            double sigma_0 = pow(_x[0], 2);
            double sigma_1 = pow(_x[1], 2);
            double sigma_2 = sigma_0 + sigma_1;
            double sigma_3 = 2/(sigma_2 + 1);
            double sigma_4 = sqrt(sigma_2);
            double sigma_5 = exp(-100*sigma_0 - 100*sigma_1);
            double sigma_6 = sigma_4 + sigma_5 + _x[0];
            double sigma_7 = 1.0/sigma_6;
            double sigma_8 = cos(sigma_4 + 2*atan(sigma_7*_x[1]));
            double sigma_9 = 1.0/sigma_4;
            double sigma_10 = sigma_9*_x[0];
            double sigma_11 = 200*sigma_5;
            double sigma_12 = pow(sigma_6, -2);
            double sigma_13 = sigma_12*_x[1];
            double sigma_14 = 2/(sigma_1*sigma_12 + 1);
            double sigma_15 = sigma_9*_x[1];

            _g[0] = sigma_3*_x[0] + sigma_8*(sigma_10 + sigma_13*sigma_14*(-sigma_10 + sigma_11*_x[0] - 1));
            _g[1] = sigma_3*_x[1] + sigma_8*(sigma_14*(sigma_13*(sigma_11*_x[1] - sigma_15) + sigma_7) + sigma_15);
        }

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {
            //------------------------------------------------------//
            //Todo: implement the gradient and store in _g

            //------------------------------------------------------//
        }

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {
            //------------------------------------------------------//
            //Todo: implement the hessian matrix and store in _H
            _H << _coeffs[0], 0, -_coeffs[0], 0,
                    0, _coeffs[0], 0, -_coeffs[0],
                    -_coeffs[0], 0, _coeffs[0], 0,
                    0, -_coeffs[0], 0, _coeffs[0];
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


