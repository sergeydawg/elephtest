#include "font.h"
#include "shaders.h"

namespace pclengine {

unsigned char assets_font1_dat[] = {
  0x66, 0x6f, 0x6e, 0x74, 0x10, 0x0a, 0x6d, 0x00, 0x00, 0x00, 0x05, 0x03,
  0x06, 0x08, 0x07, 0x08, 0x08, 0x04, 0x05, 0x05, 0x09, 0x07, 0x05, 0x07,
  0x03, 0x08, 0x08, 0x05, 0x07, 0x07, 0x08, 0x07, 0x07, 0x07, 0x07, 0x07,
  0x03, 0x04, 0x06, 0x07, 0x07, 0x07, 0x08, 0x07, 0x07, 0x07, 0x07, 0x07,
  0x07, 0x07, 0x07, 0x05, 0x07, 0x08, 0x07, 0x08, 0x08, 0x07, 0x07, 0x07,
  0x07, 0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x07, 0x08, 0x05, 0x08, 0x05,
  0x07, 0x09, 0x04, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03,
  0x06, 0x07, 0x03, 0x0b, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
  0x07, 0x0b, 0x08, 0x07, 0x07, 0x07, 0x03, 0x07, 0x08, 0x08, 0x02, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0b, 0x0b
};
unsigned int assets_font1_dat_len = 119;

TextField::~TextField()
{
  delete vertices;
}

TextField::TextField(GLuint tex, std::string dat_file)
:
  field_text(""),
  max_width(0),
  max_height(0),
  spacing(0),
  scale(1.0),
  w_frac(0),
  h_frac(0)

{
  int indices_num = 2048;

  vertices = new Vertex3F_T2F[2048]; 

	//generate indices for text rendition
	for(size_t i=0; i<(indices_num/6); ++i)
	{
		indices[i*6] = (u16)(i*4);
		indices[i*6+1] = (u16)(i*4+1);
		indices[i*6+2] = (u16)(i*4+2);
		indices[i*6+3] = (u16)(i*4);
		indices[i*6+4] = (u16)(i*4+2);
		indices[i*6+5] = (u16)(i*4+3);
	}

  max_width = assets_font1_dat[4];
  max_height = assets_font1_dat[5];

  for(size_t i = 9; i < assets_font1_dat_len; i++)
  {
    char_width[i-9] = assets_font1_dat[i];
  }

  LOGI("max_w:%d max_h:%d", max_width, max_height);

  texture = tex;
  scale = 5.0f;
  w_frac = (float)(max_width / (float)256);
  h_frac = (float)(max_height / (float)128);
}

void TextField::setText(std::string text, float x, float y)
{
  field_text = text;
  p_x = x;
  p_y = y;
}

void TextField::render(GLuint program)
{
  if(field_text.empty())
    return;

  genCharsVertices();

  const size_t vertex_stride = sizeof(vertices[0]);
  GLuint pos = glGetAttribLocation(program, "vPosition");
  GLuint tex = glGetAttribLocation(program, "a_texCoord");
  GLuint texUniform = glGetUniformLocation(program, "s_texture");

  glUniform1i(texUniform, texture);
  checkGlError("glUniform1i");

  glVertexAttribPointer(pos, 2 , GL_FLOAT, GL_FALSE, vertex_stride, vertices);
  glEnableVertexAttribArray(pos);
 
  // Prepare the texturecoordinates
  glVertexAttribPointer(tex, 2, GL_FLOAT,GL_FALSE, 0 ,uvBuffer);
  glEnableVertexAttribArray(tex);

  glDrawElements(GL_TRIANGLES, total_verts/4*6, GL_UNSIGNED_SHORT, indices); 
  glDisableVertexAttribArray(pos);
  glDisableVertexAttribArray(tex);
}

void TextField::genCharsVertices()
{
  total_verts = 0;
  const char* text = field_text.c_str();
	int len = strlen(text);
  int max_len = 50;

  float x = p_x;
  float y = p_y;

  len = (max_len != -1 && len > max_len) ? max_len : len;
	
	for(int i = 0; i < len; ++i)
	{
		int verts_for_char = genCharVertices(x, y, text[i], total_verts);
    x += char_width[text[i]-32] * scale;
		
		total_verts += verts_for_char;
	}
}

int TextField::genCharVertices(float x, float y, char num, int vert_index)
{
	int	row, col; 
	float frow, fcol;
	
	num &= 255;
	
	if((num & 127) == 32)
		return 0;		// space
	
	//if(y <= -font.max_height)
	//	return 0;			// totally off screen
	
	row = (num >> 4) - 2;
	col = num & 15;
	
	frow = row * h_frac;
	fcol = col * w_frac;

  int index = vert_index;
	
	//1
	vertices[index].tex.u = fcol;
	vertices[index].tex.v = frow;		
	vertices[index].pos.x = x;
	vertices[index].pos.y = y;
  uvBuffer[index].u = fcol;
  uvBuffer[index].v = frow;

	++index;
		
	//2
	vertices[index].tex.u = fcol+w_frac;
	vertices[index].tex.v = frow;		
	vertices[index].pos.x = x+max_width*scale;
	vertices[index].pos.y = y;
  uvBuffer[index].u = fcol+w_frac;
  uvBuffer[index].v = frow;
	++index;
	
	//3
	vertices[index].tex.u = fcol+w_frac;
	vertices[index].tex.v = frow+h_frac;		
	vertices[index].pos.x = x+max_width*scale;
	vertices[index].pos.y = y+max_height*scale;
  uvBuffer[index].u = fcol+w_frac;
  uvBuffer[index].v = frow+h_frac;
	++index;
	 
	//4
	vertices[index].tex.u = fcol;
	vertices[index].tex.v = frow+h_frac; 
	vertices[index].pos.x = x;
	vertices[index].pos.y = y+max_height*scale;
  uvBuffer[index].u = fcol;
  uvBuffer[index].v = frow+h_frac;
	++index;
	
	return 4;

}


} //namespace pclengine

