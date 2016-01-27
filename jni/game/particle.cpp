//
//  particle.cpp
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#include "particle.h"
#include <algorithm>
#include "random.h"
#include "shaders.h"
#include "log.h"

namespace pclengine
{

static void init_quad_tex_coords(ParticleEmitter& pe)
{
  const float origin_x = 0.0f;
  const float origin_y = 0.0f;
  const GLfloat w = 32;
  const GLfloat cw = 32;
  const GLfloat h = 32;
  const GLfloat ch = 32;

  GLfloat left = origin_x / w;
  GLfloat bottom = origin_y / h;
  GLfloat right = left + cw / w;
  GLfloat top = bottom + ch / h;
  //#endif

  for(int i=0; i<pe.mMaxNumParticles; ++i) 
  {
    // bottom-left vertex:
    pe.uvBuffer[i].u = left;
    pe.uvBuffer[i].v = bottom;
    // bottom-right vertex:
    pe.uvBuffer[i+1].u = right;
    pe.uvBuffer[i+1].v = bottom;
    // top-left vertex:
    pe.uvBuffer[i+2].u = left;
    pe.uvBuffer[i+2].v = top;
    // top-right vertex:
    pe.uvBuffer[i+3].u = right;
    pe.uvBuffer[i+3].v = top;

    i+=3;
  }
}


bool add_particle_at_pos(ParticleEmitter& pe, float x, float y, float startX, float startY)
{    
  if(pe.mParticles.size() >= pe.mMaxNumParticles)
    return false;

  float lifespan = RANDOM().frand(pe.mLifespan, pe.mLifespanVariance);

  //LOGI("lifespan %f", lifespan);

  if(lifespan <= 0.0f) 
    return false;

  Particle particle;

  particle.timeToLive = lifespan;

  particle.x = x;
  particle.y = y;
  particle.startX = startX;
  particle.startY = startY;


  //LOGI("particle x:%f y:%f", x,y);

  float angle = RANDOM().frand(pe.mEmitAngle, pe.mEmitAngleVariance);
  float speed = RANDOM().frand(pe.mSpeed, pe.mSpeedVariance);
  particle.velocityX = speed * cosf(angle);
  particle.velocityY = speed * sinf(angle);

  particle.radius = RANDOM().frand(pe.mMaxRadius, pe.mMaxRadiusVariance);
  particle.radiusDelta = pe.mMaxRadius / lifespan;
  particle.rotation = RANDOM().frand(pe.mEmitAngle, pe.mEmitAngleVariance);
  particle.rotationDelta = RANDOM().frand(pe.mRotatePerSecond, pe.mRotatePerSecondVariance);    
  particle.radialAcceleration = pe.mRadialAcceleration;
  particle.tangentialAcceleration = pe.mTangentialAcceleration;

  float spinA = RANDOM().frand(pe.mStartSpin, pe.mStartSpinVariance);
  float spinB = RANDOM().frand(pe.mEndSpin, pe.mEndSpinVariance);
  particle.spin = spinA;
  particle.spinDelta = (spinB - spinA) / particle.timeToLive;

  float scale = 1.0f;
  float particleStartSize  = std::max(0.1f, RANDOM().frand(pe.mStartSize, pe.mStartSizeVariance)) * scale;
  float particleFinishSize = std::max(0.1f, RANDOM().frand(pe.mEndSize, pe.mEndSizeVariance)) * scale; 
  particle.size = particleStartSize;
  particle.sizeDelta = (particleFinishSize - particleStartSize) / lifespan;

  ParticleColor colorDelta;
  colorDelta.red   = (pe.mEndColor.red   - pe.mStartColor.red)   / lifespan;
  colorDelta.green = (pe.mEndColor.green - pe.mStartColor.green) / lifespan;
  colorDelta.blue  = (pe.mEndColor.blue  - pe.mStartColor.blue)  / lifespan;
  colorDelta.alpha = (pe.mEndColor.alpha - pe.mStartColor.alpha) / lifespan;

  particle.color = pe.mStartColor;
  particle.colorDelta = colorDelta;

  pe.mParticles.push_back(particle);
  return true;
}

bool add_particle(ParticleEmitter& pe)
{    
  return add_particle_at_pos(
    pe, 
    RANDOM().frand(pe.mEmitterX, pe.mEmitterXVariance), 
    RANDOM().frand(pe.mEmitterY, pe.mEmitterYVariance), 
    pe.mEmitterX, 
    pe.mEmitterY
  );
}

void update_particle(ParticleEmitter& pe, Particle& particle, float passedTime, EmitterType etype)
{
  passedTime = std::min(passedTime, particle.timeToLive);
  particle.timeToLive -= passedTime;

  if(etype == RADIAL) 
  {
    particle.rotation += particle.rotationDelta * passedTime;
    particle.radius   -= particle.radiusDelta   * passedTime;
    particle.x = pe.mEmitterX - cos(particle.rotation) * particle.radius;
    particle.y = pe.mEmitterY - sin(particle.rotation) * particle.radius;

    if(particle.radius < pe.mMinRadius)
      particle.timeToLive = 0;                
  } 
  else 
  {
    float distanceX = particle.x - particle.startX;
    float distanceY = particle.y - particle.startY;                
    float distanceScalar = std::max(0.01f, (float)sqrt(distanceX * distanceX + distanceY * distanceY));

    float radialX = distanceX / distanceScalar;
    float radialY = distanceY / distanceScalar;
    float tangentialX = radialX;
    float tangentialY = radialY;

    radialX *= particle.radialAcceleration;
    radialY *= particle.radialAcceleration;

    float newY = tangentialX;
    tangentialX = -tangentialY * particle.tangentialAcceleration;
    tangentialY = newY * particle.tangentialAcceleration;

    particle.velocityX += passedTime * (pe.mGravityX + radialX + tangentialX);
    particle.velocityY += passedTime * (pe.mGravityY + radialY + tangentialY);
    particle.x += particle.velocityX * passedTime;
    particle.y += particle.velocityY * passedTime;
  }

  particle.spin += particle.spinDelta * passedTime;
  particle.size += particle.sizeDelta * passedTime;
}

void emit_particles_at_pos(ParticleEmitter& pe, float passedTime, float x, float y, float startX, float startY)
{
  if(pe.mParticles.size() < pe.mMaxNumParticles) 
  {
    add_particle_at_pos(pe, x, y, startX, startY);
  }
}

void emit_particles(ParticleEmitter& pe, float passedTime)
{
  emit_particles_at_pos(
    pe,
    passedTime,
    RANDOM().frand(pe.mEmitterX, pe.mEmitterX + pe.mEmitterXVariance), 
    RANDOM().frand(pe.mEmitterY, pe.mEmitterY + pe.mEmitterYVariance), 
    pe.mEmitterX, 
    pe.mEmitterY
  );
}

void sync_render_data(ParticleEmitter& ps)
{
  for(int i=0; i < ps.mParticles.size(); ++i)
  {
    auto& particle = ps.mParticles.at(i);    

    ParticleQuad *quad = &ps.mQuads[i];

    // vertices
    const GLfloat size_2 = particle.size*0.5f;//40.0;
    // bottom-left vertex:
    quad->bl.x = particle.x - size_2;
    quad->bl.y = particle.y - size_2;

    // bottomht vertex:
    quad->br.x = particle.x + size_2;
    quad->br.y = particle.y - size_2;

    // top-leertex:
    quad->tl.x = particle.x - size_2;
    quad->tl.y = particle.y + size_2;

    // top-rivertex:
    quad->tr.x = particle.x + size_2;
    quad->tr.y = particle.y + size_2;       
  }
}


ParticleEmitter:: ~ParticleEmitter()
{
  delete mQuads;
  delete mQIndices;
}

void ParticleEmitter::setPosition(float x, float y)
{
  mEmitterX = x;
  mEmitterY = y;
}

void ParticleEmitter::setTexture(GLuint tex)
{
  mTexture = tex;
}

ParticleEmitter::ParticleEmitter(size_t max_particles)
:
  mMaxNumParticles(max_particles),
  mNumParticles(0),
  mVertexBuffer(0)
{
  mQuads = new ParticleQuad[mMaxNumParticles];
  mQIndices = new GLushort[mMaxNumParticles*6];
  uvBuffer = new Tex2F[mMaxNumParticles*4];

  mActive = true;
  mEmissionRate = max_particles;
  mType = GRAVITY;
  
  mEmitterYVariance = 0.0f;
  mEmitterXVariance = 0.0f;
  mDuration = -1.0f;
  mSpeed = 5.0f;
  mSpeedVariance = RANDOM().urand(30, 100);
  mLifespan = 10;
  mLifespanVariance = 1;

  mEmitAngle = 0.0f;
  mEmitAngleVariance = 114 * M_PI/180;
  mGravityX = 0;
  mGravityY = RANDOM().irand(-150, 350); //-150;
  mRadialAcceleration = 0;
  mRadialAccelerationVariance = 0;
  mTangentialAcceleration = 0;
  mTangentialAccelerationVariance = 0;
  mStartSize = 128.0f;
  mStartSizeVariance = 15.0f;
  mEndSize = 5.0f;
  mEndSizeVariance = 0.0f;

  mMinRadius = 0.0f;
  mMaxRadius = 0.0f;
  mMaxRadiusVariance = 0.0f;

  mRotatePerSecond = 0;
  mRotatePerSecondVariance = 0;
  
  mStartSpin = 212*(M_PI/180);
  mStartSpinVariance = 5*(M_PI/180);
  mEndSpin = 0*(M_PI/180);
  mEndSpinVariance = 0*(M_PI/180);

  for(int i = 0; i < mMaxNumParticles; ++i)
  {
    const int i6 = i*6;
    const int i4 = i*4;
    mQIndices[i6+0] = (GLushort) i4+0;
    mQIndices[i6+1] = (GLushort) i4+1;
    mQIndices[i6+2] = (GLushort) i4+2;

    mQIndices[i6+5] = (GLushort) i4+1;
    mQIndices[i6+4] = (GLushort) i4+2;
    mQIndices[i6+3] = (GLushort) i4+3;
  }

  init_quad_tex_coords(*this);

  glGenBuffers(1, &mVertexBuffer);
  glGenBuffers(1, &mIndexBuffer);
 
}

void ParticleEmitter::update(float passedTime)
{
  for( auto it = mParticles.begin(); it != mParticles.end(); )
  {
    if(it->timeToLive > passedTime)
    { 
      update_particle(*this, *it, passedTime, mType);
      ++it;
    } 
    else
      it = mParticles.erase(it);
  }

  emit_particles(*this, passedTime);
}

void ParticleEmitter::render(GLuint program)
{
  sync_render_data(*this);
  const size_t vertex_stride = sizeof(mQuads[0].bl);

  GLuint pos = glGetAttribLocation(program, "vPosition");
  GLuint tex = glGetAttribLocation(program, "a_texCoord");
  GLuint texUniform = glGetUniformLocation(program, "s_texture");

  glUniform1i(texUniform, mTexture);
  checkGlError("glUniform1i");

  glVertexAttribPointer(pos, 2 , GL_FLOAT, GL_FALSE, vertex_stride, mQuads);
  glEnableVertexAttribArray(pos);
 
  // Prepare the texturecoordinates
  glVertexAttribPointer(tex, 2, GL_FLOAT,GL_FALSE, 0 ,uvBuffer);
  glEnableVertexAttribArray(tex);

  glDrawElements(GL_TRIANGLES,mParticles.size()*6, GL_UNSIGNED_SHORT, mQIndices); 
  glDisableVertexAttribArray(pos);
  glDisableVertexAttribArray(tex);
  
  glDisableVertexAttribArray(pos);
  glDisableVertexAttribArray(tex);
}


}
