/* @file graph.cpp
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
  Vertex *v = nullptr;
  if (this->find(label, v)) {
    return v->neighbors.size();
  }
  return -1;
}

/* add is the function to add vertices to the graph, it returns 
 * true if the vertext is added, false if it already is in the graph
 * @param label is the string referenced
 */
bool Graph::add(const string &label) { 
  if (!this->contains(label)) {
    auto v = new Vertex(label);
    vertices.push_back(v);
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
  for (auto &v : vertices) {
    if (v->label == label) {
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
  Vertex *v = nullptr;
  if(this->find(label, v)) {
    if (v->neighbors.empty()) {
      return s;
    } 
    s = s + v->neighbors[0]->to->label;
    s = s + "(" + to_string(v->neighbors[0]->weight) + ")";

    for (auto it = v->neighbors.begin() + 1; it != v->neighbors.end(); ++it) {
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
  Vertex *v1 = nullptr;
  Vertex *v2 = nullptr;

  if(!find(from, v1)) {
    v1 = new Vertex(from);
    vertices.push_back(v1);
    numberOfVertices++;
  }

  if(!find(to, v2)) {
    v2 = new Vertex(to);
    vertices.push_back(v2);
    numberOfVertices++;
  }
  
  bool c1 = false;
  bool c2 = false;
  auto e1 = new Edge (v1, v2, weight);

  for(int i = 0; i < v1->neighbors.size(); i++) {
    Edge *temp = v1->neighbors.at(i);
    if (from == temp->from->label && to == temp->to->label) {
      delete e1;
      return false;
    }
    if (to < temp->to->label) {
      v1->neighbors.insert(v1->neighbors.begin() + i, e1);
      numberOfEdges++;
      c1 = true;
      break;
    }
  }

  if(!c1) {
    v1->neighbors.push_back(e1);
    numberOfEdges++;
  }

  if(!directionalEdges) {
    auto e2 = new Edge(v2, v1, weight);
    for (int i = 0; i < v2->neighbors.size(); i++) {
      Edge *temp = v2->neighbors.at(i);
      if(from == temp->from->label && to == temp->to->label) {
        delete e2;
        return false;
      } 
      if (to < temp->to->label) {
        v2->neighbors.insert(v2->neighbors.begin() + i, e2);
        c2 = true;
        break;
      }
    }
    if (!c2) {
      v2->neighbors.push_back(e2);
    }
  }
  return true;
}

/* disconnect disconnects two vertices from eachother and removes the edge from the graph, 
 * it returns true if the edge is successfully deleted
 * @param string from and to are references
 */
bool Graph::disconnect(const string &from, const string &to) { 
  Vertex *v = nullptr;
  if (!find(from, v)) {
    return false;
  }

  for (int i = 0; i < v->neighbors.size(); i++) {
    Edge *e = v->neighbors.at(i);
    if (from == e->from->label && to == e->to->label) {
      v->neighbors.erase(v->neighbors.begin() + i);
      delete e;
      numberOfEdges--;
      if (!directionalEdges) {
        Vertex *v2 = nullptr;
        find(to, v2);
        for (int j = 0; j < v2->neighbors.size(); j++) {
          Edge *e2 = v2->neighbors.at(j);
          if (to == e2->from->label && from == e2->to->label) {
            v2->neighbors.erase(v2->neighbors.begin() + j);
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
  for (auto &v : vertices) {
    v->visited = false;
  }
  
  Vertex *v = nullptr;
  if (!find(startLabel, v)) {
    return;
  }
  dfsHelper(v, visit);
}

/* dfsHelper is the helper function of the depth first search
 */
void Graph::dfsHelper(Vertex *v, void visit(const string &label)) {
  if (v == nullptr) {
    return;
  }

  v->visited = true;
  visit(v->label);
  for (auto &neighbor : v->neighbors) {
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
  for (auto &v : vertices) {
    v->visited = false;
  }

  Vertex *v = nullptr;
  if (!find(startLabel, v)) {
    return;
  }
  v->visited = true;
  queue<Vertex *> q;
  q.push(v);
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
    vector<Edge *> smallestEdge = dijakstraNeighborHelper(visitedArray);
    Edge *e = dijakstraDistanceHelper(smallestEdge, weights);
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

/* find looks for a specific edge in the graph
 * @param label is the string being referenced
 */
bool Graph::find(const string &label, Vertex *&vertex) const {
  for (auto &v : vertices) {
    if (v->label == label) {
      vertex = v; 
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
