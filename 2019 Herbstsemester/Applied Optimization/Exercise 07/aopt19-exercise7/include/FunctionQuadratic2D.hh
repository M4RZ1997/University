#pragma once

#include <iostream>
#include "FunctionBase.hh"

//== NAMESPACES ===================================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class FunctionQuadratic2D : public FunctionBase {
    public:
        // f(x,y) = 0.5(x^2 + gamma *y^2)

        // constructor
        FunctionQuadratic2D(const double _gamma = 1)
                : FunctionBase(), gamma_(_gamma) {}

        // number of unknowns
        inline virtual int n_unknowns() override { return 2; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) override {
            //------------------------------------------------------//
            //Todo: implement the function f(x,y) = 0.5(x^2 + gamma *y^2)
            return 0.5 * (_x[0] * _x[0] + gamma_ * _x[1] * _x[1]);
            //------------------------------------------------------//
        }

        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            return 0.;
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {
            _g[0] = _x[0];
            _g[1] = gamma_ * _x[1];
        }

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {}

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {
            _H = Mat(n_unknowns(), n_unknowns());
            _H << 1.0, 0.0, 0.0, gamma_;
        }

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {}

        // initial point for optimization
        inline virtual void initial_x(Vec &_x) override {}

        // callback that is called after optimization finished
        inline virtual void handle_result(const Vec &_x) override {
            std::cerr << "solution converged to x = " << std::endl << _x << std::endl;
        }
        double get_gamma() const { return gamma_; }
    private:
        double gamma_;
    };

//=============================================================================
}
