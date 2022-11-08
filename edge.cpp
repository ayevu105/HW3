/* @file edge.cpp
 * @brief The following code gives the implementations of the edge class
 * @author Anthony Vu
 * @date 11/07/2022
 */
#include "edge.h"

//edge constructor 
Edge::Edge(Vertex *from, Vertex *to, int weight) {
  this->from = from;
  this->to = to;
  this->weight = weight;
}