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

            //    // backtracking (unstable in case of NAN)
//          while(_problem->f(_x+_t*_dx) > fx + _alpha*_t*gtdx)
//          _t *= _beta;

            // backtracking (stable in case of NAN)
            while (!(_problem->eval_f(_x + _t * _dx) <= fx + _alpha * _t * gtdx))
                _t *= _beta;

            //------------------------------------------------------//

            return _t;
        }

        static double exact_line_search(ProblemBase* _problem,
                                        const Vec&     _x,
                                        const Vec&    _dx,
                                        double        _t,
                                        const double _eps = 1e-9)
        {
            // 1. search appropriate interval
            double l = 0.0;
            double u = _t;

            // get upper bound which is inside domain
            while(1)
            {
                double f = _problem->eval_f(_x+u*_dx);
                if( std::isnan(f) || std::isinf(f))
                    u*=0.5;
                else
                    break;
            }

            // get gradient at upper bound
            Vec g(_problem->n_unknowns());
            _problem->eval_gradient(_x+u*_dx, g);

            // repeat until derivative gets positive
            while(g.transpose()*_dx <= 0 )
            {
                u *= 2.0;
                _problem->eval_gradient(_x+u*_dx, g);

                // check for domain violation
                double f = _problem->eval_f(_x+u*_dx);
                if( std::isnan(f) || std::isinf(f))
                {
                    u *= 0.5;
                    break;
                }
            }

            // 2. binary search
            while(u-l > _eps)
            {
                // get gradient at midpoint
                double m = 0.5*(u+l);
                _problem->eval_gradient(_x+m*_dx, g);
                if(g.transpose()*_dx  < 0)
                    l = m;
                else
                    u = m;
            }

            // 3. make sure that decreasing
            _t = u;
            double fx = _problem->eval_f(_x);
            while( !(fx > _problem->eval_f(_x+_t*_dx)) ) _t *= 0.8;

            return _t;
        }
    };
//=============================================================================
}



