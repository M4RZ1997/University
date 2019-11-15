#include <iostream>
#include <NewtonMethod.hh>
#include <GradientDescent.hh>
#include "../MassSpringSystem/MassSpringSystem.hh"
#include "../ModifiedMassSpringSystem/ModifiedMassSpringSystem.hh"

#include "TestProblem.hh"

using namespace AOPT;

void run_newton_method_comparison(double gamma);
void run_newton_method_with_projected_hessian(int m, int n);
void run_projected_hessian_local_separability(int m, int n);

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << "[ 0 to run all parts with default parameters, or specify: 1,2,3]\n"
                  << "Addional parameters:\n"
                  << "part 1: gamma=(10 default), or set a differerent value, example:\n  " << argv[0] << " 1 20.0\n"
                  << "part 2: m=(10) n=(10) for grid size" << std::endl;

        return EXIT_SUCCESS;
    }
    std::cout << argv[0] << " " << argv[1] << std::endl;
    std::cout << argc << std::endl;
    int opt = atoi(argv[1]);
    bool run_all = (opt == 0);

    if (run_all || opt == 1) {
        double gm = 10.0;
        if (argc > 2) {
            gm = atof(argv[2]);
        }
        run_newton_method_comparison(gm);
    }
    if (run_all || opt == 2) {
        int m = 10;
        int n = 10;
        if (argc > 2) m = atoi(argv[2]);
        if (argc > 3) n = atoi(argv[3]);
        run_newton_method_with_projected_hessian(m, n);
    }
    if (run_all || opt == 3) {
        int m = 3;
        int n = 3;
        if (argc > 2) m = atoi(argv[2]);
        if (argc > 3) n = atoi(argv[3]);
        run_projected_hessian_local_separability(m, n);
    }
    return EXIT_SUCCESS;
}

void run_newton_method_comparison(double gamma) {

    // build problem
    std::cout << "using convex quadratic problem with gamma = " << gamma << std::endl;
    TestProblem my_problem(gamma);

    // run newton method and print statistitcs
    NewtonMethods newton_solver;
    newton_solver.solve(&my_problem);

    // run gradient descent and print statistics
    GradientDescent gd_solver;
    gd_solver.solve(&my_problem);
}

void run_newton_method_with_projected_hessian(int m, int n) {

    // build problem
//    MassSpringSystem mss(m, n, 1);
//    ProblemBase* problem = mss.get_problem().get();     // unpack the shared pointer

    ModifiedMassSpringSystem mss(m, n, 1);
    ProblemBase* problem = mss.get_problem().get();

    // run newton method with projected hessian variant and print statistitcs
    NewtonMethods newton_solver;
    newton_solver.solve_with_projected_hessian( problem );

}

void run_projected_hessian_local_separability(int m, int n) {
    // build problem

    // record statistics
}

