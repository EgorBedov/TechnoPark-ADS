#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
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
        std::priority_queue<Node, std::vector<Node>, std::greater<Node> > unvisited;

        std::vector<int> distance(amount_of_vertices, 2147483647);  // int max
        distance[0] = 0;
        std::vector<int> prev_vertex(amount_of_vertices);

        unvisited.push(Node(from, 0));

        while ( !unvisited.empty() ) {
            Node current = unvisited.top();
            unvisited.pop();
            visited[current.id] = true;

            std::vector<Node> neighbours = GetNextVertices(current.id);
            for ( const auto & neighbour : neighbours ) {
                if ( !visited[neighbour.id] ) {
                    int new_distance = distance[current.id] + neighbour.weight;
                    if (new_distance < distance[neighbour.id]) {
                        distance[neighbour.id] = new_distance;
                        prev_vertex[neighbour.id] = current.id;
                    }
                    if (!visited[neighbour.id]) {
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
    std::cout << "OK" << std::endl;
}


int main() {
    test_logic();
//    run(std::cin, std::cout);
    return 0;
}
