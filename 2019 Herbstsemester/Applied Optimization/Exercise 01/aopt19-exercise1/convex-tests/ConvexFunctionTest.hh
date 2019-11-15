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
 * @brief isConvex takes a function as argument and runs
 * forever if the function is convex or returns when finds the first pair of points that
 * fail convexity condition.
 */
bool isConvex(FunctionBase* _function)
{
    const int n = _function->n_unknowns();
    RandomNumberGenerator rng(n);
    FunctionBase::Vec a,b,halfAB;
//== Add your code here =======================================================
    //Lets take 1/2 as check for the line.
    
    double alpha = 0.5; 
    do{
     // Wir sollen überprüfen ob: f ( θx + ( 1 − θ ) y ) ≤ θ f ( x ) + ( 1 − θ ) f ( y )
     a = rng.get_random_nd_vector(n); // linker Vector
     b = rng.get_random_nd_vector(n); // rechter Vektor
     halfAB = alpha * a +(1-alpha)*b; // "Mittelpunkt" nach Folie : 
      
    }while(_function->f(halfAB)<=(alpha*_function->f(a)+(1-alpha)*_function->f(b)));
    printPathInfo(a, b, halfAB, alpha);
//=============================================================================
    return false;
}

}
