#pragma once

#include <vector>
#include <Eigen/Dense>

//== NAMESPACES ===============================================================

namespace AOPT {

//== CLASS DEFINITION =========================================================

    class SpringGraph {
    public:
        using Point = Eigen::Vector2d;
        using Edge = std::pair<int, int>;

        SpringGraph(){}
        ~SpringGraph(){}

        void add_vertex(const Point& _pt) {
            points_.push_back(_pt);
        }

        void set_vertex(const int _v_idx, const Point& _pt) {
            points_[_v_idx] = _pt;
        }

        void add_edge(const int _vt0, const int _vt1, const double _k = 1., const double _l = 1.) {
            edges_.push_back(Edge(_vt0, _vt1));
            coeffs_.push_back(_k);
            lengths_.push_back(_l);
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



    private:
        std::vector<Edge> edges_;
        std::vector<Point> points_;
        std::vector<double> coeffs_;
        std::vector<double> lengths_;
    };

//=============================================================================
}


