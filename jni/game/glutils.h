//
//  glutils.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#ifndef __elephtest__glutils__
#define __elephtest__glutils__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "types.h"

namespace pclengine
{

#define GAME_COLOR_PART_RED_B(color)    (((color) >> 0)  & 0xff)
#define GAME_COLOR_PART_GREEN_B(color)  (((color) >> 8)  & 0xff)
#define GAME_COLOR_PART_BLUE_B(color)   (((color) >> 16) & 0xff)
#define GAME_COLOR_PART_ALPHA_B(color)  (((color) >> 24) & 0xff)

struct Tex2F 
{
   GLfloat u;
   GLfloat v;
};

inline Tex2F tex2f(const GLfloat u, const GLfloat v)
{
  Tex2F t = {u , v};
  return t;
}

struct Vertex2F
{
  GLfloat x;
  GLfloat y;
};

inline Vertex2F vertex2f(const GLfloat x, const GLfloat y)
{
  Vertex2F c = {x, y};
  return c;
}

struct Vertex3F
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

inline Vertex3F vertex3f(const GLfloat x, const GLfloat y, const GLfloat z)
{
  Vertex3F c = {x, y, z};
  return c;
}
    
struct Color4B
{
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
};

inline Color4B color4b(const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte a)
{
  Color4B c = {r, g, b, a};
  return c;
}

inline Color4B color4b(u32 color)
{
  Color4B c = {
    (GLubyte)GAME_COLOR_PART_RED_B(color),
    (GLubyte)GAME_COLOR_PART_GREEN_B(color),
    (GLubyte)GAME_COLOR_PART_BLUE_B(color),
    (GLubyte)GAME_COLOR_PART_ALPHA_B(color)
  };
  return c;
}
  
struct Vertex2F_C4B_T2F
{
  Vertex2F pos;
  Color4B  color;
  Tex2F    tex;
};

struct Vertex3F_T2F
{
  Vertex3F pos;
  Tex2F    tex;
};

struct Vertex2F_T2F
{
  Vertex2F pos;
  Tex2F    tex;
};
}

#endif /* defined(__elephtest__glutils__) */
