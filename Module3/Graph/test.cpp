#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#include "ArcGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"

enum GraphType {
    ARC,
    LIST,
    MATRIX,
    SET,
    ALL
};

template <typename T>
void run(std::istream &input, std::ostream &output) {
    size_t vertices = 0;
    size_t edges = 0;
    input >> vertices >> edges;
    assert(vertices > 0);
    auto graph = T(vertices);

    int from = 0;
    int to = 0;
    for ( size_t iii = 0; iii < edges; ++iii ) {
        input >> from >> to;
        graph.AddEdge(from, to);
    }

    output << graph.VerticesCount() << std::endl;

    auto next_vertices = graph.GetNextVertices(1);
    for ( const auto & element : next_vertices ) {
        output << element << " ";
    }
    output << std::endl;

    auto prev_vertices = graph.GetPrevVertices(0);
    for ( const auto & element : prev_vertices ) {
        output << element << " ";
    }
    output << std::endl;
}

template <typename T>
void run_amount(std::istream &input, std::ostream &output) {
    size_t vertices = 0;
    size_t edges = 0;
    input >> vertices >> edges;
    assert(vertices > 0);
    auto graph = T(vertices);

    int from = 0;
    int to = 0;
    for ( size_t iii = 0; iii < edges; ++iii ) {
        input >> from >> to;
        graph.AddEdge(from, to);
    }

    output << graph.VerticesCount() << std::endl;
}

void test_graph(const GraphType g) {
        {
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
                 "8 9" << std::endl;

        switch (g) {
            case ARC:
                run_amount<ArcGraph>(input, output);
                break;
            case LIST:
                run_amount<ListGraph>(input, output);
                break;
            case MATRIX:
                run_amount<MatrixGraph>(input, output);
                break;
            case SET:
                run_amount<SetGraph>(input, output);
                break;
            default:
                exit(EXIT_FAILURE);
        }
        assert(output.str() == "10\n" );
    }
    {
        std::stringstream input;
        std::stringstream output;
        std::stringstream answer;

        input << "4\n"
                 "5\n"
                 "0 1\n"
                 "1 2\n"
                 "1 0\n"
                 "1 3\n"
                 "3 0" << std::endl;

        switch (g) {
            case ARC:
                run<ArcGraph>(input, output);
                answer << "4\n2 0 3 \n1 3 " << std::endl;
                break;
            case LIST:
                run<ListGraph>(input, output);
                answer << "4\n2 0 3 \n1 3 " << std::endl;
                break;
            case MATRIX:
                run<MatrixGraph>(input, output);
                answer << "4\n0 2 3 \n1 3 " << std::endl;
                break;
            case SET:
                run<SetGraph>(input, output);
                answer << "4\n2 0 3 \n3 1 " << std::endl;
                break;
            default:
                exit(EXIT_FAILURE);
        }

        assert(output.str() == answer.str());
    }
    std::cout << "OK" << std::endl;
}

int main() {
    test_graph(LIST);
    test_graph(MATRIX);
    test_graph(SET);
    test_graph(ARC);

    return 0;
}
