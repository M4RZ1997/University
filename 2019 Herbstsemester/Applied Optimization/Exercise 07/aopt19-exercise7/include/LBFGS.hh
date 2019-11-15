#pragma once

#include <Eigen/Core>
#include "LineSearch.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class LBFGS {
    public:
        using Vec = ProblemBase::Vec;
        using Mat = ProblemBase::Mat;
        using MapVec = Eigen::Map<Vec>;

        LBFGS(const int _m): m_(_m) {}

        Vec solve(ProblemBase *_problem, const double _eps = 1e-4, const int _max_iters = 100000) {
            std::cout << "******** LBFGS ********" << std::endl;

            int n = _problem->n_unknowns();
            init_storage(n);

            // get starting point
            Vec x(n);
            _problem->initial_x(x);

            if(m_ < 1) {
                std::cout<<"\nError: m should be larger than 0!"<<std::endl;
                return x;
            }

            //------------------------------------------------------//
            //TODO: implement the LBFGS algorithm
            //hint: use back tracking line search for the step length
            //or Wolfe line search if you implement it


            //------------------------------------------------------//

            return x;
        }

    private:
        void init_storage(const int _n) {
            mat_y_.resize(_n, m_);
            mat_s_.resize(_n, m_);
            xp_.resize(_n);
            gp_.resize(_n);
            r_.resize(_n);
            rho_.resize(m_);
            alpha_.resize(m_);
        }

    private:
        int m_;
        //store y
        Mat mat_y_;
        //store s
        Mat mat_s_;
        //previous function value
        double fp_;
        //previous x
        Vec xp_;
        //previous gradient
        Vec gp_;
        //move direction
        Vec r_;
        Vec alpha_;
        Vec rho_;

    };

//=============================================================================
}





