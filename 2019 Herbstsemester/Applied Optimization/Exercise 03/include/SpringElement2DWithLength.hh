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
            double length_squared = std::pow(_coeffs[1],2);
            return 0.5 * _coeffs[0] * std::pow((std::pow((_x[0]-_x[2]),2) + std::pow((_x[1]-_x[3]),2)-length_squared),2);
            //------------------------------------------------------//
            //return 0.;
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {}

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {
            //------------------------------------------------------//
            //Todo: implement the gradient and store in _g
            double  lengthSquared = std::pow(_coeffs[1],2);
             _g(0)=1/2*_coeffs[0]*(4*_x[0]-4*_x[2])*(std::pow(-_x[3]+_x[1],2)+std::pow(_x[0]-_x[2],2)-lengthSquared); //x1

             _g(1)=1/2*_coeffs[0]*(4*_x[1]-4*_x[3])*(std::pow(_x[0]-_x[2],2)+std::pow(_x[1]-_x[3],2)-lengthSquared); //y1

             _g(2)=1/2*_coeffs[0]*(4*_x[2]-4*_x[0])*(std::pow(_x[0]-_x[2],2)+std::pow(_x[1]-_x[3],2)-lengthSquared); //x2?

             _g(3)=1/2*_coeffs[0]*(4*_x[3]-4*_x[1])*(std::pow(_x[0]-_x[2],2)+std::pow(_x[1]-_x[3],2)-lengthSquared); //y2
            //------------------------------------------------------//
        }

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {
            //------------------------------------------------------//
            //Todo: implement the hessian matrix and store in _H
            //x1,y1,x2,y2 = x,y,z,g
            _H(0,0) = 0.5*_coeffs[0]*(2*_x[0]-2*_x[2])*(4*_x[0]-4*_x[2])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
            _H(0,1) = 0.5*_coeffs[0]*(2*_x[1]-2*_x[3])*(4*_x[0]-4*_x[2]);
            _H(0,2) = 0.5*_coeffs[0]*(2*_x[2]-2*_x[0])*(4*_x[0]-4*_x[2])-2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
            _H(0,3) = 0.5*_coeffs[0]*(2*_x[3]-2*_x[1])*(4*_x[0]-4*_x[2]);

            _H(1,0) = 0.5*_coeffs[0]*(2*_x[0]-2*_x[2])*(4*_x[1]-4*_x[3]);
            _H(1,1) = 0.5*_coeffs[0]*(4*_x[1]-4*_x[3])*(2*_x[1]-2*_x[3])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
            _H(1,2) = 0.5*_coeffs[0]*(2*_x[2]-2*_x[1])*(4*_x[1]-4*_x[3]);
            _H(1,3) = 0.5*_coeffs[0]*(4*_x[1]-4*_x[3])*(2*_x[3]-2*_x[1])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));

            _H(2,0) = 0.5*_coeffs[0]*(4*_x[2]-4*_x[0])*(2*_x[0]-2*_x[2])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
            _H(2,1) = 0.5*_coeffs[0]*(2*_x[1]-2*_x[3])*(4*_x[2]-4*_x[0]);
            _H(2,2) = 0.5*_coeffs[0]*(4*_x[2]-4*_x[0])*(2*_x[2]-2*_x[0])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
            _H(2,3) = 0.5*_coeffs[0]*(2*_x[3]-2*_x[1])*(4*_x[2]-4*_x[0]);

            _H(3,0) = 0.5*_coeffs[0]*(2*_x[0]-2*_x[2])*(4*_x[3]-4*_x[1]);
            _H(3,1) = 0.5*_coeffs[0]*(4*_x[3]-4*_x[1])*(2*_x[1]-2*_x[3])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
            _H(3,2) = 0.5*_coeffs[0]*(2*_x[2]-2*_x[0])*(4*_x[3]-4*_x[1]);
            _H(3,3) = 0.5*_coeffs[0]*(4*_x[3]-4*_x[1])*(2*_x[3]-2*_x[1])+2*_coeffs[0]*(std::pow(_x[1]-_x[3],2)+std::pow(_x[0]-_x[2],2)-std::pow(_coeffs[1],2));
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


