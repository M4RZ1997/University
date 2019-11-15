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

        // k and l are stored in _coeffs, i.e._coeffs[0] is k, _coeffs[1] is l
        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            //------------------------------------------------------//
            //Todo: implement the function f(x) = 1/2 * k * (((x[0] - x[2])^2 + (x[1] - x[3])^2) - l^2)^2
            double dx = _x[0] - _x[2];
            double dy = _x[1] - _x[3];

            return 0.5 * _coeffs[0] * std::pow((dx*dx + dy*dy) - _coeffs[1]*_coeffs[1], 2);
            //------------------------------------------------------//
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {}

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {
            //------------------------------------------------------//
            //Todo: implement the gradient and store in _g (0.3 pt)
            double part = _coeffs[0]*(std::pow((_x[0]-_x[2]),2) + std::pow((_x[1]-_x[3]),2) - _coeffs[1]*_coeffs[1]);
            _g[0] = part * 2*(_x[0] - _x[2]);
            _g[1] = part * 2*(_x[1] - _x[3]);
            _g[2] = part * 2*(_x[2] - _x[0]);
            _g[3] = part * 2*(_x[3] - _x[1]);
            //------------------------------------------------------//
        }

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {
            //------------------------------------------------------//
            //Todo: implement the hessian matrix and store in _H (0.4 pt)
//            _H << _coeffs[0]*std::pow((2*_x[0] - 2*_x[2]), 2) + 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]), _coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]), - _coeffs[0]*std::pow((2*_x[0] - 2*_x[2]), 2) - 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]), -_coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]),
//                  _coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]),   _coeffs[0]*std::pow((2*_x[1] - 2*_x[3]), 2) + 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]), -_coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]), - _coeffs[0]*std::pow((2*_x[1] - 2*_x[3]), 2) - 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]),
//                  - _coeffs[0]*std::pow((2*_x[0] - 2*_x[2]), 2) - 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]), -_coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]),   _coeffs[0]*std::pow((2*_x[0] - 2*_x[2]), 2) + 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]), _coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]),
//                  -_coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]), - _coeffs[0]*std::pow((2*_x[1] - 2*_x[3]), 2) - 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]), _coeffs[0]*(2*_x[0] - 2*_x[2])*(2*_x[1] - 2*_x[3]), _coeffs[0]*std::pow((2*_x[1] - 2*_x[3]), 2) + 2*_coeffs[0]*(std::pow((_x[0] - _x[2]), 2) + std::pow((_x[1] - _x[3]), 2) - _coeffs[1]*_coeffs[1]);

            _H(0,0) = _coeffs[0]*(-2.0*std::pow(_coeffs[1], 2) + 6.0*std::pow(_x[0] - _x[2], 2) + 2.0*std::pow(_x[1] - _x[3], 2));
            _H(0,1) = 4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(0,2) = _coeffs[0]*(2.0*std::pow(_coeffs[1], 2) - 6.0*std::pow(_x[0] - _x[2], 2) - 2.0*std::pow(_x[1] - _x[3], 2));
            _H(0,3) = -4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(1,0) = 4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(1,1) = _coeffs[0]*(-2.0*std::pow(_coeffs[1], 2) + 2.0*std::pow(_x[0] - _x[2], 2) + 6.0*std::pow(_x[1] - _x[3], 2));
            _H(1,2) = -4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(1,3) = _coeffs[0]*(2.0*std::pow(_coeffs[1], 2) - 2.0*std::pow(_x[0] - _x[2], 2) - 6.0*std::pow(_x[1] - _x[3], 2));
            _H(2,0) = _coeffs[0]*(2.0*std::pow(_coeffs[1], 2) - 6.0*std::pow(_x[0] - _x[2], 2) - 2.0*std::pow(_x[1] - _x[3], 2));
            _H(2,1) = -4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(2,2) = _coeffs[0]*(-2.0*std::pow(_coeffs[1], 2) + 6.0*std::pow(_x[0] - _x[2], 2) + 2.0*std::pow(_x[1] - _x[3], 2));
            _H(2,3) = 4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(3,0) = -4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(3,1) = _coeffs[0]*(2.0*std::pow(_coeffs[1], 2) - 2.0*std::pow(_x[0] - _x[2], 2) - 6.0*std::pow(_x[1] - _x[3], 2));
            _H(3,2) = 4.0*_coeffs[0]*(_x[0] - _x[2])*(_x[1] - _x[3]);
            _H(3,3) = _coeffs[0]*(-2.0*std::pow(_coeffs[1], 2) + 2.0*std::pow(_x[0] - _x[2], 2) + 6.0*std::pow(_x[1] - _x[3], 2));
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


