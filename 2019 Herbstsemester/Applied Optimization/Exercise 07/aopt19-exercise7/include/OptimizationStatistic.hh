#pragma once

#include <iostream>
#include <iomanip>
#include "ProblemBase.hh"
#include "StopWatch.hh"

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class OptimizationStatistic : public ProblemBase {
    public:
        using Vec = ProblemBase::Vec;
        using Mat = ProblemBase::Mat;
        // sparse matrix type
        using SMat = ProblemBase::SMat;

        OptimizationStatistic(ProblemBase *_base) : base_(_base) {start_recording();}

        virtual int n_unknowns() override {
            return base_->n_unknowns();
        }

        virtual void initial_x(Vec &_x) override {
            base_->initial_x(_x);
        }

        virtual double eval_f(const Vec &_x) override {
            ++n_eval_f_;
            sw_.start();
            double f = base_->eval_f(_x);
            timing_eval_f_ += sw_.stop();

            return f;
        }

        virtual void eval_gradient(const Vec &_x, Vec &_g) override {
            ++n_eval_gradient_;
            sw_.start();
            base_->eval_gradient(_x, _g);
            timing_eval_gradient_ += sw_.stop();
        }

        virtual void eval_hessian(const Vec &_x, Mat &_H) override {
            ++n_eval_hessian_;
            sw_.start();
            base_->eval_hessian(_x, _H);
            timing_eval_hessian_ += sw_.stop();
        }

        virtual void eval_hessian(const Vec &_x, SMat &_H) override {
            ++n_eval_hessian_;
            sw_.start();
            base_->eval_hessian(_x, _H);
            timing_eval_hessian_ += sw_.stop();
        }

        virtual void handle_result(const Vec &_x) override {
            base_->handle_result(_x);
            print_statistics();
        }

        void start_recording() {
            swg_.start();

            timing_eval_f_ = 0.0;
            timing_eval_gradient_ = 0.0;
            timing_eval_hessian_ = 0.0;

            n_eval_f_ = 0;
            n_eval_gradient_ = 0;
            n_eval_hessian_ = 0;
        }

    protected:

        void print_statistics() {
            double time_total = swg_.stop();

            double time_np = timing_eval_f_ + timing_eval_gradient_ + timing_eval_hessian_;


            std::cerr << "######## Timing statistics ########" << std::endl;
            std::cerr << "total time    : " << time_total / 1000000.0 << "s\n";
            std::cerr << "total time evaluation : " << time_np / 1000000.0 << "s  (" << time_np / time_total * 100.0 << " %)\n";

            double timing_eval_f_avg = timing_eval_f_ / double(n_eval_f_);
            double timing_eval_gradient_avg = timing_eval_gradient_ / double(n_eval_gradient_);
            double timing_eval_hessian_avg = timing_eval_hessian_ / double(n_eval_hessian_);

            std::cerr << std::fixed << std::setprecision(5)
                      << "eval_f time   : " << timing_eval_f_ / 1000000.0
                      << "s  ( #evals: " << n_eval_f_ << " -> avg "
                      << timing_eval_f_avg / 1000000.0 << "s )\n"
                      << "eval_grad time: " << timing_eval_gradient_ / 1000000.0
                      << "s  ( #evals: " << n_eval_gradient_ << " -> avg "
                      << timing_eval_gradient_avg / 1000000.0 << "s, factor: "
                      << timing_eval_gradient_avg / timing_eval_f_avg << ")\n"
                      << "eval_hess time: " << timing_eval_hessian_ / 1000000.0
                      << "s  ( #evals: " << n_eval_hessian_ << " -> avg "
                      << timing_eval_hessian_avg / 1000000.0 << "s, factor: "
                      << timing_eval_hessian_avg / timing_eval_f_avg << ")\n";
        }

    private:
        ProblemBase *base_;
        AlgoHex::StopWatch<std::chrono::microseconds> swg_;
        AlgoHex::StopWatch<std::chrono::microseconds> sw_;

        // timings
        double timing_eval_f_;
        double timing_eval_gradient_;
        double timing_eval_hessian_;

        // number of function executions
        int n_eval_f_;
        int n_eval_gradient_;
        int n_eval_hessian_;
    };

//=============================================================================
}



