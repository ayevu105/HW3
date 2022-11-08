/* @file edge.h
 * @brief The following code gives the declarations of the edge class
 * @author Anthony Vu
 * @date 11/07/2022
 */
#ifndef EDGE_H
#define EDGE_H

// forward declaration for vertex.cpp
class Vertex;

class Edge {
  friend class Vertex;
  friend class Graph;

 private:
  int weight = 0;
  Vertex * to;
  Vertex * from;

  // edge constructor with label and weight 
   Edge(Vertex *from, Vertex *to, int weight);

};

#endif