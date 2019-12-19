#include "gtest/gtest.h"

#include "Graph/ListGraph.h"
#include "Calculator/Calculator.h"

TEST(ListGraph, GetNextVertices) {
    size_t N = 2;
    Calculator calc(N);
    ListGraph graph(N);

    Vertex v0(0, 0.0, 0.0);
    Vertex v1(1, 3.0, 4.0);

    graph.AddVertex(v0);
    graph.AddVertex(v1);

    graph.ConnectAll();

    auto v0_neighbours = graph.GetNextVertices(v0);
    auto v1_neighbours = graph.GetNextVertices(v1);
    EXPECT_EQ(N, v0_neighbours.size());
    EXPECT_EQ(N, v1_neighbours.size());
    EXPECT_EQ(v1.id, v0_neighbours[0].id);
    EXPECT_EQ(v0.id, v0_neighbours[1].id);
}

TEST(ListGraph, ConnectAll) {
    size_t N = 2;
    Calculator calc(N);
    ListGraph graph(N);

    Vertex v0(0, 0.0, 0.0);
    Vertex v1(1, 3.0, 4.0);

    graph.AddVertex(v0);
    graph.AddVertex(v1);

    graph.ConnectAll();

    auto v0_neighbours = graph.GetNextVertices(v0);
    auto v1_neighbours = graph.GetNextVertices(v1);

    EXPECT_DOUBLE_EQ(5.0, v0_neighbours[0].length);
    EXPECT_DOUBLE_EQ(5.0, v1_neighbours[1].length);
}

TEST(ListGraph, Distances) {
    size_t N = 2;
    Calculator calc(N);
    ListGraph graph(N);

    graph.AddVertex({0, 0.0, 0.0});
    graph.AddVertex({1, 3.0, 4.0});

    graph.ConnectAll();

    EXPECT_DOUBLE_EQ(10.0, graph.SpanningTree());
    EXPECT_DOUBLE_EQ(10.0, graph.EnumerateTrip());
}

TEST(ListGraph, Square) {
    size_t N = 4;
    Calculator calc(N);
    ListGraph graph(N);

    graph.AddVertex({0, 5.0, 5.0});
    graph.AddVertex({1, -5.0, 5.0});
    graph.AddVertex({2, -5.0, -5.0});
    graph.AddVertex({3, 5.0, -5.0});

    graph.ConnectAll();

    EXPECT_LE(40.0, graph.SpanningTree());
    EXPECT_DOUBLE_EQ(40.0, graph.EnumerateTrip());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
