#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits.h>
#include <queue>
#include <sstream>
#include <vector>

/// Списки смежности
class ListGraph {
struct Node {
    int id;
    int weight;

    Node(int _id, int _weight) : id(_id), weight(_weight) {};

    bool operator>(const Node &r) const {
        return weight > r.weight;
    }

    bool operator==(const Node &r) const {
        return id == r.id;
    }
};
public:
    explicit ListGraph(size_t _size) : amount_of_vertices(_size) {
        list_.resize(_size);
        for ( size_t iii = 0; iii < amount_of_vertices; ++iii ) {
            list_[iii].push_back(Node(iii, 0));
        }
    }

    void AddEdge(int from, int to, int weight) {
        auto it = std::find(list_[from].begin(), list_[from].end(), Node(to, weight));
        if ( it == list_[from].end() ) {    // Если нет такого ребра
            list_[from].push_back(Node(to, weight));
            list_[to].push_back(Node(from, weight));
        } else if ( it->weight > weight ) { // Если есть такое ребро
            it.base()->weight = weight;     // list_[from][to]
            it = std::find(list_[to].begin(), list_[to].end(), Node(from, weight));
            assert( it != list_[to].end() );
            it.base()->weight = weight;     // list_[to][from]
        }
    }

    int VerticesCount() const {
        return amount_of_vertices;
    }

    std::vector<Node> GetNextVertices(int vertex) const {
        return list_[vertex];
    }

    int MeasureShortestPath(const int from, const int to) { // dijkstra algorithm
        std::vector<bool> visited(amount_of_vertices, false);

        std::vector<int> distance(amount_of_vertices, INT_MAX);
        distance[from] = 0;
        std::vector<int> prev_vertex(amount_of_vertices, -1);

        std::priority_queue<Node, std::vector<Node>, std::greater<Node> > unvisited;
        unvisited.push(Node(from, 0));

        while ( !unvisited.empty() ) {
            Node current = unvisited.top(); unvisited.pop();
            visited[current.id] = true;

            for ( const auto & neighbour : GetNextVertices(current.id) ) {
                if ( !visited[neighbour.id] ) {
                    int new_distance = distance[current.id] + neighbour.weight;
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

        return distance[to];
    }
private:
    std::vector<std::vector<Node> > list_;
    size_t amount_of_vertices = 0;
};

void run(std::istream &input, std::ostream &output) {
    size_t cities = 0;
    size_t roads = 0;
    input >> cities >> roads;
    assert(cities > 0);
    auto graph = ListGraph(cities);

    int from = 0;
    int to = 0;
    int weight;
    for ( size_t iii = 0; iii < roads; ++iii ) {
        input >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    input >> from >> to;
    output << graph.MeasureShortestPath(from, to) << std::endl;
}

void test_logic() {
    { // Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "6\n"
                 "9\n"
                 "0 3 1\n"
                 "0 4 2\n"
                 "1 2 7\n"
                 "1 3 2\n"
                 "1 4 3\n"
                 "1 5 3\n"
                 "2 5 3\n"
                 "3 4 4\n"
                 "3 5 6\n"
                 "0 2" << std::endl;

        run(input, output);

        assert(output.str() == "9\n");
    }
    { // ещё одно условие
        std::stringstream input;
        std::stringstream output;

        input << "7\n"
                 "11\n"
                 "0 1 2\n"
                 "0 2 4\n"
                 "1 3 3\n"
                 "1 4 10\n"
                 "2 3 2\n"
                 "2 5 5\n"
                 "3 4 1\n"
                 "3 5 8\n"
                 "3 6 4\n"
                 "4 6 6\n"
                 "5 6 1\n"
                 "0 2" << std::endl;

        run(input, output);

        assert(output.str() == "4\n");
    }
    { // Короткое условие
        std::stringstream input;
        std::stringstream output;

        input << "3\n"
                 "3\n"
                 "0 1 1\n"
                 "0 2 2\n"
                 "1 2 2\n"
                 "0 2" << std::endl;

        run(input, output);

        assert(output.str() == "2\n");
    }
    { // Очень короткое условие
        std::stringstream input;
        std::stringstream output;

        input << "1\n"
                 "1\n"
                 "0 0 5"
                 "0 0" << std::endl;

        run(input, output);

        assert(output.str() == "0\n");
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
    { // 2 условие из 5 задачи
        std::stringstream input;
        std::stringstream output;

        input << "5\n"
                 "10\n"
                 "3 2 3046\n"
                 "3 4 90110\n"  // longer
                 "4 0 57786\n"
                 "2 1 28280\n"
                 "3 2 18010\n"  // longer
                 "3 4 61367\n"
                 "3 0 18811\n"
                 "3 1 69898\n"
                 "2 4 72518\n"
                 "2 0 85838\n"
                 "4 1" << std::endl;

        run(input, output);

        assert(output.str() == "92693\n");
    }
    { // Условие с петлей
        std::stringstream input;
        std::stringstream output;

        input << "3\n"
                 "4\n"
                 "0 1 1\n"
                 "0 2 2\n"
                 "1 2 2\n"
                 "2 2 2"
                 "2 2" << std::endl;

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
