#pragma once

#include <iostream>
#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class FunctionQuadraticND : public FunctionBase {
    public:
        // f(x) = 1/2 x^T A x + b^T x + c  with x in R^n

        // constructor
        FunctionQuadraticND(const int _n)
                : FunctionBase(), n_(_n), A_(n_, n_), b_(n_), c_(0) {
            initialize_random_convex_problem();
        }

        FunctionQuadraticND(const Mat& _A, const Vec& _b, const double _c)
                : FunctionBase(), A_(_A), b_(_b), c_(_c) {
            if(A_.rows() != A_.cols())
                std::cerr << "Warning: matrix not square in FunctionQuadraticND" << std::endl;
            n_ = A_.rows();
        }

        // number of unknowns
        inline virtual int n_unknowns() override { return n_; }

        // funcion evaluation
        inline virtual double f(const Vec &_x) override {
            //-------------------------------------------------------------------------------//
            //Todo: implement the function 0.5*(_x.transpose()*A_*_x) + b_.transpose()*_x + c_
            double v = 0.5 * _x.transpose() * A_ * _x;
            v += b_.transpose() * _x + c_;
            return v;
            //-------------------------------------------------------------------------------//
        }

        inline virtual double f(const Vec &_x, const Vec &_coeffs) override {
            return 0.;
        }

        // gradient evaluation
        inline virtual void grad_f(const Vec &_x, Vec &_g) override {
            //-------------------------------------------------------------------------------//
            //Todo: implement the gradient of the function
            // Ay,ay capt'ain
             _g= A_*_x+b_;
            //-------------------------------------------------------------------------------//
        }

        inline virtual void grad_f(const Vec &_x, const Vec &_coeffs, Vec &_g) override {}

        // hessian matrix evaluation
        inline virtual void hess_f(const Vec &_x, Mat &_H) override {}

        inline virtual void hess_f(const Vec &_x, const Vec &_coeffs, Mat &_H) override {}

        // initial point for optimization
        inline virtual void initial_x(Vec &_x) override {}

        // callback that is called after optimization finished
        inline virtual void handle_result(const Vec &_x) override {}

    private:
        void initialize_random_convex_problem(double _max_val = 10.0, const int _random_index = 0) {
            std::cerr << "initialize random QP problem with " << n_ << " unknowns... " << std::endl;
            // create random matrix
            std::srand(_random_index);
            for (int i = 0; i < n_; ++i)
                for (int j = 0; j < n_; ++j)
                    A_(i, j) = _max_val * (2.0 * double(std::rand()) / double(RAND_MAX) - 1.0);

            // symmetrize
            A_ = A_.transpose() * A_;

            for (int i = 0; i < n_; ++i) {
                // random linear part
                b_[i] = _max_val * (2.0 * double(std::rand()) / double(RAND_MAX) - 1.0);

                // make A_ spd (smallest eigenvalue will be 1.0)
                A_(i, i) += 1.0;
            }

            c_ = 0.0;

            std::cerr << "done!" << std::endl;
        }

    private:

        // quadratic function 1/2 x^T A x + b^T x + c
        int n_;
        Mat A_;
        Vec b_;
        double c_;
    };

//=============================================================================
}




