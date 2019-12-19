#include "ListGraph.h"

ListGraph::ListGraph(size_t _size) : amount_(_size) {
    map_.reserve(_size);
}

void ListGraph::AddVertex(Vertex vertex) {
    auto ret = map_.emplace(std::make_pair(vertex, std::vector<Vertex>()));
    ret.first->second.reserve(amount_);
}

std::vector<Vertex> ListGraph::GetNextVertices(const Vertex& vertex) const {
    auto it = map_.find(vertex);
    assert( it != map_.end() );
    return it->second;
}

void ListGraph::ConnectAll() {
    for (auto &it : map_) {
        for (const auto &it2 : map_) {
            Vertex tmp = it2.first;
            tmp.EstimateLength(it.first.x, it.first.y);
            it.second.push_back(tmp);
        }
    }
}

double ListGraph::SpanningTree() {
    /// Preparation
    std::vector<bool> visited(amount_, false);
    std::vector<double> distance(amount_, DBL_MAX);
    std::vector<size_t> prev_vertex(amount_, SIZE_T_MAX);
    std::priority_queue<Vertex, std::vector<Vertex>, std::greater<> > unvisited;

    /// First node
    Vertex source = map_.begin()->first;
    distance[source.id] = 0;
    unvisited.push(source);

    /// Minimal spanning tree construction
    Vertex current(0);
    while ( !unvisited.empty() ) {
        current = unvisited.top();
        unvisited.pop();
        visited[current.id] = true;

        for ( const auto& neighbour : GetNextVertices(current.id) ) {
            if ( !visited[neighbour.id] ) {
                double new_distance = neighbour.length;
                if ( new_distance < distance[neighbour.id] ) {
                    distance[neighbour.id] = new_distance;
                    prev_vertex[neighbour.id] = current.id;
                }
                if ( !visited[neighbour.id] ) {
                    unvisited.push(neighbour);
                }
            }
        }
    }

    assert(source.id == map_.at(current)[0].id);

    return std::accumulate(distance.begin(),
                                 distance.end(),
                                 map_.at(current)[0].length);
}

double ListGraph::EnumerateTrip() {
    std::vector<bool> visited(map_.size(), false);
    std::vector<double> distance;
    double result = DBL_MAX;
    DFS_(map_.begin()->first, visited, distance, result);
    return result;
}

void ListGraph::DFS_(const Vertex& current, std::vector<bool>& visited, std::vector<double>& distance, double& result_out) const {
    distance.push_back(current.length);
    visited[current.id] = true;

    for ( const auto& neigh : GetNextVertices(current) ) {
        if ( !visited[neigh.id] ) {
            DFS_(neigh, visited, distance, result_out);
        }
    }

    // TODO: find a way to replace this stupid if
    if ( distance.size() == amount_ ) {
        double tmp_dist = std::accumulate(distance.begin(), distance.end(), 0.0);
        tmp_dist += map_.at(current)[0].length; // first element in its vector is our starter
        result_out = std::min(result_out, tmp_dist);
    }
    distance.pop_back();
    visited[current.id] = false;
}
