#include "ArcGraph.h"

ArcGraph::ArcGraph(size_t _size) {
    arc_.reserve(_size);
}

ArcGraph::ArcGraph(const IGraph& g) : arc_(g.VerticesCount()) {
    for ( size_t iii = 0; iii < arc_.size(); ++iii ) {
        std::vector<int> next_vertices = g.GetNextVertices(iii);
        for ( const auto & next : next_vertices ) {
            AddEdge(iii, next);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    arc_.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    int amount = 0;

    std::vector<bool> visited(arc_.size(), false);
    for (const auto & pair : arc_) {
        if ( !visited[pair.first] ) {
            ++amount;
            visited[pair.first] = true;
        }
        if ( !visited[pair.second] ) {
            ++amount;
            visited[pair.second] = true;
        }
    }

    return amount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    auto result = std::vector<int>();

    for (const auto & iii : arc_) {
        if ( iii.first == vertex ) {
            result.push_back(iii.second);
        }
    }

    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    auto result = std::vector<int>();

    for (const auto & iii : arc_) {
        if ( iii.second == vertex ) {
            result.push_back(iii.first);
        }
    }

    return result;
};
