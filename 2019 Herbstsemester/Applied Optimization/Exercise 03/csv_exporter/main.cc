#include <CSVExporter.hh>
#include <FunctionNonConvex2D.hh>
#include <FunctionQuadratic2D.hh>

int main(int _argc, const char* _argv[]) {

    if(_argc != 9) {
        std::cout << "Usage: input should be 'output filename, function index(0: non-convex, 1: 2d quadratic), lower bound x, lower bound y, upper bound x, upper bound y, grid number in x, grid number in y.', e.g. "
                     "./csv_exporter /home/func1.csv 0 -10 -10 10 10 20 20"<< std::endl;
        return -1;
    }

    //read the input parameters
    int func_index, n_grid_x, n_grid_y;
    double x_l, x_u, y_l, y_u, dx, dy;
    func_index = atoi(_argv[2]);
    x_l = atof(_argv[3]);
    y_l = atof(_argv[4]);
    x_u = atof(_argv[5]);
    y_u = atof(_argv[6]);
    n_grid_x = atoi(_argv[7]);
    n_grid_y = atoi(_argv[8]);

    dx = (x_u - x_l)/n_grid_x;
    dy = (y_u - y_l)/n_grid_y;

    std::string filename(_argv[1]);

    //export data
    AOPT::CSVExporter csvexp;
    AOPT::CSVExporter::Mat mt(n_grid_x+1, n_grid_y+1);
    AOPT::CSVExporter::Vec vec_x(n_grid_x+1), vec_y(n_grid_y+1);

    if(func_index == 0 || func_index == 1) {
        for(int i = 0; i <= n_grid_x; ++i)
            vec_x[i] = x_l + dx*i;
        for (int j = 0; j <= n_grid_y; ++j)
            vec_y[j] = y_l + dy*j;

        AOPT::FunctionNonConvex2D fnc2d;
        AOPT::FunctionQuadratic2D fq2d(-1.);
        AOPT::FunctionQuadratic2D::Vec x(2);
        if(func_index == 0) {
            for(int i = 0; i <= n_grid_x; ++i)
                for (int j = 0; j <= n_grid_y; ++j) {
                    x[0] = vec_x[i];
                    x[1] = vec_y[j];
                    mt(i,j) = fnc2d.f(x);
                }
        } else {
            for(int i = 0; i <= n_grid_x; ++i)
                for (int j = 0; j <= n_grid_y; ++j) {
                    x[0] = vec_x[i];
                    x[1] = vec_y[j];
                    mt(i,j) = fq2d.f(x);
                }
        }
    } else {
        std::cout << "Error: Function index is from 0 to 1." << std::endl;
        return -1;
    }


    csvexp.export_function2d(mt, vec_x, vec_y, filename);

    return 0;
}
