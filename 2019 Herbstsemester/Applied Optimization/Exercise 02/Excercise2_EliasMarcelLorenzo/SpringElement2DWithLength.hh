#pragma once

#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {


//== CLASS DEFINITION =========================================================

    class SpringElement2DWithLength : public FunctionBase {
    public:
        // E'_ab(x) = 1/2 * k * (((x[0] - x[2])^2 + (x[1] - x[3])^2) - l^2)^2
        // constructor
        SpringElement2DWithLength() : FunctionBase() {}

        // number of unknowns
        inline virtual int n_unknowns() override { return 4; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) override {
            return 0.;
        }

        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            //------------------------------------------------------//
            //Todo: implement the function f(x) = 1/2 * k * (((x[0] - x[2])^2 + (x[1] - x[3])^2) - l^2)^2
            double length_squared = std::pow((_x[0]-_x[2]),2) + std::pow((_x[1]-_x[3]),2);
            return 0.5 * _coeffs[0] * std::pow((std::pow((_x[0]-_x[2]),2) + std::pow((_x[1]-_x[3]),2)-length_squared),2);
            //------------------------------------------------------//
            //return 0.;
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {}

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {
            //------------------------------------------------------//
            //Todo: implement the gradient and store in _g
             //1/2*k (((x-z)² + (y-h)²)-l²)²= 0 is this meant by you? Where l=(x1-x2)²+(y1-y2)²?
            // (ノಠ益ಠ)ノ彡┻━┻
             _g(0)=0;
             _g(1)=0;
             _g(2)=0;
             _g(3)=0;
            //------------------------------------------------------//
        }

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {
            //------------------------------------------------------//
            //Todo: implement the hessian matrix and store in _H
            // (ノಠ益ಠ)ノ彡┻━┻

            _H(0,0) = 0.0;
            _H(0,1) = 0.0;
            _H(0,2) = 0.0;
            _H(0,3) = 0.0;
            _H(1,0) = 0.0;
            _H(1,1) = 0.0;
            _H(1,2) = 0.0;
            _H(1,3) = 0.0;
            _H(2,0) = 0.0;
            _H(2,1) = 0.0;
            _H(2,2) = 0.0;
            _H(2,3) = 0.0;
            _H(3,0) = 0.0;
            _H(3,1) = 0.0;
            _H(3,2) = 0.0;
            _H(3,3) = 0.0;
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


