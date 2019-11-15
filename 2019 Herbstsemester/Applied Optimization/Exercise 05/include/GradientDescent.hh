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
            std::cerr << "******** Gradient Descent ********" << std::endl;

            // squared epsilon for stopping criterion
            double e2 = _eps * _eps;
            //WHAT DOES THIS??
            double g2 = std::numeric_limits<double>::max();

            // get starting point
            Vec x(_problem->n_unknowns());
            _problem->initial_x(x);

            // allocate gradient storage
            Vec g(_problem->n_unknowns());
            //What does this???
            int iter(0);

            //------------------------------------------------------//
            //TODO: implement the gradient descent

            LineSearch* ls = new LineSearch;
            while(iter < _max_iters) {
                iter++;
                _problem->eval_gradient(x, g);
                g2 = ls->backtracking_line_search(_problem, x, g, -g, g2);
                if (std::sqrt((x - (x - g2 * g)).dot((x - (x - g2 * g)))) < e2){
                    x = x - g2 * g;
                    break;
                }
                x = x - g2 * g;
            }

            //------------------------------------------------------//

            // callback that algorithm finished
            _problem->handle_result(x);

            return x;
        }
    };
}



