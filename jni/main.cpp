// OpenGL ES 2.0 code

#include <jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "game/j.h"
#include "game/log.h"
#include "game/shaders.h"
#include "game/particle.h"
#include "game/font.h"
#include "game/node.h"
#include "game/random.h"

namespace pclengine
{

GLuint gProgram;
std::vector<Node*> renderNodes;

std::string osVersion = "";
std::string SDK       = "";
std::string vendor    = "";
std::string locale    = "";

GLfloat projMat[16];
void ortho(float left, float right, float top, float bottom, float near, float far){

    float tx = ((right+left)/(right-left))*-1;
    float ty = ((top+bottom)/(top-bottom))*-1;
    float tz = ((far+near)/(far-near))*-1;

    projMat[0] = 2/(right-left);
    projMat[1] = 0.0f;
    projMat[2] = 0.0f;
    projMat[3] = 0.0f;
    projMat[4] = 0.0f;
    projMat[5] = 2/(top-bottom);
    projMat[6] = 0.0f;
    projMat[7] = 0.0f;
    projMat[8] = 0.0f;
    projMat[9] = 0.0f;
    projMat[10] = -2/(far-near);
    projMat[11] = 0.0f;
    projMat[12] = tx;
    projMat[13] = ty;
    projMat[14] = tz;
    projMat[15] = 1.0f; 
}

void applyOrtho(GLuint program)
{
    GLint projectionUniform = glGetUniformLocation(program, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projMat[0]);
    checkGlError("glUniformMatrix4fv");
}

void createEmitter(int maxp, float x, float y)
{
  ParticleEmitter* emitter = new ParticleEmitter(maxp);
  emitter->setTexture(1);
  emitter->setPosition(x, y);
  renderNodes.push_back(emitter);
}

void createTextField(std::string text, float x, float y)
{
  TextField* tf = new TextField(0,"font1.dat");
  tf->setText(text, x, y);
  renderNodes.push_back(tf);
}


bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram();
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    glViewport(0, 0, w, h);
    ortho(0,w,0,h,-1,1);
    checkGlError("glViewport");

    for(auto node : renderNodes)
      delete node;

    renderNodes.clear();

    createEmitter(5000, w * 0.5f, h * 0.5f);
    createEmitter(900, w * 0.8f, h * 0.7f);
    createEmitter(1670, w * 0.2f, h * 0.7f);

    createTextField(std::string("OS: " + osVersion), 0, 100);
    createTextField(std::string("SDK: " + SDK), 0, 160);
    createTextField(std::string("Vendor: " + vendor), 0, 220);
    createTextField(std::string("Loc: " + locale), 0, 290);

    return true;
}

void renderFrame() {

    glClearColor(1.0, 0.5, 0.5, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");
    applyOrtho(gProgram);

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    for(auto node : renderNodes)
    {
      node->update(RANDOM().frand(0.04, 0.2));
      node->render(gProgram);
    }
}

std::string getOSVersion()
{
  std::string os = "";
  JniMethodInfo minfo;
  if(JniHelper::getStaticMethodInfo(minfo, "com/android/gl2jni/GL2JNIActivity", "getOSVersion", "()Ljava/lang/String;"))
  {
    jstring s = (jstring) minfo.env->CallStaticObjectMethod(minfo.cid, minfo.mid);
    const char *str = minfo.env->GetStringUTFChars(s, 0);
    os.append(str);
    minfo.env->ReleaseStringUTFChars(s, str);
  }

  return os;
}

std::string getSDK()
{
  std::string sdk = "";
  JniMethodInfo minfo;
  if(JniHelper::getStaticMethodInfo(minfo, "com/android/gl2jni/GL2JNIActivity", "getSDK", "()Ljava/lang/String;"))
  {
    jstring s = (jstring) minfo.env->CallStaticObjectMethod(minfo.cid, minfo.mid);
    const char *str = minfo.env->GetStringUTFChars(s, 0);
    sdk.append(str);
    minfo.env->ReleaseStringUTFChars(s, str);
  }

  return sdk;
}

std::string getDeviceName()
{
  std::string dev = "";
  JniMethodInfo minfo;
  if(JniHelper::getStaticMethodInfo(minfo, "com/android/gl2jni/GL2JNIActivity", "getDeviceName", "()Ljava/lang/String;"))
  {
    jstring s = (jstring) minfo.env->CallStaticObjectMethod(minfo.cid, minfo.mid);
    const char *str = minfo.env->GetStringUTFChars(s, 0);
    dev.append(str);
    minfo.env->ReleaseStringUTFChars(s, str);
  }

  return dev;
}

std::string getLocale()
{
  std::string loc = "";
  JniMethodInfo minfo;
  if(JniHelper::getStaticMethodInfo(minfo, "com/android/gl2jni/GL2JNIActivity", "getLocale", "()Ljava/lang/String;"))
  {
    jstring s = (jstring) minfo.env->CallStaticObjectMethod(minfo.cid, minfo.mid);
    const char *str = minfo.env->GetStringUTFChars(s, 0);
    loc.append(str);
    minfo.env->ReleaseStringUTFChars(s, str);
  }

  return loc;
}

extern "C" {
    JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI INIT");
    int code = JniHelper::init(vm);
    osVersion = getOSVersion();
    SDK = getSDK();
    vendor = getDeviceName();
    locale = getLocale();

    return code;
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}

}
