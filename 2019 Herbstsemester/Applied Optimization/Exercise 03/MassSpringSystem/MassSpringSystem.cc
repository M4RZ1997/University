#include <SpringElement2DWithLength.hh>
#include "MassSpringSystem.hh"

namespace AOPT {

    bool MassSpringSystem::is_convex(const int _spring_element_type, const int _sparsity_type) {
        auto n_vertices = sg_.n_vertices();
        auto n_unknowns = 2*n_vertices;

        Vec points(n_unknowns);
        Mat h(n_unknowns, n_unknowns);
        SMat sh(n_unknowns, n_unknowns);

        int iter = 0;

        if(_spring_element_type == WITHOUT_LENGTH) {
            SpringElement2D se;
            if(_sparsity_type == DENSE) {
                MassSpringProblem2D msp(se, n_unknowns);
                for(size_t i=0; i<sg_.n_edges(); ++i)
                    msp.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

                //------------------------------------------------------//
                //Todo: check the convexity of the function in SpringElement2D.hh
                //Hint: randomly set the coordinates of the vertices,
                //see if all the eigenvalues of the hessian matrix (Dense) are >=0
                FunctionBase::Vec doublePoint = rng_.get_random_nd_vector(4);
                se.hess_f(doublePoint,h);
                Eigen::VectorXcd e;
                e = h.eigenvalues();
                /*
                for (int i = 0; i < e.size(); i++){
                    if (e(i) < 0.0) {

                        return false;
                    }
                }*/
                return true;


                //------------------------------------------------------//
            } else if(_sparsity_type == SPARSE) {
                //------------------------------------------------------//
                MassSpringProblem2DSparse msps(se, n_unknowns);
                for(size_t i=0; i<sg_.n_edges(); ++i)
                    msps.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

                //check the gradient and hessian
                DerivativeChecker npd;
                npd.check_all(msps);

                //Todo: check the convexity of the function in SpringElement2D.hh
                //Hint: see if all the eigenvalues of the hessian matrix (Sparse) are >=0
                //This is the sparse version and the eigenvalues can be calculated with Spectra library
                //------------------------------------------------------//
                FunctionBase::Vec doublePoint = rng_.get_random_nd_vector(4);
                se.hess_f(doublePoint,h);
                Eigen::VectorXcd e;
                /*
                e = SymEigSolver.h;
                for (int i = 0; i < e.size(); i++){
                    if (e[i] < 0){
                        return false;
                    }
                }*/
                return true;

            }
        } else if(_spring_element_type == WITH_LENGTH){
            SpringElement2DWithLength se2;
            if(_sparsity_type == DENSE) {
                //------------------------------------------------------//
                //Todo: check the convexity of the function in SpringElement2DWithLength.hh
                //Hint: see if all the eigenvalues of the hessian matrix (Dense) are >=0
                //------------------------------------------------------//
                FunctionBase::Vec doublePoint = rng_.get_random_nd_vector(4);
                se2.hess_f(doublePoint,h);
                Eigen::VectorXcd e;
                e = h.eigenvalues();
                /*
                for (int i = 0; i < e.size(); i++){
                    if (e[i] < 0){
                        return false;
                    }
                }*/
                return true;
            } else if(_sparsity_type == SPARSE) {
                MassSpringProblem2DSparse msps(se2, n_unknowns);
                for(size_t i=0; i<sg_.n_edges(); ++i)
                    msps.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

                //check the gradient and hessian
                DerivativeChecker npd;
                npd.check_all(msps, 1e-5, 1e-2);

                //Todo: check the convexity of the function in SpringElement2DWithLength.hh
                //Hint: see if all the eigenvalues of the hessian matrix (Sparse) are >=0
                //This is the sparse version and the eigenvalues can be calculated with Spectra library
                //------------------------------------------------------//
                FunctionBase::Vec doublePoint = rng_.get_random_nd_vector(4);
                se2.hess_f(doublePoint,h);
                Eigen::VectorXcd e;
                /*
                e = SymEigSolver;
                for (int i = 0; i < e.size(); i++){
                    if (e[i] < 0){
                        return false;
                    }
                }*/
                return true;
            }
        }
        return false;
    }

