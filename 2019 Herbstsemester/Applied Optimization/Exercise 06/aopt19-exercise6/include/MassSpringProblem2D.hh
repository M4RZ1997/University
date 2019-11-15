#pragma once

#include "ProblemBase.hh"
#include "FunctionBase.hh"
#include "FixingNodeElement.hh"

//== NAMESPACES ===============================================================

namespace AOPT {


//== CLASS DEFINITION =========================================================

    class MassSpringProblem2D : public ProblemBase {
    public:
        using Vec = ProblemBase::Vec;
        using Mat = ProblemBase::Mat;
        using Edge = std::pair<int, int>;
        // sparse matrix type
        using SMat = ProblemBase::SMat;
        // triplet
        using T = ProblemBase::T;

        MassSpringProblem2D(FunctionBase& _spring, const int _n_unknowns) : ProblemBase(), func_(_spring), n_(_n_unknowns) {
            xe_.resize(func_.n_unknowns());
            ge_.resize(func_.n_unknowns());
            he_.resize(func_.n_unknowns(), func_.n_unknowns());

            nc_xe_.resize(fne_.n_unknowns());
            nc_ge_.resize(fne_.n_unknowns());
            nc_he_.resize(fne_.n_unknowns(), fne_.n_unknowns());

            initial_x_.setZero();
        }

        ~MassSpringProblem2D() {}

        virtual int n_unknowns() override {
            return n_;
        }

        virtual void initial_x(Vec &_x) override {
            // hard code to zero
            _x = initial_x_;
        }

        virtual double eval_f(const Vec &_x) override {
            double energy(0);

            //------------------------------------------------------//
            //TODO: assemble function values of all the elements (springs + node constraints)
            Vec coeff(2);
            for(size_t i=0; i<springs_.size(); ++i) {
                xe_[0] = _x[2*springs_[i].first];
                xe_[1] = _x[2*springs_[i].first+1];

                xe_[2] = _x[2*springs_[i].second];
                xe_[3] = _x[2*springs_[i].second+1];

                coeff[0] = ks_[i];
                coeff[1] = ls_[i];

                energy += func_.f(xe_, coeff);
            }

            Vec coeff1(3);
            for(int i=0; i<fixed_node_indices_.size(); ++i) {
                nc_xe_[0] = _x[2*fixed_node_indices_[i]];
                nc_xe_[1] = _x[2*fixed_node_indices_[i]+1];

                coeff1[0] = penalties_[i];
                coeff1[1] = fixed_points_[2*i];
                coeff1[2] = fixed_points_[2*i+1];

                energy += fne_.f(nc_xe_, coeff1);
            }
            //------------------------------------------------------//

            return energy;
        }

        virtual void eval_gradient(const Vec &_x, Vec &_g) override {
            _g.resize(n_unknowns());
            _g.setZero();
            Vec coeff(2);

            //------------------------------------------------------//
            //TODO: assemble local gradient vector to the global one (springs + node constraints)
            for(size_t i=0; i<springs_.size(); ++i) {
                xe_[0] = _x[2 * springs_[i].first];
                xe_[1] = _x[2 * springs_[i].first + 1];

                xe_[2] = _x[2 * springs_[i].second];
                xe_[3] = _x[2 * springs_[i].second + 1];

                coeff[0] = ks_[i];
                coeff[1] = ls_[i];
                // get local gradient
                func_.grad_f(xe_, coeff, ge_);

                //copy to global
                _g[2 * springs_[i].first] += ge_[0];
                _g[2 * springs_[i].first + 1] += ge_[1];
                _g[2 * springs_[i].second] += ge_[2];
                _g[2 * springs_[i].second + 1] += ge_[3];
            }

            Vec coeff1(3);
            for(int i=0; i<fixed_node_indices_.size(); ++i) {
                nc_xe_[0] = _x[2*fixed_node_indices_[i]];
                nc_xe_[1] = _x[2*fixed_node_indices_[i]+1];

                coeff1[0] = penalties_[i];
                coeff1[1] = fixed_points_[2*i];
                coeff1[2] = fixed_points_[2*i+1];

                // get local gradient
                fne_.grad_f(nc_xe_, coeff1, nc_ge_);

                //copy to global
                _g[2 * fixed_node_indices_[i]] += nc_ge_[0];
                _g[2 * fixed_node_indices_[i] + 1] += nc_ge_[1];
            }
            //------------------------------------------------------//
        }

        //Hessian in dense matrix
        virtual void eval_hessian(const Vec &_x, Mat& _h) override {
            _h.resize(n_unknowns(), n_unknowns());
            _h.setZero();
            Vec coeff(2);

            //------------------------------------------------------//
            for(int i=0; i<ks_.size(); ++i) {
                xe_[0] = _x[2 * springs_[i].first];
                xe_[1] = _x[2 * springs_[i].first + 1];

                xe_[2] = _x[2 * springs_[i].second];
                xe_[3] = _x[2 * springs_[i].second + 1];

                coeff[0] = ks_[i];
                coeff[1] = ls_[i];

                //get local hessian
                func_.hess_f(xe_, coeff, he_);

                //copy to global
                _h(2 * springs_[i].first, 2 * springs_[i].first) += he_(0,0);
                _h(2 * springs_[i].first, 2 * springs_[i].first+1) += he_(0,1);
                _h(2 * springs_[i].first, 2 * springs_[i].second) += he_(0,2);
                _h(2 * springs_[i].first, 2 * springs_[i].second+1) += he_(0,3);

                _h(2 * springs_[i].first+1, 2 * springs_[i].first) += he_(1,0);
                _h(2 * springs_[i].first+1, 2 * springs_[i].first+1) += he_(1,1);
                _h(2 * springs_[i].first+1, 2 * springs_[i].second) += he_(1,2);
                _h(2 * springs_[i].first+1, 2 * springs_[i].second+1) += he_(1,3);

                _h(2 * springs_[i].second, 2 * springs_[i].first) += he_(2,0);
                _h(2 * springs_[i].second, 2 * springs_[i].first+1) += he_(2,1);
                _h(2 * springs_[i].second, 2 * springs_[i].second) += he_(2,2);
                _h(2 * springs_[i].second, 2 * springs_[i].second+1) += he_(2,3);

                _h(2 * springs_[i].second+1, 2 * springs_[i].first) += he_(3,0);
                _h(2 * springs_[i].second+1, 2 * springs_[i].first+1) += he_(3,1);
                _h(2 * springs_[i].second+1, 2 * springs_[i].second) += he_(3,2);
                _h(2 * springs_[i].second+1, 2 * springs_[i].second+1) += he_(3,3);
            }
            //------------------------------------------------------//
        }


