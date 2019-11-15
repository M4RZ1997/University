#pragma once

#include <FunctionBase.hh>
#include <vector>
//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================
    class OptimalityChecker {
    public:
        using Vec = Eigen::VectorXd;

        OptimalityChecker(const double _epsilon = 1e-13) : eps_(_epsilon) {}
        ~OptimalityChecker(){}

        bool is_local_optimum(FunctionBase* _objective, const std::vector<FunctionBase*>& _inequality_constraints,
                const std::vector<FunctionBase*>& _equality_constraints, const Vec& _query_point, const Vec& _lambda, const Vec& _v);

    private:
        double eps_;
    };
//=============================================================================
}



