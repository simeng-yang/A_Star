#include <iostream>
#include <math.h>
#include <algorithm> 
#include <string>
#include <sstream>

#include "a_star.hpp"

using namespace std;

// Calculates and returns the fcost
int AStar::getFCost(Node* currentNode) {
	return currentNode -> gCost + currentNode -> hCost;
}

// Computes the Euclidean distance between two nodes and scales it by 10 to avoid floats
int AStar::getHDistance(Node* A, Node* B) {
	return sqrt(pow(A -> posX - B -> posX, 2)+(pow(A -> posY - B -> posY, 2))) * 10;
}

// Iterates through the node's parents and creates a list of the shortest path to take
list<Node*> AStar::retracePath (Node* startNode, Node* endNode) {
	list<Node*> path;	
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		path.push_front(currentNode);
		currentNode = currentNode->parent;
	}
	return path;
}

// For the current node, cnode, discovers all walkable neighbours, and adds them to the neighbours list
list<Node*> AStar::getNeighbours(Node* cnode, Node* grid[Y_LENGTH][X_LENGTH]) {
	list<Node*> neighbours;	
	
	int xRef = cnode -> posX;
	int yRef = cnode -> posY;
	
	//	Y-LENGTH = 5
	//	X_LENGTH = 6
	
	for (int i = yRef - 1; i <= yRef + 1; i++){
		for (int j = xRef - 1; j <= xRef + 1; j++){
			if (i <= Y_LENGTH - 1 && i >= 0 && j <= X_LENGTH - 1 && j >= 0){
				if (grid[i][j] -> walkable == true && grid[i][j] != cnode){
					neighbours.push_front(grid[i][j]);
				}
			}
		}
	}
	
	return neighbours;
}

// Outputs the discovered path as a string
string AStar::getPathString(list<Node*> path) {
	stringstream outputStream;
	for (list<Node*>::iterator it = path.begin(); it != path.end(); ++it) {
		outputStream << "(" << (*it)->posX << "," << (*it)->posY << ")" << endl;
	}
	return outputStream.str();
}

// Outputs path distance
int AStar::getPathDistance(list<Node*> path) {
	return path.size() > 0 ? path.back() -> fCost : 0;
}

// Finds shortest path between startNode and endNode using A* algorithm
list<Node*> AStar::findPath(Node* world[Y_LENGTH][X_LENGTH], Node* startNode, Node* endNode) {
	list<Node*> openSet; // list of nodes not yet evaluated
	list<Node*> closedSet; // list of nodes already evaluated
	Node* currentNode;

	startNode -> gCost = 0;
	startNode -> hCost = getHDistance(startNode, endNode);
	startNode -> fCost = startNode -> hCost;
	
	openSet.push_back(startNode);
	while(!openSet.empty()) {

		int minF = 999;	//	Minimum f cost
		int minH = 999;	//	Minimum h cost
		Node* curNode;	//	Pointer to node with lowest fCost/hCost
		
		for(list<Node*>::iterator it = openSet.begin(); it != openSet.end(); it++) {
			if (getFCost(*it) < minF){	//	Node fCost is smaller than current minimum
				minF = getFCost(*it);	//	Update minimum
				curNode = (*it);	//	Update pointer
			}
			else if (getFCost(*it)  == minF){	//	fCost is the same
				if ((*it) -> hCost < minH){	//	If hCost is smaller, update hCost and update pointer
					minH = (*it) -> hCost;
					curNode = (*it);
				}
			}
		}
				
		openSet.remove(curNode);
		closedSet.push_back(curNode);
		
		list<Node*> neighbours = getNeighbours(curNode, world);
			
		for (list<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); it++){ 
		
			if (!(find(closedSet.begin(), closedSet.end(), (*it)) != closedSet.end())){
			
			//	Distance from start to neighbour
			int tempG = curNode -> gCost + getHDistance(curNode, (*it));	
			
			//	Neighbour not in open set
			if (!(find(openSet.begin(), openSet.end(), (*it)) != openSet.end())){		
				openSet.push_back(*it);
				(*it) -> parent = curNode;
				(*it) -> gCost = tempG;
				(*it) -> hCost = getHDistance((*it), endNode);
				(*it) -> fCost = getFCost(*it);	
			}
			else if (tempG < getHDistance(startNode, (*it))){
				(*it) -> parent = curNode;
				(*it) -> gCost = tempG;
				(*it) -> hCost = getHDistance((*it), endNode);
				(*it) -> fCost = getFCost(*it);	
			}
		}
	}
		if (curNode == endNode) {
			list<Node*> path = retracePath(startNode, endNode);
			return path;
		}
	}
	
	list<Node*> empty;
	return empty;
}
