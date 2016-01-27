//
//  shaders.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#ifndef __elephtest__shaders__
#define __elephtest__shaders__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "types.h"

namespace pclengine
{

GLuint createProgram();
void checkGlError(const char* op);
void printGLString(const char *name, GLenum s);

}
#endif
