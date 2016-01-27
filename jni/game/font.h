//
//  font.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#ifndef __elephtest_font_H
#define __elephtest_font_H

#include "types.h"
#include "glutils.h"
#include "log.h"
#include "node.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>

namespace pclengine {

class TextField : public Node
{
public:
  TextField(GLuint tex, std::string dat_file);
  virtual ~TextField();

  virtual void render(GLuint program);
  virtual void update(float passedTime){};
  void setText(std::string text, float x, float y);

private:

  void genCharsVertices();
  int genCharVertices(float x, float y, char num, int vert_index);

  std::string field_text;
  Vertex3F_T2F* vertices;
  u16 indices[2048];
  Tex2F uvBuffer[2048]; 
  int total_verts;
 

  u8 char_width[128];  // width of each character 
  u8 max_width;        // box width 
  u8 max_height;       // box height 
  u32 spacing;         // space between characters 
  u32 scale;             
  float w_frac;
  float h_frac;
  GLuint texture;

  float p_x,p_y;

};



} //namespace pclengine

#endif
