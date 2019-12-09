#include <cassert>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

/// Матрица смежности
class MatrixGraph {
public:
    explicit MatrixGraph(size_t _vertices)
      : matrix_(std::vector<std::vector<bool> >(_vertices)),
        amount_of_vertices_(_vertices)
      {
        for (auto &iii : matrix_) {
            iii = std::vector<bool>(_vertices, false);
        }
    }

    void AddEdge(int from, int to) {
        if ( from > matrix_.size() || to > matrix_.size() ) {
            int max = from > to ? from : to;
            matrix_.reserve( max );
            for ( auto & row : matrix_ ) {
                row.reserve( max );
            }
        }
        matrix_[from][to] = true;
        matrix_[to][from] = true;
    }

    size_t CountShortestPaths(int from, int to) {
        std::queue<int> q;
        q.push(from);

        std::vector<int> neighbours;
        std::vector<bool> visited(amount_of_vertices_, false);
        while ( !q.empty() ) {  // цикл итерации по всем элементам
            size_t entries = 0;
            size_t layer_width = q.size();
            for ( size_t iii = 0; iii < layer_width; ++iii ) { // цикл итерации по слою
                int current = q.front();
                visited[current] = true;
                neighbours = GetNextVertices(current);
                q.pop();
                for ( const auto & tmp: neighbours ) {
                    if ( !visited[tmp] ) {
                        if (tmp == to) {
                            ++entries;
                        }
                        q.push(tmp);    // we don't really need to push if we found "to"
                    }
                }
            }
            if ( entries ) {
                return entries;
            }
        }
    }

    std::vector<int> GetNextVertices(int vertex) const {
        auto result = std::vector<int>();

        for ( size_t iii = 0; iii < matrix_[vertex].size(); ++iii ) {
            if ( matrix_[vertex][iii] ) {
                result.push_back(iii);
            }
        }

        return result;
    }
private:
    std::vector<std::vector<bool> > matrix_;
    size_t amount_of_vertices_ = 0;
};

void run(std::istream &input, std::ostream &output) {
    size_t vertices = 0;
    size_t edges = 0;
    input >> vertices >> edges;
    assert((vertices & edges) > 0);
    auto graph = MatrixGraph(vertices);

    int from = 0;
    int to = 0;
    for ( size_t iii = 0; iii < edges; ++iii ) {
        input >> from >> to;
        graph.AddEdge(from, to);
    }

    input >> from >> to;
    output << graph.CountShortestPaths(from, to) << std::endl;
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
    std::cout << "OK" << std::endl;
}


int main() {
    test_logic();
//    run(std::cin, std::cout);
    return 0;
}
