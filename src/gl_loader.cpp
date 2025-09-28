#include "gl_loader.hpp"

PFNGLATTACHSHADERPROC             glAttachShader             = nullptr;
PFNGLCOMPILESHADERPROC            glCompileShader            = nullptr;
PFNGLCREATEPROGRAMPROC            glCreateProgram            = nullptr;
PFNGLCREATESHADERPROC             glCreateShader             = nullptr;
PFNGLDELETEPROGRAMPROC            glDeleteProgram            = nullptr;
PFNGLDELETESHADERPROC             glDeleteShader             = nullptr;
PFNGLDETACHSHADERPROC             glDetachShader             = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC    glDrawElementsInstanced    = nullptr;
PFNGLGENBUFFERSPROC               glGenBuffers               = nullptr;
PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays          = nullptr;
PFNGLGETPROGRAMIVPROC             glGetProgramiv             = nullptr;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog        = nullptr;
PFNGLGETSHADERIVPROC              glGetShaderiv              = nullptr;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog         = nullptr;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation       = nullptr;
PFNGLLINKPROGRAMPROC              glLinkProgram              = nullptr;
PFNGLSHADERSOURCEPROC             glShaderSource             = nullptr;
PFNGLUNIFORM3FVPROC               glUniform3fv               = nullptr;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv         = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

void load_gl_functions() {
    glAttachShader             = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glCompileShader            = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glCreateProgram            = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glCreateShader             = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    glDeleteShader             = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glDetachShader             = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader"); 
    glDrawElementsInstanced    = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
    glGenBuffers               = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glGenVertexArrays          = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    glGetShaderiv              = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glGetUniformLocation       = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glLinkProgram              = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");    
    glShaderSource             = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glUniform3fv               = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
    glUniformMatrix4fv         = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
}

