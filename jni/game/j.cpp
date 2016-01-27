#include "j.h"
#include "log.h"

namespace pclengine {

static JavaVM* java_vm = NULL;

jint JniHelper::init(JavaVM* vm)
{
  JNIEnv* env = NULL;
  if(vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
    return -1;
  if(vm->AttachCurrentThread(&env, 0) < 0)
    return -1;

  java_vm = vm;

  return JNI_VERSION_1_6;
}

JavaVM* JniHelper::getJavaVM()
{
  return java_vm;
}

bool JniHelper::getJavaEnv(JNIEnv **env)
{
  //if(java_vm->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK)
  if(java_vm->GetEnv((void**)env, JNI_VERSION_1_6) != JNI_OK)
  {
    LOGI("Failed to get the environment using GetEnv()");
    return false;
  }

  if(java_vm->AttachCurrentThread(env, 0) < 0)
  {
    LOGI("Failed to get the environment using AttachCurrentThread()");
    return false;
  }

  return true;
}

JNIEnv* JniHelper::getJavaEnv()
{
  JNIEnv* env = NULL;
  const bool res = getJavaEnv(&env);
  return env;
}

jclass JniHelper::getClassId(JNIEnv* env, const char* name)
{
  jclass ret = env->FindClass(name);
  if(!ret)
    LOGI("Failed to find class of %s", name);

  return ret;
}

bool JniHelper::getStaticMethodInfo(JniMethodInfo& methodinfo, const char* className, const char* methodName, const char* paramCode)
{
  JNIEnv* pEnv = getJavaEnv();
  jclass classID = getClassId(pEnv, className);

  return getStaticMethodInfo(methodinfo, classID, methodName, paramCode);
}

bool JniHelper::getStaticMethodInfo(JniMethodInfo& methodinfo, jclass classID, const char* methodName, const char* paramCode)
{
  JNIEnv* pEnv = getJavaEnv();
  jmethodID methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
  if(!methodID)
  {
    LOGI("Failed to find static method id of %s", methodName);
    return false;
  }

  methodinfo.env = pEnv;
  methodinfo.cid = classID;
  methodinfo.mid = methodID;

  return true;
}

jclass JniHelper::makeGlobalClassRef(const char* class_name)
{
  JNIEnv* env = getJavaEnv();
  jclass cls = env->FindClass(class_name);
  jclass glob_cls = (jclass)env->NewGlobalRef(cls);
  env->DeleteLocalRef(cls);
  return glob_cls;
}

} // namespace pclengine
