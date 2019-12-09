#include <iostream>
#include <vector>

#include "ListGraph.h"

ListGraph::ListGraph(size_t _size) : list_(std::vector<std::vector<int> >(_size * 2)) {}

void ListGraph::AddEdge(int from, int to) {
    if ( from > list_.size() || to > list_.size() ) {
        list_.reserve( from > to ? from : to );
    }
    list_[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    int amount = 0;

    std::vector<bool> visited(list_.size(), false);
    for ( size_t iii = 0; iii < list_.size(); ++iii ) {
        for ( size_t jjj = 0; jjj < list_[iii].size(); ++jjj ) {
            if ( !visited[list_[iii][jjj]] ) {
                ++amount;
                visited[list_[iii][jjj]] = true;
            }
        }
    }

    return amount;
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
