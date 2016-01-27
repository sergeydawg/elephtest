//
//  node.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//
#ifndef __elephtest_node_H
#define __elephtest_node_H

#include "types.h"
#include "glutils.h"

namespace pclengine {

class Node
{

public:

  Node(){};
  virtual ~Node(){}

  virtual void render(GLuint program) = 0;
  virtual void update(float passedTime) = 0;

private:

};


} //namespace pclengine

#endif
