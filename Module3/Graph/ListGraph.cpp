#include <iostream>
#include <vector>

#include "ListGraph.h"

ListGraph::ListGraph(size_t _size) {
    list_.resize(_size);
}

ListGraph::ListGraph(const IGraph& g) : list_(g.VerticesCount()) {
    for ( size_t iii = 0; iii < list_.size(); ++iii ) {
        std::vector<int> next_vertices = g.GetNextVertices(iii);
        for ( const auto & next : next_vertices ) {
            AddEdge(iii, next);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    list_[from].push_back(to);
//    list_[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return list_.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    return list_[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    auto result = std::vector<int>();

    for ( size_t iii = 0; iii < list_.size(); ++iii ) {
        for ( size_t jjj = 0; jjj < list_[iii].size(); ++jjj ) {
            if ( list_[iii][jjj] == vertex ) {
                result.push_back(iii);
                break;
            }
        }
    }

    return result;
}
