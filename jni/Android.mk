LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(USE_ARM_MODE),1)
LOCAL_ARM_MODE := arm
endif

LOCAL_MODULE    := libgl2jni
LOCAL_CPFLAGS   :=-Werror -Wextern-c-compat
LOCAL_SRC_FILES := main.cpp \
	                 game/particle.cpp \
	                 game/shaders.cpp \
	                 game/font.cpp \
	                 game/j.cpp \
									 game/random.cpp

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid

include $(BUILD_SHARED_LIBRARY)
