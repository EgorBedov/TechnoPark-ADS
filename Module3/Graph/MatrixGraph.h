#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H


#include <vector>
#include <cstdlib>

#include "IGraph.h"

/// Матрица смежности
class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(size_t _edges);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::vector<bool> > matrix_;
};


#endif // MATRIXGRAPH_H
