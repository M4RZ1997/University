#include <StopWatch.hh>
#include <iostream>
#include <LBFGS.hh>
#include <OptimizationStatistic.hh>
#include <MassSpringSystem.hh>

int main(int _argc, const char* _argv[]) {
    if(_argc != 7) {
        std::cout << "Usage: input should be 'function index(0: f without length, 1: f with length), "
                     "number of grid in x, number of grid in y, m, max iteration, filename', e.g. "
                     "./LBFGS 1 20 20 10 10000 /usr/spring" << std::endl;
        return -1;
    }

    //read the input parameters
    int func_index, n_grid_x, n_grid_y, m, max_iter;
    func_index = atoi(_argv[1]);
    n_grid_x = atoi(_argv[2]);
    n_grid_y = atoi(_argv[3]);
    m = atoi(_argv[4]);
    max_iter = atoi(_argv[5]);

    std::string filename(_argv[6]);

    //initial energy
    AOPT::MassSpringSystem mss(n_grid_x, n_grid_y, func_index);
    auto energy = mss.initial_system_energy();
    std::cout<<"\nInitial MassSpring system energy is "<<energy<<std::endl;

    std::cout<<"Saving initial spring graph to "<<filename<<"_*.csv"<<std::endl;
    mss.save_spring_system(filename.c_str());

    filename += "_opt";

    auto opt_st = std::make_unique<AOPT::OptimizationStatistic>(mss.get_problem().get());
    AOPT::LBFGS lbfgs(m);
    AOPT::LBFGS::Vec x = lbfgs.solve(opt_st.get(), 1e-4, max_iter);

    mss.set_spring_graph_vertices(x);
    std::cout<<"Saving optimized spring graph to "<<filename<<"_*.csv"<<std::endl;
    mss.save_spring_system(filename.c_str());

    return 0;
}

