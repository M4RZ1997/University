#include "OptimalityChecker.hh"

namespace AOPT {

    bool OptimalityChecker::
    is_local_optimum(FunctionBase *_objective, const std::vector<FunctionBase *>& _inequality_constraints,
            const std::vector<FunctionBase *>& _equality_constraints,
            const Vec& _query_point, const Vec& _lambda, const Vec& _nu) {

        //-------------------------------------------------------------------------------//
        //Todo: check every item of the KKT conditions

        for (int i = 0; i < _inequality_constraints.size() ; i++) {
            if(_inequality_constraints[i]->f(_query_point)>0){ return  false;}
        }
        for (int i = 0; i < _equality_constraints.size() ; i++) {
            if(_equality_constraints[i]->f(_query_point)!=0){ return  false;}
        }
        for (int j = 0; j < _lambda.size(); j++) {
            if(_lambda[j]<0){return false;}
        }
        for (int k = 0; k < _inequality_constraints.size(); k++) {
            if((_lambda[k]*_inequality_constraints[k]->f(_query_point))!=0){return false;}
        }
         Vec grad_zero, ergebnis;
        _objective->grad_f(_query_point,grad_zero);

        ergebnis=grad_zero;

        for (int l = 0; l < _inequality_constraints.size(); l++) {

            _inequality_constraints[l]->grad_f(_query_point,grad_zero);
            ergebnis += _lambda[l]*grad_zero;
        }
        for (int l = 0; l < _equality_constraints.size(); l++) {

            _equality_constraints[l]->grad_f(_query_point,grad_zero);
            ergebnis += _nu[l]*grad_zero;
        }

        for (int m = 0; m < ergebnis.size(); m++) {
            if(ergebnis[m]>eps_){ return false;}
        }
        return true;

        //-------------------------------------------------------------------------------//
    }
}