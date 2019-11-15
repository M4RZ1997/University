#pragma once

#include <Eigen/Eigenvalues>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/MatOp/SparseSymMatProd.h>

#include <MassSpringProblem2D.hh>
#include <SpringGraph.hh>
#include <SpringElement2D.hh>
#include <SpringElement2DWithLength.hh>

#include <RandomNumberGenerator.hh>
#include <DerivativeChecker.hh>
#include <memory>

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

        MassSpringSystem(const int _n_grid_x, const int _n_grid_y, const int _spring_element_type) :
        n_grid_x_(_n_grid_x), n_grid_y_(_n_grid_y), n_unknowns_(0), rng_(-1., 1.) {
            setup_spring_graph();
            setup_problem(_spring_element_type);
        }

        ~MassSpringSystem(){}

        enum SpringElementType {WITHOUT_LENGTH, WITH_LENGTH};
        enum SparsityType {DENSE, SPARSE};


        double initial_system_energy();

        bool is_convex(const int _sparsity_type);

        std::shared_ptr<MassSpringProblem2D> get_problem() { return msp_; }

    private:
        void setup_problem(const int _spring_element_type);

        void setup_spring_graph();

        int get_grid_index(const int _i, const int _j) const;

    private:
        int n_grid_x_;
        int n_grid_y_;

        int n_unknowns_;

        SpringGraph sg_;
        RandomNumberGenerator rng_;

        SpringElement2D se_;
        SpringElement2DWithLength sewl_;
        std::shared_ptr<MassSpringProblem2D> msp_;
    };

//=============================================================================
}

