#pragma once

#include "Types.h"
#include <string>

class Shader
{
public:
    Shader();
    ~Shader();

    void Draw();

private:
    void CreateProgram();
    void GenBuffers();
    u32 Compile(const char* shaderFile);

private:
    u32 _program;
    u32 _vbo;
    u32 _vao;
    u32 _ebo;

    f32 _verts[16] =
    {
         1.0f,  1.0f, 1.0f, 0.0f,  // top right
         1.0f, -1.0f, 1.0f, 1.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f,  // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f   // top left
    };

    u32 _indices[6] =
    {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

private:
    // Shaders
    const char* _vertShader = R"(
        #version 460 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aUV;

        out vec2 UV;

        void main()
        {
            UV = aUV;
            gl_Position = vec4(aPos, 0.0, 1.0);
        })";

    const char* _fragShader = R"(
        #version 460 core
        in vec2 UV;
        out vec4 FragColor;

        uniform sampler2D uTex;

        void main()
        {
            FragColor = texture(uTex, UV);
        })";
};

