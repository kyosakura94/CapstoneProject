#ifndef ASTARPATHFINDING_H
#define ASTARPATHFINDING_H

#include "node.h";
#include "Graph.h"
#include <iostream>
#include <algorithm> 
#include <vector>
#include <random>
#include <cmath>
#include <cstdlib>


using namespace std;
class AStarPathFinding
{
public:
	AStarPathFinding(Graph<Node> grid_);
	~AStarPathFinding();

    std::vector<vec3> FindPath(vec3 startWorldPosition, vec3 endWorldPosition);
    std::vector<vec3> FindPath(GameObject * first, GameObject * last);
    std::vector<Node *> FindPath(int startX, int startY, int endX, int endY);
    std::vector<Node *> FindPath(Node* startNode_, Node* endNode_);
	int CalculateDistanceCost(Node* a, Node* b);
	Node * GetLowestFCostNode(vector<Node*> pathNodeList);
	std::vector<Node*> GetNeighbourList(Node* currentNode);
	std::vector<Node*> CalculatePath(Node* endNode);
	Node* GetNode(int x, int y);
	void Reset();

private:
	int MOVE_STRAIGHT_COST = 10;
	int MOVE_DIAGONAL_COST = 14;

	Graph<Node> grid;
	
	std::vector<Node *> openlist;
	std::vector<Node *> closedList;
	std::vector<Node *> finalpath;

	GameObject* lastnote;

};
#endif // !1



