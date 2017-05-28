#include <assert.h>
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

/*
  Use Me!
  =======
  A Node in this graph representation consists of:
  1. data: the string data contained in that node
  2. edges: a vector of pointers to nodes that node is connected to.
*/
struct Node{
  string data;
  vector<Node*> *edges;
};

/*
  Use Me!
  =======
  A Graph in this graph representation consists of:
  1. nodes: a vector of nodes contained in the graph.
*/
struct Graph{
  vector<Node*> *nodes;
};

/*
  Fill me in!
  ===========
  Purpose: perform a Breadth-First Search (BFS) for a Node* with particular data
  Argument: graph -- a Graph* as defined above
  Argument: query -- the string data of the node we are searching for
  Argument: root_index -- the index of the Node* in graph->nodes at which to start the search
  Returns: A deque* of Node*s    during the search.
  Side effects: None
 */
deque<Node*>* bfs(Graph* graph, string query, unsigned int root_index){
  /*
  cout << graph << endl;
  cout << query << endl;
  cout << root_index << endl;

  for (unsigned i = 0 ; i < graph->nodes->size() ; i++) {
    cout << i << ", " << graph->nodes->at(i) << endl;
  }
  */
  deque<Node*> *explored = new deque<Node*>;
  deque<Node*> *frontier = new deque<Node*>;
  vector<Node*> *trail = new vector<Node*>;

  Node *root = graph->nodes->at(root_index); // Storing address of root/start node
  /*
  cout << root->data << endl; // printing the node data ('South Station' for index 9)
  for (unsigned i = 0 ; i < root->edges->size() ; i++)
  cout << i << ", " << root->edges->at(i) << endl; // printing the address of connecting nodes
  */
  // If we are already at the goal state return explored deque
  
  if(root->data.compare(query) == 0) {
    explored->push_back(root); // Add node at the end
    return explored;
  }

  frontier->push_front(root); // Add node at beginning
  trail->push_back(root); // Add node at the end

  //Keep looping while 'frontier' is not empty
  while(frontier->empty() == 0) {
    //cout << "frontier->front(): " << frontier->front() << endl;

    Node *node = frontier->front(); // Address of first node in frontier
    explored->push_back(node); // Add node at the end
    frontier->pop_front(); // Delete first node

    int no_nodes = node->edges->size(); // number of connecting nodes

    for(signed int i = 0; i < no_nodes; i++) {
      // If we are at the goal state return explored deque
      if(node->edges->at(i)->data.compare(query) == 0) {
      explored->push_back(node->edges->at(i));
        return explored;
      }

      // we do not want to revisit a node again
      int processed = 0;
      for(unsigned int j = 0; j < trail->size(); j++) {
        if (trail->at(j) == node->edges->at(i)) {
          processed = 1;
          break;
        }
      }
      if(processed == 0) {
        frontier->push_back(node->edges->at(i));
        trail->push_back(node->edges->at(i));
      }
    }
  }

  return new deque<Node*>();
}

/*
  Fill me in!
  ===========
  Purpose: perform a Depth-First Search (DFS) for a Node* with particular data
  Argument: graph -- a Graph* as defined above
  Argument: query -- the string data of the node we are searching for
  Argument: root_index -- the index (>=0) of the Node* in graph->nodes at which to start the search
  Returns: A deque* of Node*s visited during the search.
  Side effects: None
 */
deque<Node*>* dfs(Graph* graph, string query, unsigned int root_index){
  deque<Node*> *explored = new deque<Node*>;
  deque<Node*> *frontier = new deque<Node*>;
  vector<Node*> *trail = new vector<Node*>;

  Node *root = graph->nodes->at(root_index);  // Storing address of root/start node

  // If we are already at the goal state return explored deque
  if(root->data.compare(query) == 0) {
        explored->push_back(root); // Add node at the end
        return explored;
      }

  frontier->push_front(root); // Add node at beginning
  trail->push_back(root); // Add node at the end

  //Keep looping while 'frontier' is not empty
  while(frontier->empty() == 0) {

    Node *node = frontier->back(); // Address of last node in frontier
    explored->push_back(node); // Add node at the end
    frontier->pop_back(); // Delete last node
    
    int no_nodes = node->edges->size(); // number of connecting nodes

    for(int i = 0; i < no_nodes; i++) {
      // If we are at the goal state return explored deque
      if(node->edges->at(i)->data.compare(query) == 0) {
        explored->push_back(node->edges->at(i));
        return explored;
      }
    }

    for (signed int j = no_nodes - 1; j >= 0; j--) {
      // we do not want to revisit a node again
      int visited = 0;
      for(unsigned int k = 0; k < trail->size(); k++) {
        if (trail->at(k) == node->edges->at(j)) {
          visited = 1;
          break;
        }
      }
      if(visited == 0) {
        frontier->push_back(node->edges->at(j));
        trail->push_back(node->edges->at(j));
      }
    }

  }

  return new deque<Node*>();
}

/*
  Don't modify me!
  ===========
  This is just helper code that reads in a graph file for you.
 */
Graph* readFile(char* fileName){
  ifstream inFile(fileName);
  if(!inFile){
    cerr << "Cannot open '" << fileName << "'\n";
    exit( 1 );
  }

  int size = 0;
  inFile >> size;
  Graph* graph = new Graph();
  graph->nodes = new vector<Node*>();
  for(int i = 0; i < size; i++){
    Node* n = new Node();
    n ->data = "";
    n -> edges = new vector<Node*>();
    graph->nodes->push_back(n);
  }

  string line;
  getline(inFile,line);
  for(int i = 0; i < size; i++){
    getline(inFile,line);
    istringstream ssin(line);
    vector<string> record;
    while(ssin){
      string field;
      if(!getline(ssin,field,','))break;
      record.push_back(field);
    }
    Node* node = graph->nodes->at(atoi(record[0].c_str()));
    node->data = record[1];
    for(unsigned int j = 2; j < record.size(); j++)
      node->edges->push_back(graph->nodes->at(atoi(record[j].c_str())));
  }
  return graph;
}

/*
  Don't modify me!
  ===========
  This is the code that runs your BFS and DFS functions on the input specified through the command line.
 */
int main(int argc, char* argv[]){
  assert(argc==4);
  Graph* graph = readFile(argv[1]);
  string query = argv[2];
  int root_index = atoi(argv[3]);
  try{
    deque<Node*>* bfsvalue = bfs(graph,query,root_index);
    cout<<"BFS: ";
    for(unsigned int i = 0; i < bfsvalue->size(); i++)
      cout<<bfsvalue->at(i)->data<<", ";
    if(bfsvalue->empty())
      cout<<"NO SOLUTION FOUND";
    cout<<endl;
    deque<Node*>* dfsvalue = dfs(graph,query,root_index);
    cout<<"DFS: ";
    for(unsigned int i = 0; i < dfsvalue->size(); i++)
      cout<<dfsvalue->at(i)->data<<", ";
    if(dfsvalue->empty())
      cout<<"NO SOLUTION FOUND";
    cout<<endl;
  }catch(string caught){
    cout<<"\033[1;31m"<<caught<<"\033[0m"<<endl;
  }

  for(unsigned int i = 0; i < graph->nodes->size(); i++){
    delete graph->nodes->at(i)->edges;
    delete graph->nodes->at(i);
  }
  delete graph->nodes;
  delete graph;

  return 0;
}
