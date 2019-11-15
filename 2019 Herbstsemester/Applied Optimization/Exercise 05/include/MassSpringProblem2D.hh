#pragma once

#include "ProblemBase.hh"
#include "FunctionBase.hh"
#include "NodeConstraintElement.hh"

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

            nc_xe_.resize(nce_.n_unknowns());
            nc_ge_.resize(nce_.n_unknowns());
            nc_he_.resize(nce_.n_unknowns(), nce_.n_unknowns());
        }

        ~MassSpringProblem2D() {}

        virtual int n_unknowns() override {
            return n_;
        }

        virtual void initial_x(Vec &_x) override {
            // set initial solution
            for (unsigned int i = 0; i < n_; ++i)
                _x[i] = 0.;
        }


        virtual double eval_f(const Vec &_x) override {
            double energy(0);

            //------------------------------------------------------//
            //TODO: assemble function values of all the elements (springs (done) + node constraints)
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
            for(size_t i=0; i<constrained_nodes_.size(); i++){
                nc_xe_[0] = _x[2*constrained_nodes_[i]]; // first
                nc_xe_[1] = _x[2*constrained_nodes_[i+1]];//second

                coeff[0] = penalties_[i];
                coeff[1] = fixed_points_[i];//first
                coeff[2] = fixed_points_[i+1];//secodn

                energy += nce_.f(nc_xe_, coeff);
            }

            //------------------------------------------------------//

            return energy;
        }

        virtual void eval_gradient(const Vec &_x, Vec &_g) override {
            _g.resize(n_unknowns());
            _g.setZero();

            //------------------------------------------------------//
            //TODO: assemble local gradient of all the elements
            // (springs (done) + node constraints) to the global gradient
            Vec coeff(2);
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
            for(size_t i=0; i<constrained_nodes_.size(); i++){
                nc_xe_[0] = _x[2*constrained_nodes_[i]];//first
                nc_xe_[1] = _x[2*constrained_nodes_[i+1]];//second

                coeff[0] = penalties_[i];
                coeff[1] = fixed_points_[i];//first
                coeff[2] = fixed_points_[i+1];//second

                nce_.grad_f(nc_xe_, coeff, nc_ge_);
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


        void add_spring_element(const int _v_idx0, const int _v_idx1, const double _k = 1., const double _l = 1.) {
            if (2 * _v_idx0 > (int) n_ || _v_idx0 < 0 || 2 * _v_idx1 >= (int) n_ || _v_idx1 < 0)
                std::cout << "Warning: invalid spring element was added... " << _v_idx0 << " " << _v_idx1 << std::endl;
            else {
                springs_.push_back(Edge(_v_idx0, _v_idx1));
                ks_.push_back(_k);
                ls_.push_back(_l);
            }
        }

        void add_node_constraint_element(const int _v_idx, const double _penalty = 1., const double _px = 0., const double _py = 0.) {
            if (2 * _v_idx > (int) n_ || _v_idx < 0)
                std::cout << "Warning: invalid node constraint element was added... " << _v_idx << std::endl;
            else {
                constrained_nodes_.push_back(_v_idx);
                penalties_.push_back(_penalty);
                fixed_points_.push_back(_px);
                fixed_points_.push_back(_py);
            }
        }


    private:
        int n_;
        std::vector<Edge> springs_;
        std::vector<int> constrained_nodes_;

        FunctionBase& func_;
        NodeConstraintElement nce_;

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
    };

//=============================================================================
}


