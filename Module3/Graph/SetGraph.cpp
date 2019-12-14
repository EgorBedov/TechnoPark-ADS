#include <map>

#include "SetGraph.h"

SetGraph::SetGraph(size_t _size) {
    map_.reserve(_size);
}

SetGraph::SetGraph(const IGraph& g) : map_(g.VerticesCount()) {
    for ( size_t iii = 0; iii < map_.size(); ++iii ) {
        std::vector<int> next_vertices = g.GetNextVertices(iii);
        for ( const auto & next : next_vertices ) {
            AddEdge(iii, next);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    auto it = map_.find(from);
    if ( it == map_.end() ) {
        map_.emplace(from, std::vector<int>(1, to));
    } else {
        it->second.push_back(to);
    }
    it = map_.find(to);
    if ( it == map_.end() ) {
        map_.emplace(to, std::vector<int>());
    }
}

int SetGraph::VerticesCount() const {
    return map_.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    auto it = map_.find(vertex);
    assert(it != map_.end());
    return it->second;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    auto result = std::vector<int>();

    for (const auto & it : map_) {
        for ( const auto & neighbour : it.second ) {
            if ( neighbour == vertex ) {
                result.push_back(it.first);
                break;
            }
        }
    }

    return result;
}


