#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

/// Списки смежности
class ListGraph {
struct Node {
    int id;
    int weight;

    Node(int _id, int _weight) : id(_id), weight(_weight) {};

    bool operator<(const Node &r) const {
        return ( weight == r.weight ? id < r.id : weight < r.weight );
    }
};


public:
    explicit ListGraph(size_t _size) : amount_of_vertices(_size), list_(_size) {}
    virtual ~ListGraph() = default;

    void AddEdge(int from, int to, int weight) {
        list_[from].push_back(Node(to, weight));
        list_[to].push_back(Node(from, weight));
    }

    int VerticesCount() const {
        return amount_of_vertices;
    }

    std::vector<Node> GetNextVertices(int vertex) const {
        return list_[vertex];
    }

    int MeasureShortestPath(const int from, const int to) { // dijkstra algorithm
        std::vector<int> distance(amount_of_vertices, INT_MAX);
        distance[from] = 0;

        std::set<Node> q;
        q.emplace(from, 0);

        while ( !q.empty() ) {
            auto current = q.begin()->id; q.erase(q.begin());

            for ( const auto& neigh: GetNextVertices(current) ) {
                if ( distance[neigh.id] == INT_MAX ) {
                    distance[neigh.id] = distance[current] + neigh.weight;
                    q.emplace(neigh.id, distance[neigh.id]);
                } else if ( distance[neigh.id] > distance[current] + neigh.weight ) {
                    q.erase(Node(neigh.id, distance[neigh.id]));
                    distance[neigh.id] = distance[current] + neigh.weight;
                    q.emplace(neigh.id, distance[neigh.id]);
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
    ListGraph graph(cities);

    int from = 0;
    int to = 0;
    int weight = 0;
    for ( size_t iii = 0; iii < roads; ++iii ) {
        input >> from >> to >> weight;
        assert(weight >= 0);
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
    { // Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "3\n"
                 "3\n"
                 "0 1 1\n"
                 "0 2 3\n"
                 "1 2 1\n"
                 "0 2" << std::endl;

        run(input, output);

        assert(output.str() == "2\n");
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
                 "0 0 5\n"
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
                 "2 2 2\n"
                 "2 2" << std::endl;

        run(input, output);

        assert(output.str() == "0\n");
    }
    { // Странное условие
        std::stringstream input;
        std::stringstream output;

        input << "5\n"
                 "17\n"
                 "0 1 1\n"
                 "0 2 100000\n"
                 "0 3 100500\n"
                 "0 4 5\n"
                 "1 2 1\n"
                 "2 3 1\n"
                 "3 4 1\n"
                 "1 2 2\n"
                 "1 2 2\n"
                 "1 2 3\n"
                 "2 3 3\n"
                 "2 3 3\n"
                 "3 2 3\n"
                 "4 4 1000\n"
                 "0 0 100\n"
                 "0 0 100\n"
                 "0 4" << std::endl;

        run(input, output);

        assert(output.str() == "4\n");
    }
    std::cout << "OK" << std::endl;
}


int main() {
    test_logic();
//    run(std::cin, std::cout);
    return 0;
}
