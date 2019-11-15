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
                while (iter < 1000) {
                    for (size_t i = 0; i < n_vertices; ++i)
                        sg_.set_vertex(i, rng_.get_random_nd_vector(2));

                    for (size_t i = 0; i < n_vertices; ++i) {
                        points[2 * i] = sg_.point(i)[0];
                        points[2 * i + 1] = sg_.point(i)[1];
                    }

                    msp.eval_hessian(points, h);
                    Eigen::VectorXcd evalues = h.eigenvalues();
                    for (size_t i = 0; i < evalues.size(); ++i) {
//                        std::cerr << evalues[i] << "\n";
                        if (evalues[i].real() < -1e-13) {
                            std::cout << "\nThe function is not convex! One Negative eigenvalue: " << evalues[i]<<std::endl;
                            return false;
                        }
                    }

                    iter++;
                }
                std::cout << "\nProbably it is a convex function!" << std::endl;
                //------------------------------------------------------//
            } else if(_sparsity_type == SPARSE) {
                //------------------------------------------------------//
                MassSpringProblem2DSparse msps(se, n_unknowns);
                for(size_t i=0; i<sg_.n_edges(); ++i)
                    msps.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));


                DerivativeChecker npd;
                npd.check_all(msps);

                //Todo: check the convexity of the function in SpringElement2D.hh
                //Hint: see if all the eigenvalues of the hessian matrix (Sparse) are >=0
                //This is the sparse version and the eigenvalues can be calculated with Spectra library
                int ncv = n_unknowns > 20 ? 20 : n_unknowns;
                while (iter < 1000) {
                    for (size_t i = 0; i < n_vertices; ++i)
                        sg_.set_vertex(i, rng_.get_random_nd_vector(2));

                    for (size_t i = 0; i < n_vertices; ++i) {
                        points[2 * i] = sg_.point(i)[0];
                        points[2 * i + 1] = sg_.point(i)[1];
                    }

                    msps.eval_hessian(points, sh);
                    // Construct matrix operation object using the wrapper class SymEigsSolver
                    Spectra::SparseSymMatProd<double> op(sh);

                    // Construct eigen solver object, requesting the SMALLEST_ALGE eigenvalue
                    Spectra::SymEigsSolver< double, Spectra::SMALLEST_ALGE, Spectra::SparseSymMatProd<double> > eigs(&op, 1, ncv);

                    // Initialize and compute
                    eigs.init();
                    int nconv = eigs.compute();

                    // Retrieve results
                    Eigen::VectorXcd evalues;
                    if(eigs.info() == Spectra::SUCCESSFUL)
                        evalues = eigs.eigenvalues();
//                    std::cout << "Eigenvalues found:\n" << evalues << std::endl;

                    for (size_t i = 0; i < evalues.size(); ++i)
                        if (evalues[i].real() < -1e-13) {
                            std::cout << "\nThe function is not convex! Smallest eigen value" <<evalues[i]<< std::endl;
                            return false;
                        }
                    iter++;
                }
                std::cout << "\nProbably it is a convex function!" << std::endl;
                //------------------------------------------------------//

            }
        } else if(_spring_element_type == WITH_LENGTH){
            SpringElement2DWithLength se2;
            if(_sparsity_type == DENSE) {
                //------------------------------------------------------//
                //Todo: check the convexity of the function in SpringElement2DWithLength.hh
                //Hint: see if all the eigenvalues of the hessian matrix (Dense) are >=0
                MassSpringProblem2D msp(se2, n_unknowns);
                for(size_t i=0; i<sg_.n_edges(); ++i)
                    msp.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

                while (iter < 1000) {
                    for(size_t i=0; i<n_vertices; ++i)
                        sg_.set_vertex(i, rng_.get_random_nd_vector(2));

                    for(size_t i=0; i<n_vertices; ++i) {
                        points[2*i] = sg_.point(i)[0];
                        points[2*i+1] = sg_.point(i)[1];
                    }

                    msp.eval_hessian(points, h);
                    Eigen::VectorXcd evalues = h.eigenvalues();
                    for (size_t i = 0; i < evalues.size(); ++i) {
//                        std::cerr << evalues[i] << "\n";
                        if (evalues[i].real() < -1e-13) {
                            std::cout << "\nThe function is not convex! One Negative eigenvalue: " << evalues[i]<<std::endl;
                            return false;
                        }
                    }

                    iter++;
                }
                std::cout << "\nProbably it is a convex function!" << std::endl;
                //------------------------------------------------------//
            } else if(_sparsity_type == SPARSE) {
                MassSpringProblem2DSparse msps(se2, n_unknowns);
                for(size_t i=0; i<sg_.n_edges(); ++i)
                    msps.add_spring_element(sg_.edge(i).first, sg_.edge(i).second, sg_.coefficient(i), sg_.length(i));

                DerivativeChecker npd;
                npd.check_all(msps, 1e-5, 1e-2);

                //Todo: check the convexity of the function in SpringElement2DWithLength.hh
                //Hint: see if all the eigenvalues of the hessian matrix (Sparse) are >=0
                //This is the sparse version and the eigenvalues can be calculated with Spectra library
                int ncv = n_unknowns > 20 ? 20 : n_unknowns;
                while (iter < 1000) {
                    for (size_t i = 0; i < n_vertices; ++i)
                        sg_.set_vertex(i, rng_.get_random_nd_vector(2));

                    for (size_t i = 0; i < n_vertices; ++i) {
                        points[2 * i] = sg_.point(i)[0];
                        points[2 * i + 1] = sg_.point(i)[1];
                    }

                    msps.eval_hessian(points, sh);
                    // Construct matrix operation object using the wrapper class SymEigsSolver
                    Spectra::SparseSymMatProd<double> op(sh);

                    // Construct eigen solver object, requesting the SMALLEST_ALGE eigenvalue
                    Spectra::SymEigsSolver< double, Spectra::SMALLEST_ALGE, Spectra::SparseSymMatProd<double> > eigs(&op, 1, ncv);

                    // Initialize and compute
                    eigs.init();
                    int nconv = eigs.compute();

                    // Retrieve results
                    Eigen::VectorXcd evalues;
                    if(eigs.info() == Spectra::SUCCESSFUL)
                        evalues = eigs.eigenvalues();
//                    std::cout << "Eigenvalues found:\n" << evalues << std::endl;

                    for (size_t i = 0; i < evalues.size(); ++i)
                        if (evalues[i].real() < -1e-13) {
                            std::cout << "\nThe function is not convex! Smallest eigenvalue: " <<evalues[i]<<std::endl;
                            return false;
                        }
                    iter++;
                }
            }
            std::cout<<"\nProbably it is a convex function!"<<std::endl;
            //------------------------------------------------------//
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
        //then, add edges

        //add vertices
        for(int j = 0; j <= n_grid_y_; ++j)
            for (int i = 0; i <= n_grid_x_; ++i)
                sg_.add_vertex(rng_.get_random_nd_vector(2));

        //add edges
        for(int j = 0; j < n_grid_y_; ++j) {
            for(int i = 0; i < n_grid_x_; ++i) {
                //horizontal edge
                sg_.add_edge(get_grid_index(i, j), get_grid_index(i+1, j), 1., 1.);
                //vertical edge
                sg_.add_edge(get_grid_index(i, j), get_grid_index(i, j+1), 1., 1.);
                //diagonal edge
                sg_.add_edge(get_grid_index(i, j), get_grid_index(i+1, j+1), 1., sqrt(2.));
                //diagonal edge
                sg_.add_edge(get_grid_index(i+1, j), get_grid_index(i, j+1), 1., sqrt(2.));
            }
        }

        //add right most
        for(int j = 0; j < n_grid_y_; ++j)
            sg_.add_edge(get_grid_index(n_grid_x_, j), get_grid_index(n_grid_x_, j+1), 1., 1.);

        //add top cap
        for(int i = 0; i < n_grid_x_; ++i)
            sg_.add_edge(get_grid_index(i, n_grid_y_), get_grid_index(i+1, n_grid_y_), 1., 1.);
        //------------------------------------------------------//


//        std::cout<<"\nPoints: ";
//        for(auto point : sg_.points())
//            std::cout<<point[0]<<" "<<point[1]<<"\n";
//        std::cout<<"\nEdges: ";
//        for(auto edge : sg_.edges())
//            std::cout<<edge.first<<" "<<edge.second<<"\n";
    }

    int MassSpringSystem::get_grid_index(const int _i, const int _j) const {
        assert(_i<=n_grid_x_ && _j<=n_grid_y_);
        return (n_grid_x_+1)*_j + _i;
    }
}