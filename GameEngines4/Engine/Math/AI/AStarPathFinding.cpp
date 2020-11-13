#include "AStarPathFinding.h"

AStarPathFinding::AStarPathFinding(Graph<Node> grid_)
{
	grid = grid_;
}

std::vector<vec3> AStarPathFinding::FindPath(vec3 startWorldPosition, vec3 endWorldPosition)
{
	int startX = grid.GetX(startWorldPosition);
	int startY = grid.GetY(startWorldPosition);
	int endX = grid.GetX(endWorldPosition);
	int endY = grid.GetY(endWorldPosition);


	std::vector<Node*> path = FindPath(startX, startY, endX, endY);

	if (path.size() == 0)
	{
		return {};
	}
	else {

		std::vector<vec3> vectorPath;

		vec3 one(1, 1, 1);
		for (auto pathNode : path)
		{
			vec3 pathvec(pathNode->x, pathNode->y, 0);

			pathvec = pathvec * grid.GetCellSize() + one * grid.GetCellSize() * 0.5f;

			vectorPath.push_back(pathvec);
		}
	}

	return std::vector<vec3>();
}

std::vector<vec3> AStarPathFinding::FindPath(GameObject* first, GameObject* last)
{
	Node* startNode = grid.GetGridObject(0, 0);

	//if (startNode == nullptr)
	//{
	//	startNode = grid.GetGridObject(0,0);
	//}
	//Node * startNode = grid.GetGridObject(first);
	Node * endNode = grid.GetGridObject(last);


	std::vector<Node*> path = FindPath(startNode, endNode);

	if (path.size() == 0)
	{
		return {};
	}
	else {

		std::vector<vec3> vectorPath;

		vec3 one(0.5, 1.0f, 0.5);
		for (auto pathNode : path)
		{
			vec3 pathvec = pathNode->getobjRef()->GetPosition();

			pathvec = pathvec + one;

			vectorPath.push_back(pathvec);
		}

		return vectorPath;
	}

	return std::vector<vec3>();
}

std::vector<Node*> AStarPathFinding::FindPath(int startX, int startY, int endX, int endY)
{
	Node* startNode = grid.GetGridObject(startX, startY);
	Node* endNode = grid.GetGridObject(endX, endY);

	if (startNode == nullptr || endNode == nullptr) {
		// Invalid Path
		return {};
	}

	openlist.push_back(startNode);

	for (int x = 0; x < grid.GetWidth(); x++) {
		for (int y = 0; y < grid.GetHeight(); y++) {
			Node* pathNode = grid.GetGridObject(x, y);
			pathNode->gCost = 99999999;
			pathNode->CalculateFCost();
			pathNode->cameFromNode = nullptr;
		}
	}

	startNode->gCost = 0;
	startNode->hCost = CalculateDistanceCost(startNode, endNode);
	startNode->CalculateFCost();


	while (openlist.size() > 0)
	{
		Node* currentNode = GetLowestFCostNode(openlist);

		if (currentNode == endNode)
		{
			return CalculatePath(endNode);
		}
		
		auto it = std::find(openlist.begin(), openlist.end(), currentNode);
		if (it != openlist.end()) { openlist.erase(it); }


		closedList.push_back(currentNode);

		for (auto neighbourNode : GetNeighbourList(currentNode))
		{
			if (std::find(closedList.begin(), closedList.end(), neighbourNode) != closedList.end())
			{
				continue;
			}

			if (!neighbourNode->isWalkable) 
			{
				closedList.push_back(neighbourNode);
				continue;
			}

			int tentativeGCost = currentNode->gCost + CalculateDistanceCost(currentNode, neighbourNode);

			if (tentativeGCost < neighbourNode->gCost) 
			{
				neighbourNode->cameFromNode = currentNode;
				neighbourNode->gCost = tentativeGCost;
				neighbourNode->hCost = CalculateDistanceCost(neighbourNode, endNode);
				neighbourNode->CalculateFCost();


				if (std::find(openlist.begin(), openlist.end(), neighbourNode) == openlist.end())
				{
					openlist.push_back(neighbourNode);
				}
			}
		}
	}

	return {};
}

