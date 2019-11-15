#pragma once

#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class NodeConstraintElement : public FunctionBase {
    public:
        NodeConstraintElement(){}
        ~NodeConstraintElement(){}

        // number of unknowns
        inline virtual int n_unknowns() override { return 2; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) override {
            return 0.;
        }

        // k and l are stored in _coeffs, i.e._coeffs[0] is penalty, _coeffs[1] and _coeffs[2] are desired point coordinates
        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            //------------------------------------------------------//
            //Todo: implement the function f(x) = 1/2 * penalty * ((x[0] - px)^2 + (x[1] - py)^2)
            //Done I thinks.
            return 1/2 * _coeffs[0] * (std::pow((_x[0]-_coeffs[1]),2) + std::pow((_x[1]-_coeffs[2]),2));
            return 0.;
            //------------------------------------------------------//
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {}

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {
            //------------------------------------------------------//
            //Todo: implement the gradient and store in _g
            //DONE I thinks.
            double part = _coeffs[0] * (std::pow((_x[0]-_coeffs[1]),2) + std::pow((_x[1]-_coeffs[2]),2));
            _g[0] = part * 2 * (_x[0] - _coeffs[1]);
            _g[1] = part * 2 * (_x[1] - _coeffs[2]);
            //------------------------------------------------------//
        }

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {}

        // initial point for optimization
        inline virtual void initial_x(Vec &_x) override {}

        // callback that is called after optimization finished
        inline virtual void handle_result(const Vec &_x) override {
            std::cout << "solution converged to x = " << std::endl << _x << std::endl;
        }
    };

    //=============================================================================
}





