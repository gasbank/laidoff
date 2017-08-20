#pragma once

#include "platform_detection.h"

#if LW_PLATFORM_WIN32 || LW_PLATFORM_OSX || LW_PLATFORM_LINUX
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <limits.h> // INT_MAX
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // _countof
#elif LW_PLATFORM_ANDROID
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <time.h>
#elif LW_PLATFORM_IOS || LW_PLATFORM_IOS_SIMULATOR
#import <OpenGLES/ES2/glext.h>
#elif LW_PLATFORM_RPI
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif
