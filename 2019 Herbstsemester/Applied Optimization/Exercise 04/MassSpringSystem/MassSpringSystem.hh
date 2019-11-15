#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/MatOp/SparseSymMatProd.h>

#include <MassSpringProblem2D.hh>
#include <MassSpringProblem2DSparse.hh>
#include <SpringGraph.hh>
#include <SpringElement2D.hh>
#include <RandomNumberGenerator.hh>
#include <DerivativeChecker.hh>

//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================

    class MassSpringSystem {
    public:
        using Point = Eigen::Vector2d;
        using Edge = std::pair<int, int>;
        using Vec = Eigen::VectorXd;
        using Mat = Eigen::MatrixXd;
        using SMat = Eigen::SparseMatrix<double>;

        MassSpringSystem(const int _n_grid_x, const int _n_grid_y) :
        n_grid_x_(_n_grid_x), n_grid_y_(_n_grid_y), rng_(-1., 1.) {
            setup_spring_graph();
        }

        ~MassSpringSystem(){}

        enum SpringElementType {WITHOUT_LENGTH, WITH_LENGTH};
        enum SparsityType {DENSE, SPARSE};


        double initial_system_energy(const int _spring_element_type, const int _sparsity_type);

        bool is_convex(const int _spring_element_type, const int _sparsity_type);

    private:
        void setup_spring_graph();

        int get_grid_index(const int _i, const int _j) const;

    private:
        int n_grid_x_;
        int n_grid_y_;

        SpringGraph sg_;
        RandomNumberGenerator rng_;
    };

//=============================================================================
}

