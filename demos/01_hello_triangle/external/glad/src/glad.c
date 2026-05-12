#include <glad/glad.h>

PFNGLVIEWPORTPROC glad_glViewport = 0;
PFNGLCLEARCOLORPROC glad_glClearColor = 0;
PFNGLCLEARPROC glad_glClear = 0;
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = 0;
PFNGLGENBUFFERSPROC glad_glGenBuffers = 0;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = 0;
PFNGLBINDBUFFERPROC glad_glBindBuffer = 0;
PFNGLBUFFERDATAPROC glad_glBufferData = 0;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = 0;
PFNGLCREATESHADERPROC glad_glCreateShader = 0;
PFNGLSHADERSOURCEPROC glad_glShaderSource = 0;
PFNGLCOMPILESHADERPROC glad_glCompileShader = 0;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = 0;
PFNGLCREATEPROGRAMPROC glad_glCreateProgram = 0;
PFNGLATTACHSHADERPROC glad_glAttachShader = 0;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = 0;
PFNGLDELETESHADERPROC glad_glDeleteShader = 0;
PFNGLUSEPROGRAMPROC glad_glUseProgram = 0;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = 0;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = 0;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = 0;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = 0;

int gladLoadGLLoader(GLADloadproc load)
{
    if (load == 0)
    {
        return 0;
    }

    glad_glViewport = (PFNGLVIEWPORTPROC)load("glViewport");
    glad_glClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
    glad_glClear = (PFNGLCLEARPROC)load("glClear");
    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load("glGenVertexArrays");
    glad_glGenBuffers = (PFNGLGENBUFFERSPROC)load("glGenBuffers");
    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load("glBindVertexArray");
    glad_glBindBuffer = (PFNGLBINDBUFFERPROC)load("glBindBuffer");
    glad_glBufferData = (PFNGLBUFFERDATAPROC)load("glBufferData");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load("glVertexAttribPointer");
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load("glEnableVertexAttribArray");
    glad_glCreateShader = (PFNGLCREATESHADERPROC)load("glCreateShader");
    glad_glShaderSource = (PFNGLSHADERSOURCEPROC)load("glShaderSource");
    glad_glCompileShader = (PFNGLCOMPILESHADERPROC)load("glCompileShader");
    glad_glGetShaderiv = (PFNGLGETSHADERIVPROC)load("glGetShaderiv");
    glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load("glGetShaderInfoLog");
    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC)load("glCreateProgram");
    glad_glAttachShader = (PFNGLATTACHSHADERPROC)load("glAttachShader");
    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC)load("glLinkProgram");
    glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)load("glGetProgramiv");
    glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load("glGetProgramInfoLog");
    glad_glDeleteShader = (PFNGLDELETESHADERPROC)load("glDeleteShader");
    glad_glUseProgram = (PFNGLUSEPROGRAMPROC)load("glUseProgram");
    glad_glDrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load("glDeleteVertexArrays");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load("glDeleteBuffers");
    glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");

    return glad_glViewport &&
           glad_glClearColor &&
           glad_glClear &&
           glad_glGenVertexArrays &&
           glad_glGenBuffers &&
           glad_glBindVertexArray &&
           glad_glBindBuffer &&
           glad_glBufferData &&
           glad_glVertexAttribPointer &&
           glad_glEnableVertexAttribArray &&
           glad_glCreateShader &&
           glad_glShaderSource &&
           glad_glCompileShader &&
           glad_glGetShaderiv &&
           glad_glGetShaderInfoLog &&
           glad_glCreateProgram &&
           glad_glAttachShader &&
           glad_glLinkProgram &&
           glad_glGetProgramiv &&
           glad_glGetProgramInfoLog &&
           glad_glDeleteShader &&
           glad_glUseProgram &&
           glad_glDrawArrays &&
           glad_glDeleteVertexArrays &&
           glad_glDeleteBuffers &&
           glad_glDeleteProgram;
}
