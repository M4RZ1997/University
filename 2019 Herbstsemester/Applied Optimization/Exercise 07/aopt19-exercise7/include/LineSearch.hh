#pragma once

#include "ProblemBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class LineSearch {
    public:
        // LA typedefs
        typedef ProblemBase::Vec Vec;
        typedef ProblemBase::Mat Mat;

        static double backtracking_line_search(ProblemBase *_problem,
                                               const Vec &_x,
                                               const Vec &_g,
                                               const Vec &_dx,
                                               double _t,
                                               const double _alpha = 0.2,
                                               const double _beta = 0.6) {
            //------------------------------------------------------//
            //TODO: implement the backtracking line search algorithm

            // pre-compute objective
            double fx = _problem->eval_f(_x);

            // pre-compute dot product
            double gtdx = _g.transpose() * _dx;

            // backtracking (stable in case of NAN)
            while (!(_problem->eval_f(_x + _t * _dx) <= fx + _alpha * _t * gtdx))
                _t *= _beta;

            //------------------------------------------------------//

            return _t;
        }


        static double wolfe_line_search(ProblemBase *_problem,
                                        const Vec &_x,
                                        const Vec &_g,
                                        const Vec &_dx,
                                        double _t, double _t_max = 100) {
            //------------------------------------------------------//
            //TODO: implement the line search algorithm for the Wolfe conditions
            //reference: "Numerical Optimization", "Algorithm 3.5 (Line Search Algorithm)".


            //------------------------------------------------------//

            return _t;
        }

    };
//=============================================================================
}



