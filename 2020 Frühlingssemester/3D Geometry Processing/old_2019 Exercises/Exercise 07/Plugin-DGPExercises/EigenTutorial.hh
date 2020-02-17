#ifndef OPENFLIPPER_EIGENTUTORIAL_HH
#define OPENFLIPPER_EIGENTUTORIAL_HH

#include <vector>
#include <iostream>

#include "./eigen3/Eigen/Sparse"

//Add your code here
//Include a solver

class EigenTutorial {
public:
    using SMatd = Eigen::SparseMatrix<double>;
    using T = Eigen::Triplet<double>;
    using VecXd = Eigen::VectorXd;

    EigenTutorial(){}
    ~EigenTutorial(){}

public:
    void solve_sparse_linear_system();
};

#endif //OPENFLIPPER_EIGENTUTORIAL_HH
