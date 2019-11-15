#include "OptimalityChecker.hh"

namespace AOPT {

    bool OptimalityChecker::
    is_local_optimum(FunctionBase *_objective, const std::vector<FunctionBase *>& _inequality_constraints,
            const std::vector<FunctionBase *>& _equality_constraints,
            const Vec& _query_point, const Vec& _lambda, const Vec& _nu) {
        if(_equality_constraints.empty() && _inequality_constraints.empty()) {
            Vec g;
            _objective->grad_f(_query_point, g);
            if(g.norm() < eps_)
                return true;
            else
                return false;
        } else {
            if(_inequality_constraints.size() != _lambda.size() || _equality_constraints.size() != _nu.size()) {
                std::cout<<"\nDual variable size should be the same as the constraint size!"<<std::endl;
                return false;
            }

            //1. check inequality constraints
            for(auto ineq_cons : _inequality_constraints){
                double val = ineq_cons->f(_query_point);
                if(val > 0){
                    std::cout<<"\nInequality Constraint violated! "<<std::endl;
                    return false;
                }
            }

            //2. check equality constraints
            for(auto eq_cons : _equality_constraints) {
                double val = eq_cons->f(_query_point);
                if(fabs(val) > eps_){
                    std::cout<<"\nEq Constraint violated! "<<std::endl;
                    return false;
                }
            }

            //3. check lambda
            for(size_t i=0; i<_lambda.size(); ++i) {
                if (_lambda[i] < 0) {
                    std::cout<<"\nLambda is smaller than 0! "<<std::endl;
                    return false;
                }
            }

            //4. check complementary slackness
            for(size_t i=0; i<_lambda.size(); ++i) {
                double val = _inequality_constraints[i]->f(_query_point);
                if(fabs(val * _lambda[i]) > eps_) {
                    std::cout<<"\nComplementary slackness violated! "<<std::endl;
                    return false;
                }
            }

            //5. check gradient
            int dim = _objective->n_unknowns();
            Vec g(dim);
            g.setZero();

            Vec gi(dim);
            _objective->grad_f(_query_point, gi);
            g += gi;

            for(int i=0; i<_inequality_constraints.size(); i++) {
                _inequality_constraints[i]->grad_f(_query_point, gi);
                g += _lambda[i] * gi;
            }

            for(int i=0; i<_equality_constraints.size(); i++) {
                _equality_constraints[i]->grad_f(_query_point, gi);
                g += _nu[i] * gi;
            }

            if(g.norm() > eps_) {
                std::cout<<"\nGradient condition violated! "<<g.norm()<<std::endl;
                return false;
            }


            return true;
        }
    }
}