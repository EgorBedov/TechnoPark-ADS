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
            it.base()->weight = weight;
        }
    }

    int VerticesCount() const {
        return amount_of_vertices;
    }

    std::vector<Node> GetNextVertices(int vertex) const {
        return list_[vertex];
    }
    size_t MeasureSpanningTree() {  // Prim's algorithm
        std::vector<bool> visited(amount_of_vertices, false);

        std::vector<int> distance(amount_of_vertices, 2147483647);  // int max
        distance[0] = 0;
        std::vector<int> prev_vertex(amount_of_vertices, -1);

        std::priority_queue<Node, std::vector<Node>, std::greater<Node> > unvisited;
        unvisited.push(Node(0, 0)); // first node

        while ( !unvisited.empty() ) {
            Node current = unvisited.top();
            unvisited.pop();
            visited[current.id] = true;

            for ( const auto & neighbour : GetNextVertices(current.id) ) {
                if ( !visited[neighbour.id] ) {
                    int new_distance = neighbour.weight;
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
        return std::accumulate(distance.begin(), distance.end(), 0);
    }
private:
    std::vector<std::vector<Node> > list_;
    size_t amount_of_vertices = 0;
};

void run(std::istream &input, std::ostream &output) {
    size_t vertices = 0;
    size_t edges = 0;
    input >> vertices >> edges;
    assert(vertices > 0);
    auto graph = ListGraph(vertices);

    int from = 0;
    int to = 0;
    int weight;
    for ( size_t iii = 0; iii < edges; ++iii ) {
        input >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);    // they start indexing with 1
    }

    output << graph.MeasureSpanningTree() << std::endl;
}

void test_logic() {
    { // 1 условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "4 4\n"
                 "1 2 1\n"
                 "2 3 2\n"
                 "3 4 5\n"
                 "4 1 4" << std::endl;

        run(input, output);

        assert(output.str() == "7\n");
    }
    { // 2 условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "5 10\n"
                 "4 3 3046\n"
                 "4 5 90110\n"
                 "5 1 57786\n"
                 "3 2 28280\n"
                 "4 3 18010\n"
                 "4 5 61367\n"
                 "4 1 18811\n"
                 "4 2 69898\n"
                 "3 5 72518\n"
                 "3 1 85838" << std::endl;

        run(input, output);

        assert(output.str() == "107923\n");
        // 5 <--57786--> 1 <--18811--> 4 <--3046--> 3 <--28280--> 2
    }
    std::cout << "OK" << std::endl;
}


int main() {
    test_logic();
//    run(std::cin, std::cout);
    return 0;
}
