#include <StopWatch.hh>
#include <iostream>
#include <MassSpringSystem.hh>
#include <RandomNumberGenerator.hh>

#include <Eigen/Eigenvalues>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <DerivativeChecker.hh>

using namespace AOPT;

bool is_convex(AOPT::ProblemBase *_problem, AOPT::SpringGraph& _sg, const int _sparsity_type) {
    if(_problem == nullptr)
        return false;


    RandomNumberGenerator rng(-1., 1.);

    int n_unknowns = _problem->n_unknowns();
    MassSpringSystem::Vec points(_problem->n_unknowns());
    MassSpringSystem::Mat h(n_unknowns, n_unknowns);
    MassSpringSystem::SMat sh(n_unknowns, n_unknowns);

    int n_vertices = _sg.n_vertices();
    int iter = 0;

    if(_sparsity_type == MassSpringSystem::DENSE) {
        //------------------------------------------------------//
        //Todo: check the convexity of the function in SpringElement2D.hh
        //Hint: randomly set the coordinates of the vertices,
        //see if all the eigenvalues of the hessian matrix (Dense) are >=0
        while (iter < 1000) {
            for (size_t i = 0; i < n_vertices; ++i)
                _sg.set_vertex(i, rng.get_random_nd_vector(2));

            for (size_t i = 0; i < n_vertices; ++i) {
                points[2 * i] = _sg.point(i)[0];
                points[2 * i + 1] = _sg.point(i)[1];
            }

            _problem->eval_hessian(points, h);
            Eigen::VectorXcd evalues = h.eigenvalues();
            for (size_t i = 0; i < evalues.size(); ++i) {
//                        std::cerr << evalues[i] << "\n";
                if (evalues[i].real() < -1e-13) {
                    std::cout << "The function is not convex! One Negative eigenvalue: " << evalues[i]<<std::endl;
                    return false;
                }
            }

            iter++;
        }
        std::cout << "Probably it is a convex function!" << std::endl;

        return true;
        //------------------------------------------------------//
    } else if(_sparsity_type == MassSpringSystem::SPARSE) {
        //------------------------------------------------------//
        DerivativeChecker npd;
        npd.check_all(*_problem);

        //Todo: check the convexity of the function in SpringElement2D.hh
        //Hint: see if all the eigenvalues of the hessian matrix (Sparse) are >=0
        //This is the sparse version and the eigenvalues can be calculated with Spectra library
        int ncv = n_unknowns > 20 ? 20 : n_unknowns;
        while (iter < 1000) {
            for (size_t i = 0; i < n_vertices; ++i)
                _sg.set_vertex(i, rng.get_random_nd_vector(2));

            for (size_t i = 0; i < n_vertices; ++i) {
                points[2 * i] = _sg.point(i)[0];
                points[2 * i + 1] = _sg.point(i)[1];
            }

            _problem->eval_hessian(points, sh);
            // Construct matrix operation object using the wrapper class SymEigsSolver
            Spectra::SparseSymMatProd<double> op(sh);

            // Construct eigen solver object, requesting the SMALLEST_ALGE eigenvalue
            Spectra::SymEigsSolver<double, Spectra::SMALLEST_ALGE, Spectra::SparseSymMatProd<double> > eigs(&op, 1, ncv);

            // Initialize and compute
            eigs.init();
            int nconv = eigs.compute();

            // Retrieve results
            Eigen::VectorXcd evalues;
            if (eigs.info() == Spectra::SUCCESSFUL)
                evalues = eigs.eigenvalues();
//                    std::cout << "Eigenvalues found:\n" << evalues << std::endl;

            for (size_t i = 0; i < evalues.size(); ++i)
                if (evalues[i].real() < -1e-13) {
                    std::cout << "The function is not convex! Smallest eigen value" << evalues[i] << std::endl;
                    return false;
                }
            iter++;
        }

        std::cout << "Probably it is a convex function!" << std::endl;
        return true;
        //------------------------------------------------------//

    }

    return false;
}


int main(int _argc, const char* _argv[]) {
    if(_argc != 5) {
        std::cout << "Usage: input should be 'function index(0: f without length, 1: f with length), "
                     "sparse hessian (0: dense, 1: sparse), number of grid in x, number of grid in y', e.g. "
                     "./ConvexTests2 1 1 10 10" << std::endl;
        return -1;
    }

    //read the input parameters
    int func_index, sparse, n_grid_x, n_grid_y;
    double dx, dy;
    func_index = atoi(_argv[1]);
    sparse = atoi(_argv[2]);
    n_grid_x = atoi(_argv[3]);
    n_grid_y = atoi(_argv[4]);

    //initial energy
    MassSpringSystem mss(n_grid_x, n_grid_y, func_index);
    auto energy = mss.initial_system_energy();
    std::cout<<"Initial MassSpring system energy is "<<energy<<std::endl;

    AlgoHex::StopWatch<> sw;
    sw.start();
    //do exhaustive search
    is_convex(mss.get_problem().get(), mss.get_spring_graph(), sparse);

    std::cout<<"Convex search takes: "<<sw.stop()/1000.<<"s"<< std::endl;

    return 0;
}

