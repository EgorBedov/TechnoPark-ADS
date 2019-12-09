#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#include "IGraph.h"
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
    size_t amount = 0;
    input >> amount;
    assert(amount > 0 && "Amount of edges is 0 or less\n");
    auto graph = T(amount);

    int from = 0;
    int to = 0;
    for ( size_t iii = 0; iii < amount; ++iii ) {
        input >> from >> to;
        graph.AddEdge(from, to);
    }

    output << graph.VerticesCount() << std::endl;

    auto next_vertices = graph.GetNextVertices(2);
    for ( const auto & element : next_vertices ) {
        output << element << " ";
    }
    output << std::endl;

    auto prev_vertices = graph.GetPrevVertices(1);
    for ( const auto & element : prev_vertices ) {
        output << element << " ";
    }
    output << std::endl;
}

void test_graph(const GraphType g) {
    {
        std::stringstream input;
        std::stringstream output;
        std::stringstream answer;

        input << "5\n"
                 "1 2\n"
                 "2 3\n"
                 "2 1\n"
                 "2 4\n"
                 "4 1" << std::endl;

        switch (g) {
            case ARC:
                run<ArcGraph>(input, output);
                answer << "4\n3 1 4 \n2 4 " << std::endl;
                break;
            case LIST:
                run<ListGraph>(input, output);
                answer << "4\n3 1 4 \n2 4 " << std::endl;
                break;
            case MATRIX:
                run<MatrixGraph>(input, output);
                answer << "4\n1 3 4 \n2 4 " << std::endl;
                break;
            case SET:
                run<SetGraph>(input, output);
                answer << "4\n3 1 4 \n4 2 " << std::endl;
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
}
