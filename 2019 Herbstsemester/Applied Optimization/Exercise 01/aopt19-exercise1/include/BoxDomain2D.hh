#pragma once

#include <iostream>
#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

/**
 * @brief The BoxDomain2D class
 * Simple indicator function returning -1 for points (x,y) such that x in [xmin,xmax]
 * and y in (ymin, ymax) and 1 otherwise.
 */
class BoxDomain2D: public FunctionBase {

public:

    BoxDomain2D(double _xmin, double _ymin, double _xmax, double _ymax)
        : xmin_{_xmin}, xmax_{_xmax},  ymin_{_ymin}, ymax_{_ymax} {}

    // number of unknowns
    inline virtual int n_unknowns() { return 2; }

    // funcion evaluation
    inline virtual double f(const Vec &_x) {
        double res = -1.0;
        if (_x[0] < xmin_ || _x[0] > xmax_ ) return 1.0;
        if (_x[1] < ymin_ || _x[1] > ymax_ ) return 1.0;
        return res;
    }

    // gradient evaluation
    inline virtual void grad_f(const Vec &_x, Vec &_g) {
    }

    // hessian matrix evaluation
    inline virtual void hess_f(const Vec &_x, Mat &_H) {
    }

    // initial point for optimization
    inline virtual void initial_x(Vec &_x) {
    }

    // callback that is called after optimization finished
    inline virtual void handle_result(const Vec &_x) {
        std::cerr << "solution converged to x = " << std::endl << _x << std::endl;
    }

private:

    double xmin_;
    double xmax_;
    double ymin_;
    double ymax_;
};

}
