#pragma once

#include "ProblemBase.hh"
#include "LineSearch.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    class GradientDescent {
    public:
        // LA typedefs
        typedef ProblemBase::Vec Vec;

        static Vec solve(ProblemBase *_problem, const double _eps = 1e-4, const int _max_iters = 1000000) {
            std::cout << "******** Gradient Descent ********" << std::endl;

            // squared epsilon for stopping criterion
            double e2 = _eps * _eps;
            double g2 = std::numeric_limits<double>::max();

            // get starting point
            Vec x(_problem->n_unknowns());
            _problem->initial_x(x);

            // allocate gradient storage
            Vec g(_problem->n_unknowns());
            int iter(0);

            //------------------------------------------------------//
            //TODO: implement the gradient descent
            do {
                ++iter;
                // get (negative) gradient as descent direction
                _problem->eval_gradient(x, g);

                // check stopping criterion
                g2 = g.transpose() * g;

                // print status
                std::cout << "iter: " << iter <<
                          "   obj = " << _problem->eval_f(x) <<
                          "   ||g||^2 = " << g2<< std::endl;

                if (g2 <= e2) break;

                // step size
                double t(1.0);
                // line search
//                t = LineSearch::wolfe_line_search(_problem, x, g, -g, t);
                t = LineSearch::backtracking_line_search(_problem, x, g, -g, t);
//                std::cerr<<"\nt: "<<t;

                // update
                x += t * (-g);

            } while (iter < _max_iters);

            //------------------------------------------------------//

            // callback that algorithm finished
            _problem->handle_result(x);

            return x;
        }
    };
}



