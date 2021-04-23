#ifndef NODE_H
#define NODE_H

#include "Graph.h"
#include "../../Rendering/3D/GameObject.h"

class Node 
{
public:

    Graph<Node> * grid;
    int x;
    int y;

    int gCost;
    int hCost;
    int fCost;

    bool isWalkable;
    Node * cameFromNode;

    GameObject* obj;

    Node(Graph<Node> * grid_, int x_, int y_)
    {
        grid = grid_;
        x = x_;
        y = y_;
        isWalkable = true;
    }

    Node();

    ~Node();

    void CalculateFCost() {
        fCost = gCost + hCost;
    }

    void SetIsWalkable(bool isWalkable_) {
        isWalkable = isWalkable_;
    }

    void setObjRef(GameObject * obj_) {
        obj = obj_;
    }
    
    GameObject * getobjRef() 
    {
        return obj;
    }
    void print();
};

#endif /* NODE_H */
