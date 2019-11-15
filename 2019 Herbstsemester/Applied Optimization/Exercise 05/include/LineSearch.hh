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

            while (_problem->eval_f(_x + _t*_dx) < ((_problem->eval_f(_x) ) + (_t * _alpha * (_g.dot(_dx)))) )
            {_t *= _beta;
            }
            //------------------------------------------------------//

            return _t;
        }

    };
//=============================================================================
}



