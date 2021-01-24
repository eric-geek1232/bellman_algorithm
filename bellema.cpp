/*By Eric 22/01/2021 -BELLMAN ALGORITHM- */
#include <iostream>

#define CONVERT_CHAR_TO_INDEX(caracter) (caracter - 'A') 
#define CONVERT_INT_TO_CHAR(integer) (char(integer + 65))
#define INFINITE (100000)

using namespace std;

// LINKED LIST STRUCTURE
struct Node
{
	char vertex;
	int cost;
	Node *next;
};

class Graph
{
	private:
		Node **adjacencyList;
		bool isUpdated;
		char *parents;
		int nVertices;
		
		Node *createVertex(char vertex, int cost);
		void insertList(Node *&edges, char vertex, int cost);
		void printList(Node *edges);
		void setParents(Node *edges, int parent);
		bool isCycleEdge(Node *edges, int parent);
		
	public:
		Graph();
		Graph(int nVertices);
		void setAdjacencyList();
		void showAdjacencyList();
		void startBellmanAlgo(char start, char end);
		void showShortestPath(char start, char end, int cost);
		void reset();
		bool isEmpty();
};

Graph::Graph()
{
	nVertices = 0;
}

Graph::Graph(int nVertices)
{
	this->nVertices = nVertices;
	adjacencyList = new Node*[nVertices];
	parents = new char[nVertices];
	isUpdated = false;
	
	for (int v = 0; v < nVertices; v++) {
		// CREATE HASH TABLE
		adjacencyList[v] = NULL;
		parents[v] = ' ';
		insertList(adjacencyList[v], CONVERT_INT_TO_CHAR(v), INFINITE);
	}
}

Node *Graph::createVertex(char vertex, int cost)
{
	Node *newNode = new Node();
	
	newNode->vertex = vertex;
	newNode->next = NULL;
	newNode->cost = cost;
	
	return newNode; 
}

void Graph::setAdjacencyList()
{
	int cost;
	char noPath;
	char destine;
	char currentVertex;
	
	for (int v = 0; v < nVertices; v++) {
		currentVertex = CONVERT_INT_TO_CHAR(v);
		cout<< "\n\n\t Add the routes of "<< currentVertex<< endl;
		
		// CREATE ROUTE
		do {
			cout<< "\n [Warning]-Would you like to add routes? [s/n]: ";
			cin>> noPath;
			
			noPath = toupper(noPath);
			
			if (noPath == 'N') {
				break;
			}
			
			cout<< "\n\n "<< currentVertex<< "--> ";
			cin>> destine;
			
			destine = toupper(destine);
			
			cout<< " Type the cost from "<< currentVertex<< " TO "<< destine<< ": ";
			cin>> cost;
			
			// INSERT ROUTE
			insertList(adjacencyList[v], destine, cost);
		} while (noPath == 'S');
		
		cout<< "\n";
	}
}

void Graph::insertList(Node *&edges, char vertex, int cost)
{
	// INSERT ALWAYS AT THE END OF THE LIST
	if (edges == NULL) {
		Node *newNode = createVertex(vertex, cost);
		edges = newNode;
		
		return;		
	}
	
	// KEEP ITERATING
	insertList(edges->next, vertex, cost);
}

void Graph::showAdjacencyList()
{
	cout<< "\n\n\t\tShowing Adjacency List...\n"<< endl;
	cout<< "\n\tVertices\t\t\tEdges\n"<< endl;
	
	// ITERATE EACH OF THE LISTS
	for (int v = 0; v < nVertices; v++) {
		cout<< " ("<< CONVERT_INT_TO_CHAR(v)<< ") Parent: "<< parents[v]<< " --> ";
		printList(adjacencyList[v]->next);
		cout<< "\n"<< endl;	
	}	
}

void Graph::printList(Node *edges)
{
	// END OF THE LIST
	if (edges == NULL) {
		return;
	}
	
	cout<< "   ("<< edges->vertex<< ") -|- cost: "<< edges->cost;
	
	// KEEP ITERATING
    	printList(edges->next);
}

