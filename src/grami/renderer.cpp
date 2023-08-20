#include "grami/grami.hpp"
#include "shaders.hpp"
#include <string>

const float quad[6][4] = {
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 0.0f},
    {1.0f, -1.0f, 1.0f, 1.0f},
};

const char* vertexShaderCode = R"(
    #version 150

    in vec2 vert;
    in vec2 vertUV;

    out vec2 fragUV;

    void main() {
        gl_Position = vec4(vert.x, vert.y, 0.0, 1.0);
        fragUV = vertUV;
    }
)";

const char* fragmentShaderCode = R"(
    #version 150
    uniform sampler2D tex;

    in vec2 fragUV;

    out vec4 outputColor;

    void main() {
        outputColor = vec4(texture(tex, fragUV).rgb, 1.0);
    }
)";

void Renderer::init(int width, int height) {
    this->width = width;
    this->height = height;

    // Init shader
    program = compileShaderProgram(vertexShaderCode, fragmentShaderCode);
    glUseProgram(program);
    GLint tex_loc = glGetUniformLocation(program, "tex");
    GLint vert_loc = glGetAttribLocation(program, "vert");
    GLint vertUV_loc = glGetAttribLocation(program, "vertUV");

    // Init buffers
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vert_loc);
    glVertexAttribPointer(vert_loc, 2, GL_FLOAT, false, 4 * 4, nullptr);
    glEnableVertexAttribArray(vertUV_loc);
    glVertexAttribPointer(vertUV_loc, 2, GL_FLOAT, false, 4 * 4, (const void*)(2 * 4));
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * 4, &quad, GL_STATIC_DRAW);

    // Init textures
    glGenTextures(1, &frame_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glUniform1i(tex_loc, 0);

    data.resize(width * height);
}

Renderer::~Renderer() {
    if (glIsTexture(frame_texture)) {
        glDeleteTextures(1, &frame_texture);
    }
    if (glIsBuffer(vbo)) {
        glDeleteBuffers(1, &vbo);
    }
    if (glIsVertexArray(vao)) {
        glDeleteVertexArrays(1, &vao);
    }
    if (glIsProgram(program)) {
        glDeleteProgram(program);
    }
}

void Renderer::update() {
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
}

void Renderer::present() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}