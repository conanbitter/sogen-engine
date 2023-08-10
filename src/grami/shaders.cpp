#include "shaders.hpp"
#include <iostream>

std::string getShaderLog(GLuint shader) {
    if (glIsShader(shader)) {
        std::string message = "";

        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            message = infoLog;
        }

        delete[] infoLog;

        return message;
    } else {
        return "Wrong shader handle";
    }
}

std::string getProgramLog(GLuint shaderProgram) {
    if (glIsProgram(shaderProgram)) {
        std::string message = "";

        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(shaderProgram, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            message = infoLog;
        }

        delete[] infoLog;
        return message;
    } else {
        return "Wrong shader program handle";
    }
}

GLuint compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const GLchar* lines[] = {
        source};
    glShaderSource(shader, 1, lines, NULL);
    glCompileShader(shader);
    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled != GL_TRUE) {
        std::string msg;
        if (shaderType == GL_VERTEX_SHADER) {
            msg = "Vertex shader compile error: ";
        } else if (shaderType == GL_FRAGMENT_SHADER) {
            msg = "Fragment shader compile error: ";
        } else {
            msg = "Shader compile error: ";
        }
        std::cout << msg << getShaderLog(shader) << std::endl;
        return 0;
    }
    return shader;
}

GLuint compileShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode) {
    GLuint program = glCreateProgram();

    GLuint compiledVertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint compiledFragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    glAttachShader(program, compiledVertexShader);
    glAttachShader(program, compiledFragmentShader);
    glLinkProgram(program);
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        std::cout << "Shader link error: " << getProgramLog(program) << std::endl;
        return 0;
    }
    glDeleteShader(compiledVertexShader);
    glDeleteShader(compiledFragmentShader);
    return program;
};