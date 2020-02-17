#include "EigenTutorial.hh"

void EigenTutorial::solve_sparse_linear_system() {
    SMatd A;
    std::vector<T> tripletList;
    VecXd b;

    //This is a small sparse linear system Ax = b, where
    //A is | 2  3  0  0  0 |
    //     | 3  0  4  0  6 |
    //     | 0 -1 -3  2  0 |
    //     | 0  0  1  0  0 |
    //     | 0  4  2  0  1 |
    //and b is (8  20  13  6  17)T. Read the documentation of Eigen on sparse linear
    //algebra (http://eigen.tuxfamily.org/dox-3.2/group__Sparse__chapter.html).
    //Solve the system and output the value of A, x and b.

    //add your code here
    A.resize(5, 5);

    tripletList.push_back(T(0, 0, 2.0));
    tripletList.push_back(T(0, 1, 3.0));
    tripletList.push_back(T(1, 0, 3.0));
    tripletList.push_back(T(1, 2, 4.0));
    tripletList.push_back(T(1, 4, 6.0));
    tripletList.push_back(T(2, 1, -1.0));
    tripletList.push_back(T(2, 2, -3.0));
    tripletList.push_back(T(2, 3, 2.0));
    tripletList.push_back(T(3, 2, 1.0));
    tripletList.push_back(T(4, 1, 4.0));
    tripletList.push_back(T(4, 2, 2.0));
    tripletList.push_back(T(4, 4, 1.0));
    A.setFromTriplets(tripletList.begin(), tripletList.end());

    b.resize(5);
    b << 8, 45, -3, 3, 19;

    VecXd x;

    Eigen::SparseLU<SMatd> solver;
    solver.analyzePattern(A);
    solver.factorize(A);

    x = solver.solve(b);

    std::cout<<"\nLinear system Ax = b. \nA is:\n"<<A<<std::endl;
    std::cout<<"b is:\n"<<b<<std::endl;
    std::cout<<"\nx is:\n"<<x<<std::endl;
    if(A*x == b)
        std::cout<<"\nThe solution is correct!"<<std::endl;
    else
        std::cout<<"\nError: wrong solution!"<<std::endl;
}