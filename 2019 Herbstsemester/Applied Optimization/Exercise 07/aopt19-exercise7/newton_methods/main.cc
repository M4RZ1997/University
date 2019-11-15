#include <iostream>
#include <NewtonMethod.hh>
#include <GradientDescent.hh>
#include <OptimizationStatistic.hh>

#include <MassSpringSystem.hh>

#include "TestProblem.hh"
//#include "ProblemWithLocallySeparableHessian.hh"

using namespace AOPT;

void run_newton_method_comparison(double gamma);
void run_newton_method_with_projected_hessian(int m, int n);
void run_projected_hessian_local_separability(int m, int n);

void run_newton_method_on_spring_system(int m, int n, int type, double eps, std::string filename);

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << "[ 0 to run all parts with default parameters, or specify: 1,2,3]\n"
                  << "Addional parameters:\n"
                  << "part 1: gamma=(10 default), or set a differerent value, example:\n  " << argv[0] << " 1 20.0\n"
                  << "part 2: m=(10) n=(10) for grid size\n"
                  << "part 3: m=(3) n=(3) for grid size\n"
                  << "part 4: m=(10) n=(10) type=(0 without leght, 1 with lenght) eps=(0.001) filepath=(results_sys_t<type>_eps<eps>)\n"
                    << std::endl;

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
    if (run_all || opt == 4) {
        int m = 10;
        int n = 10;
        int type = 0;
        double eps = 1e-4;
        
        std::stringstream ss;
        ss << "results_sys_t" << type << "_eps" << eps;
        std::string filename = ss.str();
        
        if (argc > 2) m = atoi(argv[2]);
        if (argc > 3) n = atoi(argv[3]);
        if (argc > 4) type = atoi(argv[4]);
        if (argc > 5) eps = atof(argv[5]);
        if (argc > 6) filename = std::string(argv[6]);
        run_newton_method_on_spring_system(m, n, type, eps, filename);
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

void run_newton_method_on_spring_system(int m, int n, int type, double eps,
                                        std::string filename)
{
    MassSpringSystem mss(m, n, type);
    
    auto opt_statistics_ptr = std::make_unique<OptimizationStatistic>(mss.get_problem().get());
    
    ProblemBase* problem = opt_statistics_ptr.get();

    // run newton method with projected hessian variant and print statistitcs
    NewtonMethods newton_solver;
    
    NewtonMethods::Vec x;
    if (type == 0) {
        x = newton_solver.solve(problem, eps);
    } else if (type == 1) {
        x = newton_solver.solve_with_projected_hessian(problem);
    } else {
        std::cout << "type " << type << " not supported" << std::endl;
        return;
    }
    mss.set_spring_graph_vertices(x);
    std::cout << "saving results to " << filename << std::endl;
    mss.save_spring_system(filename.c_str());
}

void run_newton_method_with_projected_hessian(int m, int n) {

    // build problem
//    MassSpringSystem mss(m, n, 1);
//    ProblemBase* problem = mss.get_problem().get();     // unpack the shared pointer

    MassSpringSystem mss(m, n, 1);
    ProblemBase* problem = mss.get_problem().get();

    // run newton method with projected hessian variant and print statistitcs
    NewtonMethods newton_solver;
    newton_solver.solve_with_projected_hessian( problem );

}

void run_projected_hessian_local_separability(int m, int n) {
    // build problem
//    MassSpringSystem mss(m, n, 0);  // without lenth
//    ProblemBase* problem = mss.get_problem().get();

    // record statistics
}
