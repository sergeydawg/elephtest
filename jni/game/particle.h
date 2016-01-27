//
//  particle.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#ifndef __elephtest__particle__
#define __elephtest__particle__

#include <vector>
#include "node.h"
#include "glutils.h"

#define MAX_PARTICLES 360

namespace pclengine
{

enum EmitterType
{
  GRAVITY = 0,
  RADIAL = 1
};

struct ParticleQuad
{
  Vertex2F bl;
  Vertex2F br;
  Vertex2F tl;
  Vertex2F tr;
};

struct ParticleColor 
{
  float red;
  float green;
  float blue;
  float alpha;
};

class Particle
{
  public:
  ParticleColor color, colorDelta;
  float x, y;
  float startX, startY;
  float velocityX, velocityY;
  float radialAcceleration;
  float tangentialAcceleration;
  float radius, radiusDelta;
  float rotation, rotationDelta;
  float spin, spinDelta;
  float size, sizeDelta;    
  float timeToLive;
};

class ParticleEmitter : public Node
{
public:

  ParticleEmitter(size_t max_particles);
  virtual ~ParticleEmitter();

  virtual void update(float passedTime);
  virtual void render(GLuint program);

  virtual void setTexture(GLuint tex);
  virtual void setPosition(float x, float y);

  float mTimeDelay;
  float mFrameTime;
  int mNumParticles;
  float mScaleFactor;
  
  std::vector<Particle> mParticles;

  u32 mVertexBuffer;
  u32 mIndexBuffer;

  ParticleQuad* mQuads;
  GLushort* mQIndices;
  Tex2F* uvBuffer;

  GLuint mTexture;
  
  // emitter configuration
  float mEmitterX;                                // sourcePosition x
  float mEmitterY;                                // sourcePosition y
  float mEmitterXVariance;                        // sourcePositionVariance x
  float mEmitterYVariance;                        // sourcePositionVariance y
  float mEmissionRate;                            // emission rate
  bool mActive;                                   // whether emitter is active
  float mElapsedTime;                             // total time elapsed in sec
  float mDuration;                                // total emitting time in sec, -1 means infinity
  EmitterType mType;
  
  // particle configuration
  int mMaxNumParticles;                           // maxParticles
  float mLifespan;                                // particleLifeSpan
  float mLifespanVariance;                        // particleLifeSpanVariance
  float mStartSize;                               // startParticleSize
  float mStartSizeVariance;                       // startParticleSizeVariance
  float mEndSize;                                 // finishParticleSize
  float mEndSizeVariance;                         // finishParticleSize
  float mEmitAngle;                               // angle
  float mEmitAngleVariance;                       // angleVariance
  float mStartSpin;                               // startSpin
  float mStartSpinVariance;                       // startSpinVariance
  float mEndSpin;                                 // endSpin
  float mEndSpinVariance;                         // endSpinVariance
  
  // gravity configuration
  float mSpeed;                                   // speed
  float mSpeedVariance;                           // speedVariance
  float mGravityX;                                // gravity x
  float mGravityY;                                // gravity y
  float mRadialAcceleration;                      // radialAcceleration
  float mRadialAccelerationVariance;              // radialAccelerationVariance
  float mTangentialAcceleration;                  // tangentialAcceleration
  float mTangentialAccelerationVariance;          // tangentialAccelerationVariance
  
  // radial configuration 
  float mMaxRadius;                               // maxRadius
  float mMaxRadiusVariance;                       // maxRadiusVariance
  float mMinRadius;                               // minRadius
  float mRotatePerSecond;                         // rotatePerSecond
  float mRotatePerSecondVariance;                 // rotatePerSecondVariance
  
  // color configuration
  ParticleColor mStartColor;                      // startColor
  ParticleColor mStartColorVariance;              // startColorVariance
  ParticleColor mEndColor;                        // finishColor
  ParticleColor mEndColorVariance;                // finishColorVariance
  float mRedSpeed;                                // speed with which changes red color 
  float mGreenSpeed;                              // speed with which changes green color
  float mBlueSpeed;                               // speed with which changes blue color
  float mAlphaSpeed;                              // speed with which changes alpha color

  bool mVisible;

  // blend function
  int mBlendFuncSource;                           // blendFuncSource
  int mBlendFuncDestination;                      // blendFuncDestination

};

}

#endif /* defined(__elephtest__particle__) */
