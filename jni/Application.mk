APP_STL := c++_static
APP_ABI := armeabi-v7a
APP_PLATFORM := android-9

NDK_TOOLCHAIN_VERSION := clang3.5
APP_CPPFLAGS := -frtti -std=c++11 -fsigned-char -ffast-math 	-mfloat-abi=softfp -marm -mfpu=vfp
APP_LDFLAGS := -latomic
USE_ARM_MODE := 1