        //Hessian in sparse matrix
        virtual void eval_hessian(const Vec &_x, SMat& _h) override {
            Vec coeff(2);

            //------------------------------------------------------//
            std::vector<T> triplets;
            triplets.reserve(16*springs_.size());
            for(int i=0; i<ks_.size(); ++i) {
                xe_[0] = _x[2 * springs_[i].first];
                xe_[1] = _x[2 * springs_[i].first + 1];

                xe_[2] = _x[2 * springs_[i].second];
                xe_[3] = _x[2 * springs_[i].second + 1];

                coeff[0] = ks_[i];
                coeff[1] = ls_[i];

                //get local hessian
                func_.hess_f(xe_, coeff, he_);

                //copy to global
                triplets.push_back(T(2 * springs_[i].first, 2 * springs_[i].first, he_(0,0)));
                triplets.push_back(T(2 * springs_[i].first, 2 * springs_[i].first+1, he_(0,1)));
                triplets.push_back(T(2 * springs_[i].first, 2 * springs_[i].second, he_(0,2)));
                triplets.push_back(T(2 * springs_[i].first, 2 * springs_[i].second+1, he_(0,3)));

                triplets.push_back(T(2 * springs_[i].first+1, 2 * springs_[i].first, he_(1,0)));
                triplets.push_back(T(2 * springs_[i].first+1, 2 * springs_[i].first+1, he_(1,1)));
                triplets.push_back(T(2 * springs_[i].first+1, 2 * springs_[i].second, he_(1,2)));
                triplets.push_back(T(2 * springs_[i].first+1, 2 * springs_[i].second+1, he_(1,3)));

                triplets.push_back(T(2 * springs_[i].second, 2 * springs_[i].first, he_(2,0)));
                triplets.push_back(T(2 * springs_[i].second, 2 * springs_[i].first+1, he_(2,1)));
                triplets.push_back(T(2 * springs_[i].second, 2 * springs_[i].second, he_(2,2)));
                triplets.push_back(T(2 * springs_[i].second, 2 * springs_[i].second+1, he_(2,3)));

                triplets.push_back(T(2 * springs_[i].second+1, 2 * springs_[i].first, he_(3,0)));
                triplets.push_back(T(2 * springs_[i].second+1, 2 * springs_[i].first+1, he_(3,1)));
                triplets.push_back(T(2 * springs_[i].second+1, 2 * springs_[i].second, he_(3,2)));
                triplets.push_back(T(2 * springs_[i].second+1, 2 * springs_[i].second+1, he_(3,3)));
            }

            _h.resize(n_unknowns(), n_unknowns());
            _h.setFromTriplets(triplets.begin(), triplets.end());
            //------------------------------------------------------//
        }

        // callback that is called after optimization finished
        inline virtual void handle_result(const Vec &_x) override {
            std::cout << "solution converged to x = " << std::endl << _x << std::endl;
        }

        // set initial start point
        void set_initial_x(const Vec& _x) {
            initial_x_ = _x;
        }


        void add_spring_element(const int _v_idx0, const int _v_idx1, const double _k = 1., const double _l = 1.) {
            if (2 * _v_idx0 > (int) n_ || _v_idx0 < 0 || 2 * _v_idx1 >= (int) n_ || _v_idx1 < 0)
                std::cout << "Warning: invalid spring element was added... " << _v_idx0 << " " << _v_idx1 << std::endl;
            else {
                springs_.push_back(Edge(_v_idx0, _v_idx1));
                ks_.push_back(_k);
                ls_.push_back(_l);
            }
        }

        void add_fixing_node_element(const int _v_idx, const double _penalty = 1., const double _px = 0., const double _py = 0.) {
            if (2 * _v_idx > (int) n_ || _v_idx < 0)
                std::cout << "Warning: invalid node constraint element was added... " << _v_idx << std::endl;
            else {
                fixed_node_indices_.push_back(_v_idx);
                penalties_.push_back(_penalty);
                fixed_points_.push_back(_px);
                fixed_points_.push_back(_py);
            }
        }


    private:
        int n_;
        std::vector<Edge> springs_;
        std::vector<int> fixed_node_indices_;

        FunctionBase& func_;
        FixingNodeElement fne_;

        //vector of constants
        std::vector<double> ks_;
        std::vector<double> ls_;
        std::vector<double> penalties_;
        std::vector<double> fixed_points_;

        // coordinates of each node
        Vec xe_;
        // gradient of each spring element
        Vec ge_;
        // hessian of each spring element
        Mat he_;

        // coordinates of each node
        Vec nc_xe_;
        // gradient of each node constraint element
        Vec nc_ge_;
        // hessian of each node constraint element
        Mat nc_he_;

        //initial x
        Vec initial_x_;
    };

//=============================================================================
}