std::vector<Node*> AStarPathFinding::FindPath(Node* startNode_, Node* endNode_)
{
	Node* startNode = startNode_;
	Node* endNode = endNode_;

	if (startNode == nullptr || endNode == nullptr) {
		// Invalid Path
		return {};
	}

	openlist.push_back(startNode);

	for (int x = 0; x < grid.GetWidth(); x++) {
		for (int y = 0; y < grid.GetHeight(); y++) {
			Node* pathNode = grid.GetGridObject(x, y);
			pathNode->gCost = 99999999;
			pathNode->CalculateFCost();
			pathNode->cameFromNode = nullptr;
		}
	}

	startNode->gCost = 0;
	startNode->hCost = CalculateDistanceCost(startNode, endNode);
	startNode->CalculateFCost();


	while (openlist.size() > 0)
	{
		Node* currentNode = GetLowestFCostNode(openlist);

		if (currentNode == endNode)
		{
			return CalculatePath(endNode);
		}
		
		auto it = std::find(openlist.begin(), openlist.end(), currentNode);
		//pop out
		if (it != openlist.end()) { openlist.erase(it); }


		closedList.push_back(currentNode);

		for (auto neighbourNode : GetNeighbourList(currentNode))
		{
			if (std::find(closedList.begin(), closedList.end(), neighbourNode) != closedList.end())
			{
				continue;
			}

			if (!neighbourNode->isWalkable) 
			{
				closedList.push_back(neighbourNode);
				continue;
			}

			int tentativeGCost = currentNode->gCost + CalculateDistanceCost(currentNode, neighbourNode);

			if (tentativeGCost < neighbourNode->gCost) 
			{
				neighbourNode->cameFromNode = currentNode;
				neighbourNode->gCost = tentativeGCost;
				neighbourNode->hCost = CalculateDistanceCost(neighbourNode, endNode);
				neighbourNode->CalculateFCost();


				if (std::find(openlist.begin(), openlist.end(), neighbourNode) == openlist.end())
				{
					openlist.push_back(neighbourNode);
				}
			}
		}
	}

	return {};
}

int AStarPathFinding::CalculateDistanceCost(Node* a, Node* b)
{
	int xDistance = abs(a->x - b->x);
	int yDistance = abs(a->y - b->y);
	int remaining = abs(xDistance - yDistance);

	return MOVE_DIAGONAL_COST * std::min(xDistance, yDistance) + MOVE_STRAIGHT_COST * remaining;
}

Node* AStarPathFinding::GetLowestFCostNode(vector<Node*> pathNodeList)
{
	Node* lowestFCostNode = pathNodeList[0];

	for (int i = 1; i < pathNodeList.size(); i++)
	{
		if (pathNodeList[i]->fCost < lowestFCostNode->fCost) {
			lowestFCostNode = pathNodeList[i];
		}
	}

	return lowestFCostNode;
}

std::vector<Node*> AStarPathFinding::GetNeighbourList(Node* currentNode)
{
	std::vector<Node*> neighbourList;

	if (currentNode->x - 1 >= 0) {
		// Left
		neighbourList.push_back(GetNode(currentNode->x - 1, currentNode->y));
		// Left Down
		if (currentNode->y - 1 >= 0) neighbourList.push_back(GetNode(currentNode->x - 1, currentNode->y - 1));
		// Left Up
		if (currentNode->y + 1 < grid.GetHeight()) neighbourList.push_back(GetNode(currentNode->x - 1, currentNode->y + 1));
	}
	if (currentNode->x + 1 < grid.GetWidth()) {
		// Right
		neighbourList.push_back(GetNode(currentNode->x + 1, currentNode->y));
		// Right Down
		if (currentNode->y - 1 >= 0) neighbourList.push_back(GetNode(currentNode->x + 1, currentNode->y - 1));
		// Right Up
		if (currentNode->y + 1 < grid.GetHeight()) neighbourList.push_back(GetNode(currentNode->x + 1, currentNode->y + 1));
	}
	// Down
	if (currentNode->y - 1 >= 0) 
	{
		neighbourList.push_back(GetNode(currentNode->x, currentNode->y - 1));
	}
	// Up
	if (currentNode->y + 1 < grid.GetHeight()) 
	{
		neighbourList.push_back(GetNode(currentNode->x, currentNode->y + 1));
	}

	return neighbourList;
}

std::vector<Node*> AStarPathFinding::CalculatePath(Node* endNode)
{
	finalpath.push_back(endNode);

	Node * currentNode = endNode;

	while (currentNode->cameFromNode != nullptr) 
	{
		finalpath.push_back(currentNode->cameFromNode);
		currentNode = currentNode->cameFromNode;
	}

	std::reverse(finalpath.begin(), finalpath.end());

	return finalpath;
}

Node* AStarPathFinding::GetNode(int x, int y)
{
	return grid.GetGridObject(x, y);
}


