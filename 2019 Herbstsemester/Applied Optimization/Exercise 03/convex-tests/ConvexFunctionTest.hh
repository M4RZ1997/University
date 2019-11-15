#pragma once

#include <iostream>
#include <RandomNumberGenerator.hh>
#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

void printPathInfo(FunctionBase::Vec p1, FunctionBase::Vec p2, FunctionBase::Vec p, double t)
{
    std::cout << "path: p(t) = (1 - t) * p1 + t * p2; \nwith:\n"
              << "  p1 = (" << p1.transpose() << ")\n"
              << "  p2 = (" << p2.transpose() << ")\n"
              << "  p (t = "<< t << ") = (" << p.transpose() << ")" << std::endl;
}

/**
 * @brief isConvex takes a function and a domain represented as a level set and runs
 * forever if the function is convex or returns when finds the first pair of points that
 * fail convexity conditions of either the function or its domain.
 */
bool isConvex(FunctionBase* _function)
{
    const int n = _function->n_unknowns();
    RandomNumberGenerator rng(n);

//== Add your code here =======================================================
    const int n_evals = 10;
    double delta = 1.0 / n_evals;
    while(1) {

        // generate a pair of points
        FunctionBase::Vec p1 = rng.get_random_nd_vector(n);
        FunctionBase::Vec p2 = rng.get_random_nd_vector(n);

        // evaluate the function along the convex combination of p1 and p2
        double f1 = _function->f(p1);
        double f2 = _function->f(p2);

        for (int i = 1; i < n_evals; i++) {

            double t = delta*i;

            FunctionBase::Vec p = (1.0 - t) * p1 + t * p2;
            double fp = _function->f(p);
            double sp = (1.0 - t)*f1 + t * f2;

            // check that the function f(p) <= (1 - t)*f(p1) + t*f(p2)
            if (fp > sp) {
                std::cout << "Function non convexity detected: f(p) = " << fp
                          << " > (1 - t)*f(p1) + t*f(p2) = " << sp << "\n";
                printPathInfo(p1, p2, p, t);
                return false;
            }
        }
    }
//=============================================================================
    return false;
}

}
