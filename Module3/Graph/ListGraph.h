#ifndef LISTGRAPH_H
#define LISTGRAPH_H


#include <cstdlib>
#include <vector>

#include "IGraph.h"

/// Списки смежности
class ListGraph : public IGraph {
public:
    explicit ListGraph(size_t _size);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::vector<int> > list_;
};


#endif // LISTGRAPH_H
