/* @file vertex.cpp
 * @brief The following code gives the implementations of the vertex class
 * @author Anthony Vu
 * @date 11/07/2022
 */

#include "vertex.h"
#include "edge.h"
#include <algorithm>

using namespace std;

//creates an unvisited vertex
Vertex::Vertex(const string &label) {
  this->label = label;
  visited = false; 
}

