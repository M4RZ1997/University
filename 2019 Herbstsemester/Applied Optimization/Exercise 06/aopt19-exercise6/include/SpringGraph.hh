#pragma once

#include <vector>
#include <Eigen/Dense>

#include <fstream>

//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================

    class SpringGraph {
    public:
        using Point = Eigen::Vector2d;
        using Edge = std::pair<int, int>;

        SpringGraph(){}
        ~SpringGraph(){}

        int add_vertex(const Point& _pt) {
            points_.push_back(_pt);
            return static_cast<int>(points_.size() - 1);
        }

        void set_vertex(const int _v_idx, const Point& _pt) {
            points_[_v_idx] = _pt;
        }

        int add_edge(const int _vt0, const int _vt1, const double _k = 1., const double _l = 1.) {
            edges_.push_back(Edge(_vt0, _vt1));
            coeffs_.push_back(_k);
            lengths_.push_back(_l);
            return static_cast<int>(edges_.size() - 1);
        }

        int from_vertex(const int _edge_idx) const {
            assert(_edge_idx < edges_.size());
            return edges_[_edge_idx].first;
        }

        int to_vertex(const int _edge_idx) const {
            assert(_edge_idx < edges_.size());
            return edges_[_edge_idx].second;
        }

        Point point(const int _vt) const {
            assert(_vt < points_.size());
            return points_[_vt];
        }

        Edge edge(int _i) const {
            return edges_[_i];
        }

        double coefficient(const int _edge_idx) const {
            assert(_edge_idx < edges_.size());
            return coeffs_[_edge_idx];
        }

        double length(const int _edge_idx) const {
            assert(_edge_idx < edges_.size());
            return lengths_[_edge_idx];
        }

        size_t n_vertices() const {
            return points_.size();
        }

        size_t n_edges() const {
            return edges_.size();
        }

        const std::vector<Point>& points() const {
            return points_;
        }

        const std::vector<Edge>& edges() const {
            return edges_;
        }

        void save_to_files(const char *prefix) const {
            std::ofstream f_nodes(std::string(prefix) + "_nodes.csv");
            for (const auto &p: points_) {
                f_nodes << p[0] << "," << p[1] << "\n";
            }
            std::ofstream f_edges(std::string(prefix) + "_edges.csv");
            size_t idx = 0;
            for (const auto &e: edges_) {
                f_edges << e.first << "," << e.second << "," << lengths_[idx++] << "\n";
            }
        }



    private:
        std::vector<Edge> edges_;
        std::vector<Point> points_;
        std::vector<double> coeffs_;
        std::vector<double> lengths_;
    };

//=============================================================================
}


