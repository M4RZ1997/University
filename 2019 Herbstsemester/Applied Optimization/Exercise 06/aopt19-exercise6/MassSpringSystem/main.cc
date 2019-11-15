#include "MassSpringSystem.hh"
#include <StopWatch.hh>
#include <iostream>


int main(int _argc, const char* _argv[]) {
    if(_argc != 5) {
        std::cout << "Usage: input should be 'function index(0: f without length, 1: f with length), sparse hessian (0: dense, 1: sparse), number of grid in x, number of grid in y', e.g. "
                     "./MassSpringSystem 1 1 10 10" << std::endl;
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
    AOPT::MassSpringSystem mss(n_grid_x, n_grid_y, func_index);
    auto energy = mss.initial_system_energy();
    std::cout<<"Initial MassSpring system energy is "<<energy<<std::endl;

    AlgoHex::StopWatch<> sw;
    sw.start();
    //do exhaustive search
    mss.is_convex(sparse);

    std::cout<<"Convex search takes: "<<sw.stop()/1000.<<"s"<< std::endl;

    return 0;
}