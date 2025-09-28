#ifndef RAYCASTER_GL_LOADER_HPP
#define RAYCASTER_GL_LOADER_HPP

#include <Windows.h>
#include <GL/gl.h>

#include <opengl/glcorearb.h>
#include <opengl/wglext.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB    0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB    0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB     0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLDETACHSHADERPROC             glDetachShader;
extern PFNGLDRAWELEMENTSINSTANCEDPROC    glDrawElementsInstanced;
extern PFNGLGENBUFFERSPROC               glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC              glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC              glLinkProgram;
extern PFNGLSHADERSOURCEPROC             glShaderSource;
extern PFNGLUNIFORM3FVPROC               glUniform3fv;
extern PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

void load_gl_functions();

#endif

