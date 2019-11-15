#pragma once

#include <Eigen/Dense>

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class FunctionBase {
    public:

        // (dense) vector type
        typedef Eigen::VectorXd Vec;
        // (dense) matrix type
        typedef Eigen::MatrixXd Mat;

        // default constructor
        FunctionBase() {}

        // defualt destructor
        virtual ~FunctionBase() {};

        // number of unknowns
        virtual int n_unknowns() = 0;

        // funcion evaluation
        virtual double f(const Vec &_x) = 0;

        // gradient evaluation
        virtual void grad_f(const Vec &_x, Vec &_g) = 0;

        // hessian matrix evaluation
        virtual void hess_f(const Vec &_x, Mat &_H) = 0;

        // initial point for optimization
        virtual void initial_x(Vec &_x) = 0;

        // callback that is called after optimization finished
        virtual void handle_result(const Vec &_x) = 0;

    };


//=============================================================================
}

