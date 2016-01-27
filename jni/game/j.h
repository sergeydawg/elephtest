//
//  jnihelper.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#ifndef _elephtest_jni_helper_H
#define _elephtest_jni_helper_H

#include <jni.h>
#include <stdlib.h>

namespace pclengine {

struct JniMethodInfo
{
  JNIEnv*     env;
  jclass      cid;
  jmethodID   mid;
};

class JniHelper
{
public:

  static jint init(JavaVM*);
  static JavaVM* getJavaVM();
  static JNIEnv* getJavaEnv();
  static bool getStaticMethodInfo(JniMethodInfo&, const char* class_name, const char* method_name, const char* param);

private:

  static bool getJavaEnv(JNIEnv **env);
  static bool getStaticMethodInfo(JniMethodInfo& methodinfo, jclass classID, const char* methodName, const char* paramCode);
  static jclass makeGlobalClassRef(const char* class_name);
  static jclass getClassId(JNIEnv* env, const char* name);
};

} // namespace pclengine

#endif
