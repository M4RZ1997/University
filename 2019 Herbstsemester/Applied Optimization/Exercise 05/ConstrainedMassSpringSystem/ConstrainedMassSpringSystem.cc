#include "ConstrainedMassSpringSystem.hh"

namespace AOPT {
    double ConstrainedMassSpringSystem::initial_system_energy() const{
        if(msp_.get() == nullptr)
            return -1;

        Vec points(n_unknowns_);
        int n_vertices = sg_.n_vertices();

        for(size_t i=0; i<n_vertices; ++i) {
            points[2*i] = sg_.point(i)[0];
            points[2*i+1] = sg_.point(i)[1];
        }

        return msp_.get()->eval_f(points);
    }


    void ConstrainedMassSpringSystem::set_spring_graph_vertices(const Vec& _points) {
        int n_vertices = sg_.n_vertices();

        for(size_t i=0; i<n_vertices; ++i)
            sg_.set_vertex(i, Point(_points[2*i], _points[2*i+1]));
    }


    void ConstrainedMassSpringSystem::save_spring_system(const char *_filename) const {
        sg_.save_to_files(_filename);
    }


    std::shared_ptr<AOPT::MassSpringProblem2D> ConstrainedMassSpringSystem::get_problem() const {
        return msp_;
    }


    void ConstrainedMassSpringSystem::setup_problem(const int _spring_element_type) {
        //initialize the problem pointer
        if(_spring_element_type == WITH_LENGTH){
            msp_ = std::make_shared<MassSpringProblem2D>(sewl_, n_unknowns_);
        } else if(_spring_element_type == WITHOUT_LENGTH){
            msp_ = std::make_shared<MassSpringProblem2D>(se_, n_unknowns_);
        } else {
            std::cout<<"Error: spring function index should be 0 or 1!"<<std::endl;
            return;
        }

        //------------------------------------------------------//
        //Todo: add spring elements and node constraint elements to the problem
        //hint: use msp_.get()-> to call the methods to add elements

        //------------------------------------------------------//
    }


    void ConstrainedMassSpringSystem::setup_spring_graph() {
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


        //set unknown variable number
        n_unknowns_ = 2*sg_.n_vertices();
    }


    int ConstrainedMassSpringSystem::get_grid_index(const int _i, const int _j) const {
        assert(_i<=n_grid_x_ && _j<=n_grid_y_);
        return (n_grid_x_+1)*_j + _i;
    }
}