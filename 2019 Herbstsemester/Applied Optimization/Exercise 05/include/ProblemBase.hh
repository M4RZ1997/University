#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class ProblemBase {
    public:

        // (dense) vector type
        typedef Eigen::VectorXd Vec;
        // (dense) matrix type
        typedef Eigen::MatrixXd Mat;
        // sparse matrix type
        using SMat = Eigen::SparseMatrix<double>;
        // triplet
        using T = Eigen::Triplet<double>;

        // default constructor
        ProblemBase() {}

        // defualt destructor
        virtual ~ProblemBase() {};

        // number of unknowns
        virtual int n_unknowns() = 0;

        // funcion evaluation
        virtual double eval_f(const Vec &_x) = 0;

        // gradient evaluation
        virtual void eval_gradient(const Vec &_x, Vec &_g) = 0;

        // hessian matrix evaluation
        // dense
        virtual void eval_hessian(const Vec &_x, Mat &_H) = 0;
        //sparse
        virtual void eval_hessian(const Vec &_x, SMat& _h) = 0;

        // initial point for optimization
        virtual void initial_x(Vec &_x) = 0;

        // callback that is called after optimization finished
        virtual void handle_result(const Vec &_x) = 0;
    };


//=============================================================================
}




