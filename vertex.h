/* @file vertex.h
 * @brief The following code gives the declarations of the vertex class
 * @author Anthony Vu
 * @date 11/07/2022
 */

#ifndef VERTEX_H
#define VERTEX_H

#include "edge.h"
#include <string>
#include <vector>


using namespace std;

class Vertex {
  friend class Graph;
  friend class Edge;

  string label;
  bool visited;
  vector<Edge*> neighbors;

public:

  explicit Vertex(const string &label);


};

#endif 
