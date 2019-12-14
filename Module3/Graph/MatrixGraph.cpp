#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(size_t _size) : matrix_(std::vector<std::vector<bool> >(_size)) {
    for (auto & iii : matrix_) {
        iii = std::vector<bool> (_size, false);
    }
}

MatrixGraph::MatrixGraph(const IGraph& g) : matrix_(g.VerticesCount()) {
    for ( size_t iii = 0; iii < matrix_.size(); ++iii ) {
        std::vector<int> next_vertices = g.GetNextVertices(iii);
        for ( const auto & next : next_vertices ) {
            AddEdge(iii, next);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    matrix_[from][to] = true;
//    matrix_[to][from] = true;
}

int MatrixGraph::VerticesCount() const {
    int amount = 0;

    std::vector<bool> visited(matrix_.size(), false);

    for ( size_t iii = 0; iii < matrix_.size(); ++iii ) {
        for ( size_t jjj = 0; jjj < matrix_[iii].size(); ++jjj ) {
            if ( matrix_[iii][jjj] ) {
                if ( !visited[iii] ) {
                    ++amount;
                    visited[iii] = true;
                }
                if ( !visited[jjj] ) {
                    ++amount;
                    visited[jjj] = true;
                }
            }
        }
    }

    return amount;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    auto result = std::vector<int>();

    for ( size_t iii = 0; iii < matrix_[vertex].size(); ++iii ) {
        if ( matrix_[vertex][iii] ) {
            result.push_back(iii);
        }
    }

    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    auto result = std::vector<int>();

    for ( size_t iii = 0; iii < matrix_.size(); ++iii ) {
        if ( matrix_[iii][vertex] ) {
            result.push_back(iii);
        }
    }

    return result;
}
