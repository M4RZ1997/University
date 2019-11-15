#pragma once

#include <ProblemBase.hh>
#include <FunctionQuadratic2D.hh>

namespace AOPT {

class TestProblem : public ProblemBase
{
public:
    TestProblem(double _gamma = 10.0)
    {
        m_function_ = FunctionQuadratic2D(_gamma);
    }
    // just delegate to the function, in this example function are problem
    // refer to the same thing, not in general
    inline int n_unknowns() override { return m_function_.n_unknowns(); }
    inline double eval_f(const Vec& _x) override {
        return m_function_.f(_x);
    }
    inline void eval_gradient(const Vec& _x, Vec& _g) override {
        m_function_.grad_f(_x, _g);
    }
    inline void eval_hessian(const Vec& _x, Mat& _H) override {
        m_function_.hess_f(_x, _H);
    }
    inline void eval_hessian(const Vec& _x, SMat& _H) override {
        // currently unused
    }
    void initial_x(Vec& _x) override {
        _x[0] = m_function_.get_gamma();
        _x[1] = 1.0;
    }
    void handle_result(const Vec& _x) override {
        std::cout << "after optimization x = " << _x.transpose() << std::endl;
    }
private:
    FunctionQuadratic2D m_function_;
};


}
