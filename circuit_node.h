#ifndef CIRCUIT_NODE_H
#define CIRCUIT_NODE_H

#include <vector>

class ElementNode;

class CircuitNode {
public:
    int id;
    std::vector<ElementNode*> connections;

    explicit CircuitNode(int id);
    void attach(ElementNode* term);
};

#endif