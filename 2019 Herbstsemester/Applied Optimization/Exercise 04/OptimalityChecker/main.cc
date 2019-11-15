#include "OptimalityChecker.hh"
#include <FunctionQuadraticND.hh>
#include <vector>
#include <iostream>


int main(int _argc, const char* _argv[]) {
    AOPT::FunctionQuadraticND::Mat A(2, 2);
    A.setZero();
    AOPT::FunctionQuadraticND::Vec b(2);
    b.setZero();
    //set up the optimization problem
    //-------------------------------------------------------------------------------//
    //Todo: set up the optimization problem
    //1. set objective function
    A(0,0)=2.0;
    A(1,1)=-4.0;

    AOPT::FunctionQuadraticND obj_func(A, b, 0);


    //2. inequality constraints
    std::vector<AOPT::FunctionBase*> ineq_cons;

    AOPT::FunctionQuadraticND::Mat AFirst(2, 2);
    AFirst.setZero();
    AOPT::FunctionQuadraticND::Vec bFirst(2);
    bFirst.setZero();
    AFirst(0,0)=2.0;
    AFirst(1,1)=2.0;
    bFirst(0)=8.0;
    AOPT::FunctionQuadraticND inequalFirst(AFirst, bFirst, 14.0);

    AOPT::FunctionQuadraticND::Mat ASecond(2, 2);
    ASecond.setZero();
    AOPT::FunctionQuadraticND::Vec bSecond(2);
    bSecond.setZero();
    bSecond(0)=-1.0;
    AOPT::FunctionQuadraticND inequalSecond(ASecond, bSecond, -10.0);


    //ineq_cons.push_back(inequalFirst);
    //ineq_cons.push_back(inequalSecond);

    //3. equality constraints
    std::vector<AOPT::FunctionBase*> eq_cons;

    AOPT::FunctionQuadraticND::Mat AThird(2, 2);
    AThird.setZero();
    AOPT::FunctionQuadraticND::Vec bThird(2);
    bThird.setZero();
    bThird(0)=1.0;
    bThird(1)=-1.0;
    AOPT::FunctionQuadraticND equality(AThird, bThird, 4.0);

    //eq_cons.push_back(equality);

    //Todo: set the lambda and v and query point
    //4. set lambdas and vs
    AOPT::FunctionQuadraticND::Vec lambda(2), nu(1);




    //5. set query point
    AOPT::OptimalityChecker::Vec x(2);

    x(0)=0.0;
    x(1)=0.0;

    //-------------------------------------------------------------------------------//

    //check
    AOPT::OptimalityChecker oc;
    if(oc.is_local_optimum(&obj_func, ineq_cons, eq_cons, x, lambda, nu))
        std::cout<<"\nThe query point is local optimum."<<std::endl;
    else
        std::cout<<"\nThe query point is NOT local optimum."<<std::endl;


    return 0;
}

