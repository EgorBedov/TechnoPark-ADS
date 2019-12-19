#ifndef HW_LISTGRAPH_H
#define HW_LISTGRAPH_H


#include <cfloat>  // for DBL_MAX
#include <cmath>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>


struct Vertex {
    size_t id;
    double x;
    double y;
    double length = 0;

    Vertex(size_t _id, double _x = 0, double _y = 0)
      : id(_id),
        x(_x),
        y(_y) {}

    bool operator==(const Vertex& other) const {
        return id == other.id;
    }

    bool operator!=(const Vertex& other) const {
        return id != other.id;
    }

    bool operator>(const Vertex& other) const {
        return length > other.length;
    }

    bool operator<(const Vertex& other) const {
        return length < other.length;
    }

    void EstimateLength(double other_x, double other_y) {
        length = sqrt(
                pow( abs(other_x - x), 2 )
                +
                pow( abs(other_y - y), 2 ));
    }

    struct Hasher {
        size_t operator()(const Vertex& v) const {
            return std::hash<size_t>()(v.id);
        }
    };
};

/// Списки смежности
class ListGraph {
public:
    explicit ListGraph(size_t _size);

    void AddVertex(Vertex vertex);
    void ConnectAll();

    std::vector<Vertex> GetNextVertices(const Vertex& vertex) const;
    double SpanningTree();
    double EnumerateTrip();

private:
    void DFS_(const Vertex& current, std::vector<bool>& visited, std::vector<double>& distance, double& result_out) const;
    void Hamilton(size_t current,
                  const std::vector<size_t>& prev_vertex,
                  const std::vector<double>& distance,
                  std::vector<size_t>& trip_out,
                  size_t counter = 1);

    std::unordered_map<Vertex, std::vector<Vertex>, Vertex::Hasher> map_;
    size_t amount_;
};


#endif // HW_LISTGRAPH_H
