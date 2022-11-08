/* @file TreadedBST.cpp
 * @brief The following code gives the implementations of the following functions that
 * were listed in the Graph Project assignment.
 * @author Anthony Vu
 * @date 11/7/2022
 */

#include "graph.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

using namespace std;

// constructor, empty graph
// directionalEdges defaults to true
Graph::Graph(bool directionalEdges) {
    this->directionalEdges = directionalEdges;
    numberOfVertices = 0;
    numberOfEdges = 0;
}

// destructor
Graph::~Graph() {
  for (auto temp : vertices) {
    for (auto &n : temp->neighbors) {
      delete n;
    }
    temp->neighbors.clear();
    delete temp;
  }
  vertices.clear();
  numberOfVertices = 0;
  numberOfEdges = 0;

}

// verticesSize returns the total number of vertices
int Graph::verticesSize() const { 
  return numberOfVertices; 
}

// edgesSize returns the total number of edges
int Graph::edgesSize() const { 
  return numberOfEdges; 
}

/* VertexDegree returns the number of edges from given vertex, -1 if vertex not found
 * @param label is the string referenced
 */
int Graph::vertexDegree(const string &label) const { 
  Vertex *vert = nullptr;
  if (this->find(label, vert)) {
    return vert->neighbors.size();
  }
  return -1;
}

/* add is the function to add vertices to the graph, it returns 
 * true if the vertext is added, false if it already is in the graph
 * @param label is the string referenced
 */
bool Graph::add(const string &label) { 
  if (!this->contains(label)) {
    auto vert = new Vertex(label);
    vertices.push_back(vert);
    numberOfVertices++;
    return true; 
  }
    return false;
}



/* contains checks if a vertex is already in the graph, it returns
 * true if vertex already in graph and false otherwise. 
 * @param label is the string reference
 */
bool Graph::contains(const string &label) const {
  for (auto &vert : vertices) {
    if (vert->label == label) {
      return true;
    }
  }
  return false;
}

/* getEdgesAsString creates a string of edges and weights, it returns
 * string representing edges and weights, "" if vertex not found 
 * A-3->B, A-5->C should return B(3),C(5)
 * @param label is the string referenced
 */
string Graph::getEdgesAsString(const string &label) const { 
  string s; 
  Vertex *vert = nullptr;
  if(this->find(label, vert)) {
    if (vert->neighbors.empty()) {
      return s;
    } 
    s = s + vert->neighbors[0]->to->label;
    s = s + "(" + to_string(vert->neighbors[0]->weight) + ")";

    for (auto it = vert->neighbors.begin() + 1; it != vert->neighbors.end(); ++it) {
      string l = (*it)->to->label;
      int w = (*it)->weight;
      s = s + "," + l;
      s = s + "(" + to_string(w) + ")";
    }
  }
    return s;
}

/* connect connects two vertices together, it returns true if the 
 * vertices are successfully connected
 * @param strings from and to are the references and weight is the weight of edge
 */
bool Graph::connect(const string &from, const string &to, int weight) {
  if(from == to) {
    return false;
  }
  Vertex *vert1 = nullptr;
  Vertex *vert2 = nullptr;

  if(!find(from, vert1)) {
    vert1 = new Vertex(from);
    vertices.push_back(vert1);
    numberOfVertices++;
  }

  if(!find(to, vert2)) {
    vert2 = new Vertex(to);
    vertices.push_back(vert2);
    numberOfVertices++;
  }
  
  bool c1 = false;
  bool c2 = false;
  auto e1 = new Edge (vert1, vert2, weight);

  for(int i = 0; i < vert1->neighbors.size(); i++) {
    Edge *temp = vert1->neighbors.at(i);
    if (from == temp->from->label && to == temp->to->label) {
      delete e1;
      return false;
    }
    if (to < temp->to->label) {
      vert1->neighbors.insert(vert1->neighbors.begin() + i, e1);
      numberOfEdges++;
      c1 = true;
      break;
    }
  }

  if(!c1) {
    vert1->neighbors.push_back(e1);
    numberOfEdges++;
  }

  if(!directionalEdges) {
    auto e2 = new Edge(vert2, vert1, weight);
    for (int i = 0; i < vert2->neighbors.size(); i++) {
      Edge *temp = vert2->neighbors.at(i);
      if(from == temp->from->label && to == temp->to->label) {
        delete e2;
        return false;
      } 
      if (to < temp->to->label) {
        vert2->neighbors.insert(vert2->neighbors.begin() + i, e2);
        c2 = true;
        break;
      }
    }
    if (!c2) {
      vert2->neighbors.push_back(e2);
    }
  }
  return true;
}

/* disconnect disconnects two vertices from eachother and removes the edge from the graph, 
 * it returns true if the edge is successfully deleted
 * @param string from and to are references
 */
bool Graph::disconnect(const string &from, const string &to) { 
  Vertex *vert = nullptr;
  if (!find(from, vert)) {
    return false;
  }

  for (int i = 0; i < vert->neighbors.size(); i++) {
    Edge *e = vert->neighbors.at(i);
    if (from == e->from->label && to == e->to->label) {
      vert->neighbors.erase(vert->neighbors.begin() + i);
      delete e;
      numberOfEdges--;
      if (!directionalEdges) {
        Vertex *vert2 = nullptr;
        find(to, vert2);
        for (int j = 0; j < vert2->neighbors.size(); j++) {
          Edge *e2 = vert2->neighbors.at(j);
          if (to == e2->from->label && from == e2->to->label) {
            vert2->neighbors.erase(vert2->neighbors.begin() + j);
            delete e2;
            break;
          }
       } 
      } 
    return true;
   }
  }
return false;
}