    double MassSpringSystem::initial_system_energy(const int _spring_element_type, const int _sparsity_type) {
        auto n_vertices = sg_.n_vertices();
        auto n_unknowns = 2*n_vertices;

        Vec points(2*n_vertices);
        for(size_t i=0; i<n_vertices; ++i) {
            points[2*i] = sg_.point(i)[0];
            points[2*i+1] = sg_.point(i)[1];
        }


        if(_spring_element_type == WITHOUT_LENGTH) {
            SpringElement2D se;
            MassSpringProblem2D msp(se, n_unknowns);
            for(size_t i=0; i<sg_.n_edges(); ++i)
                msp.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

            return msp.eval_f(points);

        } else if(_spring_element_type == WITH_LENGTH){
            SpringElement2DWithLength se2;
            MassSpringProblem2D msp(se2, n_unknowns);
            for(size_t i=0; i<sg_.n_edges(); ++i)
                msp.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

            return msp.eval_f(points);
        }

        return -1.;
    }

    void MassSpringSystem::setup_spring_graph() {
        //------------------------------------------------------//
        //Todo: implement the function to setup the spring graph
        //hint: use the functions in SpringGraph.hh
        //first, add vertices
        for(int j = 0; j <= n_grid_y_; j++){
            for(int i = 0; i <= n_grid_x_; i++){
                Point point;
                point << i,j;
                sg_.add_vertex(point);
            }
        }
        //then, add edges
        for(int j = 0; j <= n_grid_y_; j++){
            for(int i = 0; i <= n_grid_x_; i++){
                int grid_index_ = get_grid_index(i, j);
                //Lower-Left Corner
                if(grid_index_ == 0){
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j));
                    sg_.add_edge(grid_index_ , get_grid_index(i,j+1));
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j+1), 1, sqrt(2));
                }
                //Lower-Right Corner
                else if(grid_index_ == n_grid_x_){
                    sg_.add_edge(grid_index_ , get_grid_index(i-1,j+1), 1, sqrt(2));
                    sg_.add_edge(grid_index_ , get_grid_index(i,j+1));
                }
                //Upper-Left Corner
                else if(grid_index_ == (n_grid_x_+1)*n_grid_y_){
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j));
                }
                //Upper-Right Corner
                else if(grid_index_ == (n_grid_x_+1)*n_grid_y_ + n_grid_x_){
                    i=i;
                }
                //Lower Border
                else if(grid_index_ < n_grid_x_){
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j));
                    sg_.add_edge(grid_index_ , get_grid_index(i-1,j+1), 1, sqrt(2));
                    sg_.add_edge(grid_index_ , get_grid_index(i,j+1));
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j+1), 1, sqrt(2));
                }
                //Upper Border
                else if(grid_index_ > (n_grid_x_+1)*n_grid_y_){
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j));
                }
                //Left Border
                else if(grid_index_%(n_grid_x_+1) == 0){
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j));
                    sg_.add_edge(grid_index_ , get_grid_index(i,j+1));
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j+1), 1, sqrt(2));
                }
                //Right Border
                else if((grid_index_-n_grid_x_)%(n_grid_x_+1) == 0){
                    sg_.add_edge(grid_index_ , get_grid_index(i-1,j+1), 1, sqrt(2));
                    sg_.add_edge(grid_index_ , get_grid_index(i,j+1));
                }
                //All Inner Points
                else{
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j));
                    sg_.add_edge(grid_index_ , get_grid_index(i-1,j+1), 1, sqrt(2));
                    sg_.add_edge(grid_index_ , get_grid_index(i,j+1));
                    sg_.add_edge(grid_index_ , get_grid_index(i+1,j+1), 1, sqrt(2));
                }
            }
        }

        //------------------------------------------------------//
    }

    int MassSpringSystem::get_grid_index(const int _i, const int _j) const {
        assert(_i<=n_grid_x_ && _j<=n_grid_y_);
        return (n_grid_x_+1)*_j + _i;
    }
}