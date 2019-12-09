#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(size_t _edges) : matrix_(std::vector<std::vector<bool> >(_edges * 2)) {
    for (auto & iii : matrix_) {
        iii = std::vector<bool> (_edges * 2, false);
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    if ( from > matrix_.size() || to > matrix_.size() ) {
        int max = from > to ? from : to;
        matrix_.reserve( max );
        for ( auto & row : matrix_ ) {
            row.reserve( max );
        }
    }
    matrix_[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    int amount = 0;

    std::vector<bool> visited(matrix_.size(), false);

    // TODO: 0 is redundant if there are no vertices with that number
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
