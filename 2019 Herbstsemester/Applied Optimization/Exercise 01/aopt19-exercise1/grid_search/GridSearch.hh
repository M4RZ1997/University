#pragma once

#include <FunctionQuadratic2D.hh>
#include <FunctionQuadraticND.hh>
#include <FunctionNonConvex2D.hh>

//== NAMESPACES ===================================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class GridSearch {
    public:
        using Vec = FunctionBase::Vec;
        using Mat = FunctionBase::Mat;

        GridSearch(const int _n_grid = 10) : n_grid_(_n_grid){}
        ~GridSearch() {}

    public:
        int grid_search_2d(const Vec& _x_l, const Vec& _x_u, const int _func = 0) {
            std::cout<<"Grid searching the minimum of a 2d function..."<<std::endl;

            FunctionNonConvex2D fnc2d;
            FunctionQuadratic2D fq2d(-1.);

            //------------------------------------------------------//
            //Todo: implement the 2d version of the grid search
            // algorithm to find minimum value of function 0 and function 1
            //------------------------------------------------------//
            double f = 0., fmin = std::numeric_limits<double>::max();
            Vec x(2), x_min(2), dx(2);
            dx = (_x_u - _x_l)/n_grid_;

            for(int i=0; i<=n_grid_; ++i) {
                x[0] = _x_l[0] + dx[0]*i;
                for(int j=0; j<=n_grid_; ++j) {
                    x[1] = _x_l[1] + dx[1]*j;
                    if(_func == 0)
                        f = fnc2d.f(x);
                    else if(_func == 1)
                        f = fq2d.f(x);

                    if(f < fmin) {
                        fmin = f;
                        x_min = x;
                    }
                }
            }

            std::cout<<"Minimum value of the function "<<" is: "<<fmin<<" at x:\n"<<x_min<<std::endl;
            return 0;
            //------------------------------------------------------//
        }


        int grid_search_nd(const int _n, const Vec& _x_l, const Vec& _x_u) {
            if(_x_l.size() != _x_u.size()) {
                std::cout<<"Error: inputs are not of the same dimension!"<<std::endl;
                return -1;
            }
            std::cout<<"Grid searching the minimum of a "<<_x_l.size()<<" d function..."<<std::endl;
            //------------------------------------------------------//
            //Todo: implement the nd version of the grid search
            // algorithm to find minimum value of a nd quadratic function
            FunctionQuadraticND fqnd(_n);
            double fmin = std::numeric_limits<double>::max();
            Vec x(fqnd.n_unknowns()), x_min(fqnd.n_unknowns()), dx(fqnd.n_unknowns());

            dx = (_x_u - _x_l)/n_grid_;

            x = _x_l;

            grid_iterate(fqnd, 0, _x_l, dx, x, x_min, fmin);
            std::cout<<"Minimum value of tje function is: "<<fmin<<" at x:\n"<<x_min<<std::endl;

            return 0;
            //------------------------------------------------------//
        }

    private:
        void grid_iterate(FunctionQuadraticND& _fqnd, int _d, const Vec& _x_l, const Vec& _dx,
                          Vec& _x, Vec& _x_min, double& _fmin) {
            if(_d >= _fqnd.n_unknowns()) {
                double f = _fqnd.f(_x);
                //std::cerr<<"\nf: "<<f<<" _x: "<<_x;

                if(f < _fmin) {
                    _fmin = f;
                    _x_min = _x;
                }
                return;
            }

            for(int i=0; i<= n_grid_; i++) {
                _x[_d] = _x_l[_d] + i*_dx[_d];
                //std::cerr<<"\ni: "<<i<<" d: "<<_d;
                grid_iterate(_fqnd, _d+1, _x_l, _dx, _x, _x_min, _fmin);
            }
        }

    private:
        int n_grid_;
    };

    //=============================================================================
}





