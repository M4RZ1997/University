#pragma once

#include "FunctionBase.hh"

//== NAMESPACES ===============================================================

namespace AOPT {


//== CLASS DEFINITION =========================================================

    class MassSpringProblem2D {
    public:
        using Vec = FunctionBase::Vec;
        using Mat = FunctionBase::Mat;
        using Edge = std::pair<int, int>;

        MassSpringProblem2D(FunctionBase& _spring, const int _n_unknowns) : func_(_spring), n_(_n_unknowns) {
            xe_.resize(func_.n_unknowns());
            ge_.resize(func_.n_unknowns());
            he_.resize(func_.n_unknowns(), func_.n_unknowns());
        }

        ~MassSpringProblem2D() {}

        virtual int n_unknowns() {
            return n_;
        }

        virtual void initial_x(Vec &_x) {
            // use current solution
            for (unsigned int i = 0; i < n_; ++i)
                _x[i] = 0.;
        }

        virtual double eval_f(const Vec &_x) {
            double energy(0);
            Vec coeff(2);

            //------------------------------------------------------//
            //TODO: assemble function values of all the elements (springs)
            for (int i = 0; i < elements_.size(); i++) {
                Edge currentEdge = elements_[i];
                Vec currentPoints(4);
                coeff(0)=ks_[i];
                coeff(1)=ls_[i];
                Vec points(4);
                currentPoints(0) =_x[currentEdge.first];
                currentPoints(1)=_x[currentEdge.first+1];
                currentPoints(2) =_x[currentEdge.second];
                currentPoints(3)=_x[currentEdge.second+1];
                energy += func_.f(currentPoints,coeff);

            }
            return energy;
            //------------------------------------------------------//


        }

        virtual void eval_gradient(const Vec &_x, Vec &_g) {
            _g.resize(n_unknowns());
            _g.setZero();
            Vec coeff(2);

            //------------------------------------------------------//
            //TODO: assemble local gradient vector to the global one
            for (int i = 0; i < elements_.size(); i++) {
                Edge currentEdge = elements_[i];
                Vec currentPoints(4);
                coeff(0)=ks_[i];
                coeff(1)=ls_[i];
                Vec points(4);
                currentPoints(0) =_x[currentEdge.first];
                currentPoints(1)=_x[currentEdge.first+1];
                currentPoints(2) =_x[currentEdge.second];
                currentPoints(3)=_x[currentEdge.second+1];
                func_.grad_f(currentPoints,coeff,_g);
                for (int j = 0; j < n_unknowns() ; j++) {
                    ge_(i+j)=_g[j];
                }
            }
            //------------------------------------------------------//
        }

        //Hessian in dense matrix
        virtual void eval_hessian(const Vec &_x, Mat& _h) {
            _h.resize(n_unknowns(), n_unknowns());
            _h.setZero();
            Vec coeff(2);

            //------------------------------------------------------//
            //TODO: assemble local hessian matrix to the global one
            for (int i = 0; i < elements_.size(); i++) {
                Edge currentEdge = elements_[i];
                Vec currentPoints(4);
                coeff(0)=ks_[i];
                coeff(1)=ls_[i];
                Vec points(4);
                currentPoints(0) = _x[currentEdge.first];
                currentPoints(1) = _x[currentEdge.first+1];
                currentPoints(2) = _x[currentEdge.second];
                currentPoints(3) = _x[currentEdge.second+1];
                func_.hess_f(currentPoints,coeff,_h);

                for (int j = 0; j < n_unknowns(); j++) {

                    for (int k = 0; k < n_unknowns() ; k++) {

                        he_(i+j,i+k)= _h(j,k); // Brackets only for vectors
                    }
                }
            }
            //------------------------------------------------------//
        }


        void add_spring_element(const int _v_idx0, const int _v_idx1, const double _k = 1., const double _l = 1.)
        {
            if(2*_v_idx0 > (int)n_ || _v_idx0 < 0 || 2*_v_idx1 >= (int)n_ || _v_idx1 < 0 )
                std::cout << "Warning: invalid spring element was added... " << _v_idx0 << " " << _v_idx1 << std::endl;
            else {
                elements_.push_back(Edge(_v_idx0, _v_idx1));
                ks_.push_back(_k);
                ls_.push_back(_l);
            }
        }


    private:
        int n_;
        std::vector<Edge> elements_;
        FunctionBase& func_;

        //vector of constants
        std::vector<double> ks_;
        std::vector<double> ls_;

        // scalar for each element
        Vec xe_;
        // gradient for each element
        Vec ge_;
        // hessian for each element
        Mat he_;
    };

//=============================================================================
}