void Graph::startBellmanAlgo(char start, char end)
{
	int index = CONVERT_CHAR_TO_INDEX(start);
	
	parents[index] = -1;
	adjacencyList[index]->cost = 0;
	
	// RELATE THE VERTICES [V-1]
	for (int v = index; v < nVertices-1; v++) {
		isUpdated = false;
		
		setParents(adjacencyList[index]->next, index);	
		
		if (!isUpdated) break;
	}
	
	showAdjacencyList();
	
	if (isCycleEdge(adjacencyList[index]->next, index) && isUpdated) {
		cout<< "\n\n [Warning]-Graph has -VE edge cycle"<< endl;
	} else {
		cout<< "\n\n Path: ";
		showShortestPath(start, end, 0);	
	}
}

void Graph::setParents(Node *edges, int parent)
{
	// END OF THE LIST
	if (edges == NULL) {
		parent++;
		
		// ALL THE VERTICES WERE PROCESSED 
		if (parent == nVertices) return;
		
		return setParents(adjacencyList[parent]->next, parent);	
	}
    
    	// GET NEIGHBOR
    	int destine = CONVERT_CHAR_TO_INDEX(edges->vertex);
    
    	// UPDATE NEIGHBORS
    	if (adjacencyList[parent]->cost + edges->cost < adjacencyList[destine]->cost) {
		adjacencyList[destine]->cost = adjacencyList[parent]->cost + edges->cost;
		parents[destine] = CONVERT_INT_TO_CHAR(parent);
		isUpdated = true;
	}
	
	// NEXT NODE IN THE LIST
	setParents(edges->next, parent);
}

bool Graph::isCycleEdge(Node *edges, int parent) 
{
	if (edges == NULL) return false;	
    
    	int destine = CONVERT_CHAR_TO_INDEX(edges->vertex);
    
    	if (adjacencyList[parent]->cost + edges->cost < adjacencyList[destine]->cost) return true;
}

bool Graph::isEmpty() 
{
	return nVertices == 0 ? true : false; 
}

void Graph::reset()
{
	isUpdated = false;
	
	for (int v = 0; v < nVertices; v++) {
		parents[v] = ' ';
		adjacencyList[v]->cost = INFINITE;
	}
}

void Graph::showShortestPath(char start, char end, int cost)
{
	if (start == end) {
		cout<< "("<< start<< ")\n"<< endl;
		cout<< " Total cost: "<< cost<< "\n"<< endl;
		
		return;
	}
	
	int destine = CONVERT_CHAR_TO_INDEX(end);
	
	// GET COST FROM THE END
	if (cost == 0) {
		cost = adjacencyList[destine]->cost;
	}
	
	cout<< " ("<< adjacencyList[destine]->vertex<< ") <--";
	
	if (parents[destine] == ' ') {
		cout<< "\n\n [Warning]- THIS NODE HAS NO CHILDREN ";
		return;	
	}
	
	// KEEP SEARCHING
	showShortestPath(start, parents[destine], cost);	
}

int main()
{
	Graph *graph = new Graph();
	char start;
	char end;
	int nVertices;
	
	int option;
	
	do {
		cout<< "\tBELLMAN ALGORITHM\n"<< endl;
		cout<< "\t.::MENU::.\n"<< endl;
		cout<< " [1] Create Graph"<< endl;
		cout<< " [2] Show Path"<< endl;
		cout<< " [3] Exit"<< endl;
		cout<< "\n Choose: ";
		cin>>option;
		
		switch(option) {
			case 1:
				delete graph;
				
				cout<< "\n\n Type the number of vertices ";
				cin>> nVertices;
				
				graph = new Graph(nVertices);
				
				graph->setAdjacencyList();
				break;
				
			case 2:
				if (!graph->isEmpty()) {
					graph->reset();
					
					cout<< "\n\n from what point would you like to start? ";
					cin>> start;
					
					cout<< "\n\n to what point would like to go? ";
					cin>> end;
					
					start = toupper(start);
					end = toupper(end);
					
					graph->startBellmanAlgo(start, end);
										
					cout<< "\n"<< endl;	
				} else {
					cout<< "\n\n [Warning]-there's no a graph\n"<< endl;
				}
				break;
				
			case 3:
				cout<< "\n\n\t**Big thanks for using this program**\n"<< endl;
				break;
				
			default:
				cout<< "\n\n\tWrong option... try again!\n"<< endl;
		}
		
		system("pause");
		system("cls");
	} while (option != 3);
	
	return 0;
}
