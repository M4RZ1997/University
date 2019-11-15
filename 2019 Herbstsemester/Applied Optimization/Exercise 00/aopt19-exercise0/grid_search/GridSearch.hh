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
            //algorithm to find minimum value of function 0 and function 1
            //------------------------------------------------------//
            double minimum;
            double step = (_x_u(0) - _x_l(0))/(n_grid_-1);
            Eigen::Vector2d vec_dir_1,vec_dir_2;
            //Vec vec_dir_1(2) = step, 0;
            vec_dir_1 << step,0;
            vec_dir_2 << 0,step;
            //Vec vec_dir_2(2) = 0, step;


            if(_func == 0){
                minimum = fnc2d.f(_x_l);
                for (int i = 0; i < n_grid_; i++){
                    for (int h = 0; h < n_grid_; h++){
                        double nonCon_y = fnc2d.f(_x_l + (i * vec_dir_1) + (h * vec_dir_2));
                        if (nonCon_y < minimum){
                            minimum = nonCon_y;
                        }
                    }
                }
            }
            else if(_func == 1) {
                int hilfsvariable = 0;
                minimum = fq2d.f(_x_l);
                for (int i = 1; i < n_grid_; i++) {
                    double quadratic2d_f1 = fq2d.f(_x_l + (i * vec_dir_1));
                    if (quadratic2d_f1 < minimum) {
                        minimum = quadratic2d_f1;
                    } else {
                        hilfsvariable = i;
                        break;
                    }
                }
                for (int h = 1; h < n_grid_; h++) {
                    double quadratic2d_f2 = fq2d.f(_x_l + (hilfsvariable * vec_dir_1) + (h * vec_dir_2));
                    if (quadratic2d_f2 < minimum) {
                        minimum = quadratic2d_f2;
                    } else {
                        break;
                    }
                }
            }
            std::cout<<"Minimum : "<<minimum<<" "<<std::endl;
            return minimum;
            /*
            if(_func==0){
                double nonCon2d_l = fnc2d.f(_x_l);
                double nonCon2d_u = fnc2d.f(_x_u);
                if(nonCon2d_l<nonCon2d_u){minimum =nonCon2d_l;}
                else {minimum = nonCon2d_u;}
            } else if(_func==1){
                double quad2d_l = fq2d.f(_x_l);
                double quad2d_u = fq2d.f(_x_u);
                if(quad2d_l <quad2d_u ){minimum =quad2d_l; }
                else {minimum = quad2d_u; }
            }
            else{
                minimum = 0;
            }
            return minimum;
             */
            //------------------------------------------------------//
            //return 0;
        }


        int grid_search_nd(const int _n, const Vec& _x_l, const Vec& _x_u) {
            if(_x_l.size() != _n || _x_u.size() != _n) {
                std::cout<<"Error: inputs are not of"<<_n<<" dimension!"<<std::endl;
                return -1;
            }
            std::cout<<"Grid searching the minimum of a "<<_n<<" d function..."<<std::endl;
            //------------------------------------------------------//
            //Todo: implement the nd version of the grid search
            //algorithm to find the minimum value of a n-dimension
            //quadratic function. Hint: use FunctionQuadratic2D class
            //to evaluate function value
            //------------------------------------------------------//
            FunctionQuadratic2D fq2d(-1.);
            double minimum;
            double step = (_x_u(0) - _x_l(0))/ (n_grid_ - 1);
            Vec n_dim_zero_vec(_n), final_vec(_n);
            for(int i = 0; i < _n; i++){
                n_dim_zero_vec(i) = 0;
                final_vec(i) = 0;
            }
            for (int dim = 0; dim < _n; ++dim) {
                Vec n_dim_dir_vec = n_dim_zero_vec;
                n_dim_dir_vec(dim) = step;
                minimum = fq2d.f(_x_l + final_vec);
                for (int j = 1; j < n_grid_; ++j) {
                    double nd_f = fq2d.f(_x_l + final_vec + (j * n_dim_dir_vec));
                    if (nd_f < minimum) {
                        minimum = nd_f;
                        final_vec(dim) = (j * step);
                    } else {
                        break;
                    }
                }

            }

            /*
            FunctionQuadraticND fnqNd(_n);
            double minimum ;
            double quadNd_l = fnqNd.f(_x_l);
            double quadNd_u = fnqNd.f(_x_u);
            if (quadNd_l<quadNd_u){minimum =quadNd_l;}
            else {minimum =quadNd_u;}
            */
            //------------------------------------------------------//
            //return 0;
            std::cout<<"Minimum : "<<minimum<<" "<<std::endl;
            return minimum;
        }

    private:
        int n_grid_;
        // double minimumDouble;
        // Vec minimumVector;
    };

    //=============================================================================
}





