#!/bin/sh

ndk-build clean
ant clean

ndk-build
ant debug

cp bin/GL2JNIActivity-debug.apk apk_folder/debug/

adb install -r apk_folder/debug/GL2JNIActivity-debug.apk
adb shell am start -n com.android.gl2jni/com.android.gl2jni.GL2JNIActivity

