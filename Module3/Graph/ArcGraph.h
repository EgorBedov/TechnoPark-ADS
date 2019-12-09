#ifndef ARCGRAPH_H
#define ARCGRAPH_H


#include <cstdlib>
#include <vector>

#include "IGraph.h"

/// Массив пар {from, to}
class ArcGraph : public IGraph {
public:
    explicit ArcGraph(size_t _edges);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::pair<int, int> > arc_;
};


#endif // ARCGRAPH_H
