#pragma once

#include "ProblemBase.hh"
#include "LineSearch.hh"


namespace AOPT {

/**
 * @brief NewtonMethods is just a list of functions implementing several variations of the
 * newton method
 */
class NewtonMethods {
public:
    typedef ProblemBase::Vec    Vec;   // dense vector arbitrary size
    typedef ProblemBase::Mat    Mat;   // dense matrix arbitrary size
    typedef ProblemBase::SMat   SMat;  // sparse matrix arbitrary size
    
    /**
     * @brief solve
     * @param problem problem instance to be solved, must provide a hessian implementation
     */
    static Vec solve(ProblemBase* _problem, const double _eps = 1e-4, const int _max_iters = 1000000)
    {
        std::cout << "******** Newton Method ********" << std::endl;

        // squared epsilon for stopping criterion
        double e2 = _eps * _eps;
        double lambda2 = std::numeric_limits<double>::max();

        // get starting point
        int n = _problem->n_unknowns();
        Vec x(n);
        _problem->initial_x(x);

        // allocate gradient storage
        Vec g(n);

        // allocate hessian storage
        Mat H(n,n);

        // allocate search direction vector storage
        Vec delta_x(n);
        int iter(0);

        //------------------------------------------------------//
        //TODO: implement Newton method
        do {
            ++iter;

            _problem->eval_gradient(x, g);
            _problem->eval_hessian(x, H);

            //Kein Plan

            ldlt.compute(H);
            delta_x = ldlt.solve(g);

            lambda2 = g.transpose() * delta_x;

            std::cout << "iter: " << iter <<
                      "   obj = " << _problem->eval_f(x) <<
                      "   lambda^2 = " << lambda2<< std::endl;

            if (lambda2/2 <= e2) break;

            // step size
            double t(1.0);
            // line search
            t = LineSearch::backtracking_line_search(_problem, x, g, -g, t);
            // update
            x += t * delta_x;

        } while (iter < _max_iters);

        //------------------------------------------------------//

        // callback that algorithm finished
        _problem->handle_result(x);

        return x;
    }

    /**
     * @brief solve_with_projected_hessian
     * @param problem must provide a hessian, it may or may not be positive definite
     * the hessian is projected to a positive definite by adding positive values to the diagonal of the hessian
     * tao_factor is the
     */
    static Vec solve_with_projected_hessian(ProblemBase* _problem, double tau_factor = 10.0,
                                            const double _eps = 1e-3, const int _max_iters = 1000000 )
    {
        std::cout << "******** Newton Method with projected hessian ********" << std::endl;

        // squared epsilon for stopping criterion
        double e2 = _eps * _eps;
        double lambda2 = std::numeric_limits<double>::max();

        // get starting point
        int n = _problem->n_unknowns();
        Vec x(n);
        _problem->initial_x(x);

        // allocate gradient storage
        Vec g(n);

        // allocate hessian storage
        Mat H(n,n);

        // allocate search direction vector storage
        Vec delta_x(n);
        int iter(0);

        // identity and scalar to add positive values to the diagonal
        Mat I = Mat::Identity(n,n);


        //------------------------------------------------------//
        //TODO: implement Newton with projected hessian method
	
	LDLT<SMat> ldlt(n);
        do {
            ++iter;
            double tau = 1.;

            _problem->eval_gradient(x, g);
            _problem->eval_hessian(x, H);

            ldlt.compute(H);

            while(ldlt.isNegative()) {
                H += tau * I;
                tau *= tau_factor;
                ldlt.compute(H);
            }
            delta_x = ldlt.solve(g);

            lambda2 = g.transpose() * delta_x;

            std::cout << "iter: " << iter <<
                      "   obj = " << _problem->eval_f(x) <<
                      "   lambda^2 = " << lambda2<< std::endl;

            if (lambda2/2 <= e2) break;

            // step size
            double t(1.0);
            // line search
            t = LineSearch::backtracking_line_search(_problem, x, g, -g, t);
            // update
            x += t * delta_x;

        } while (iter < _max_iters);

        //------------------------------------------------------//

        // callback that algorithm finished
        _problem->handle_result(x);

        return x;

    }

};

} // namespace AOPT
