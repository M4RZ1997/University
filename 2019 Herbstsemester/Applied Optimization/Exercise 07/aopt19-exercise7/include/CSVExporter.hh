#pragma once

#include <iostream>
#include <fstream>
#include <Eigen/Dense>

//== NAMESPACES ===============================================================

namespace AOPT {

    //== CLASS DEFINITION =========================================================

    class CSVExporter {
    public:
        // (dense) vector type
        typedef Eigen::VectorXd Vec;
        // (dense) matrix type
        typedef Eigen::MatrixXd Mat;

        CSVExporter() {}
        ~CSVExporter() {}

    public:
        void export_function2d(const Mat& _mt, const Vec& _x, const Vec& _y, const std::string& _filename) {
            std::ofstream f_write(_filename);
            //write x
            for(size_t i=0 ; i<_x.size(); ++i)
                f_write<<","<<_x[i];

            f_write << std::endl;

            for(size_t j = 0 ; j < _y.size(); ++j) {
                f_write<<_y[j];
                for (int i = 0; i < _x.size(); ++i) {
                    f_write<<","<<_mt(i,j);
                }
                f_write << std::endl;
            }

            f_write.close();
        }
    };

//=============================================================================
}



