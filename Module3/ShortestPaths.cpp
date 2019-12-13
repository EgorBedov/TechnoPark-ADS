#include <limits.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

/// Матрица смежности
class ListGraph {
public:
    explicit ListGraph(size_t _size) : amount_of_vertices_(_size) {
        list_.resize(_size);
    }

    void AddEdge(int from, int to) {
        list_[from].push_back(to);
        list_[to].push_back(from);
    }

    size_t CountShortestPaths(int from, int to) {
        std::vector<bool> visited(amount_of_vertices_, false);
        std::vector<int> distance(amount_of_vertices_, INT_MAX);
        std::vector<int> paths(amount_of_vertices_, 0);
        std::queue<int> q;
        q.push(from);
        paths[from] = 1;
        visited[from] = true;

        while ( !q.empty() ) {
            int current = q.front(); q.pop();
            for ( const auto & neighbour : GetNextVertices(current) ) {
                /// Push new neighbour in queue
                if ( !visited[neighbour] ) {
                    q.push(neighbour);
                    visited[neighbour] = true;
                }
                /// Check for better path
                if ( distance[neighbour] > distance[current] + 1 ) {
                    distance[neighbour] = distance[current] + 1;
                    paths[neighbour] = paths[current];
                } else if ( distance[neighbour] == distance[current] + 1 ) {
                    paths[neighbour] += paths[current];
                }
            }
        }
        return paths[to];
    }

    std::vector<int> GetNextVertices(int vertex) const {
        return list_[vertex];
    }
private:
    std::vector<std::vector<int> > list_;
    size_t amount_of_vertices_ = 0;
};

void run(std::istream &input, std::ostream &output) {
    size_t vertices = 0;
    size_t edges = 0;
    input >> vertices >> edges;
    assert(vertices > 0);
    auto graph = ListGraph(vertices);

    int from = 0;
    int to = 0;
    for ( size_t iii = 0; iii < edges; ++iii ) {
        input >> from >> to;
        graph.AddEdge(from, to);
    }

    input >> from >> to;
    if ( edges == 0 ) {
        output << 0 << std::endl;
    } else {
        output << graph.CountShortestPaths(from, to) << std::endl;
    }
}

void test_logic() {
    { // Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "4\n"
                 "5\n"
                 "0 1\n"
                 "0 2\n"
                 "1 2\n"
                 "1 3\n"
                 "2 3\n"
                 "\n"
                 "0 3" << std::endl;

        run(input, output);

        assert(output.str() == "2\n");
    }
    { // У исходной ноды сразу есть связь с искомым
        std::stringstream input;
        std::stringstream output;

        input << "5\n"
                 "7\n"
                 "0 1\n"
                 "0 2\n"
                 "0 3\n"
                 "0 4\n"
                 "1 3\n"
                 "1 4\n"
                 "2 3\n"
                 "\n"
                 "0 4" << std::endl;

        run(input, output);

        assert(output.str() == "1\n");
    }
    { // Сложная схема
        std::stringstream input;
        std::stringstream output;

        input << "10\n"
                 "18\n"
                 "0 1\n"
                 "0 2\n"
                 "0 3\n"
                 "0 4\n"
                 "1 5\n"
                 "1 6\n"
                 "2 5\n"
                 "2 6\n"
                 "2 7\n"
                 "3 6\n"
                 "3 7\n"
                 "3 8\n"
                 "4 7\n"
                 "4 8\n"
                 "5 9\n"
                 "6 9\n"
                 "7 9\n"
                 "8 9\n"
                 "\n"
                 "0 9" << std::endl;

        run(input, output);

        assert(output.str() == "10\n");
    }
    { // Условие без дорог
        std::stringstream input;
        std::stringstream output;

        input << "10\n"
                 "0\n"
                 "0 0" << std::endl;

        run(input, output);

        assert(output.str() == "0\n");
    }
    std::cout << "OK" << std::endl;
}


int main() {
    test_logic();
//    run(std::cin, std::cout);
    return 0;
}