/* dfs is the implementation of a depth first search
 * @param startLabel is where the traversal starts and calls visit
 */
void Graph::dfs(const string &startLabel, void visit(const string &label)) {
  for (auto &vert : vertices) {
    vert->visited = false;
  }
  
  Vertex *vert = nullptr;
  if (!find(startLabel, vert)) {
    return;
  }
  dfsHelper(vert, visit);
}

/* dfsHelper is the helper function of the depth first search
 */
void Graph::dfsHelper(Vertex *vert, void visit(const string &label)) {
  if (vert == nullptr) {
    return;
  }

  vert->visited = true;
  visit(vert->label);
  for (auto &neighbor : vert->neighbors) {
    Vertex *temp = neighbor->to;
    if (!temp->visited) {
      dfsHelper(temp, visit);
    }
  }
}

/* bfs is the implementation of a breadth first search
 * @param startLabel is where the traversal starts and calls visit
 */
void Graph::bfs(const string &startLabel, void visit(const string &label)) {
  for (auto &vert : vertices) {
    vert->visited = false;
  }

  Vertex *vert = nullptr;
  if (!find(startLabel, vert)) {
    return;
  }
  vert->visited = true;
  queue<Vertex *> q;
  q.push(vert);
  while (!q.empty()) {
    Vertex *temp = q.front();
    q.pop();
    visit(temp->label);
    for(auto &neighbor :temp->neighbors) {
      Vertex *n = neighbor->to;
      if(!n->visited) {
        n->visited = true;
        q.push(n);
      }
    }
  }
}

// store the weights in a map
// store the previous label in a map
/* dijkstra is the implementation of dijakstras algorithm
 * @param startLabel is where the traversal starts
 */
pair<map<string, int>, map<string, string>>
Graph::dijkstra(const string &startLabel) const {
  map<string, int> weights;
  map<string, string> previous;
  for (auto &v : vertices) {
    v->visited = false;
  }
  
  Vertex *v = nullptr;
  if (!find(startLabel, v)) {
    return make_pair(weights, previous);
  }
  
  vector<Vertex *> visitedArray;
  v->visited = true;
  visitedArray.push_back(v);
  for (int i = 0; i < visitedArray.size(); i++) {
    vector<Edge *> smallestEdge = smallestNeighbors(visitedArray);
    Edge *e = minimumDistance(smallestEdge, weights);
    if (e == nullptr) {
      break;
    }
    e->to->visited = true;
    visitedArray.push_back(e->to);

    auto it = weights.find(e->from->label);
    if (it == weights.end()) {
      weights.emplace(e->to->label, e->weight);
      previous.emplace(e->to->label, e->from->label);
    } else {
      weights.emplace(e->to->label, (*it).second + e->weight);
      previous.emplace(e->to->label, e->from->label);
    }
  }
  return make_pair(weights, previous);
}

/* dijakstraNeighborHelper is the helper function to dijakstra, it finds the smallest
 * neighbor vector and returns it 
 * @param visited array vector
 */
vector<Edge *> Graph::dijakstraNeighborHelper(vector<Vertex *> visitedArray) {
  vector<Edge *> smallestEdge;
  for (int i = 0; i < visitedArray.size(); i++) {
  vector<Edge *> n = visitedArray.at(i)->neighbors;
  Edge *min; 
  if (n.empty()) {
    visitedArray.erase(visitedArray.begin() + i);
    i--;
    continue;
  }
  int e = 0;
  min = n[e];
  while (e + 1 < n.size() && min->to->visited) {
    min = n[++e];
  }

  for (int j = e + 1; j < n.size(); j++) {
    Edge *temp = n.at(j);
    if (temp->to->visited) {
      continue;
    }
    if (temp->weight < min->weight) {
      min = temp;
    }
  }
  if (min == nullptr || min->to->visited) {
    visitedArray.erase(visitedArray.begin() + i);
    i--;
    continue;
  }
  smallestEdge.push_back(min);
  }
  return smallestEdge;
}

/* dijakstraDistanceHelper is the helper function to dijakstra, it finds the smallest
 * distance and returns it 
 * @param vector of the smallest edge and a map
 */
Edge *Graph::dijakstraDistanceHelper(vector<Edge *> smallestEdge, map<string, int> weights) {
  if (smallestEdge.empty()) {
    return nullptr;
  }

  Edge *min = smallestEdge[0];
  for(int i = 0; i < smallestEdge.size(); i++) {
    Edge *temp = smallestEdge[i];

    auto itMin = weights.find(min->from->label);
    auto itTemp = weights.find(temp->from->label);

    int minDistance = (itMin == weights.end()) ? 0 : (*itMin).second;
    int tempDistance = (itTemp == weights.end()) ? 0 : (*itTemp).second;

    if (temp->weight + tempDistance < min->weight + minDistance) {
      min = temp;
    }
  }
  return min;
}


bool Graph::find(const string &label, Vertex *&vert) const {
  for (auto &vertex : vertices) {
    if (vertex->label == label) {
      vert = vertex; 
      return true;
    }
  }
  return false;
}

// read a text file and create the graph
bool Graph::readFile(const string &filename) {
  ifstream myfile(filename);
  if (!myfile.is_open()) {
    cerr << "Failed to open " << filename << endl;
    return false;
  }
  int edges = 0;
  int weight = 0;
  string fromVertex;
  string toVertex;
  myfile >> edges;
  for (int i = 0; i < edges; ++i) {
    myfile >> fromVertex >> toVertex >> weight;
    connect(fromVertex, toVertex, weight);
  }
  myfile.close();
  return true;
}
