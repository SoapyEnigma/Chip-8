#include "Shader.h"

#include "glad/glad.h"

Shader::Shader()
{
    CreateProgram();
}

Shader::~Shader()
{
    glDeleteProgram(_program);
}

void Shader::Draw()
{
    glUseProgram(_program);
    glUniform1i(glGetUniformLocation(_program, "uTex"), 0);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Shader::CreateProgram()
{
    GenBuffers();

    u32 vert = Compile(_vertShader);
    u32 frag = Compile(_fragShader);

    _program = glCreateProgram();
    glAttachShader(_program, vert);
    glAttachShader(_program, frag);
    glLinkProgram(_program);

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::GenBuffers()
{
    glGenVertexArrays(1, &_vao);

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_verts), _verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 Shader::Compile(const char* shaderFile)
{
    u32 shader;
    if (shaderFile == _vertShader)
        shader = glCreateShader(GL_VERTEX_SHADER);
    else if (shaderFile == _fragShader)
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    else
    {
        // Fails
        return -1;
    }

    glShaderSource(shader, 1, &shaderFile, nullptr);
    glCompileShader(shader);

    return shader;
}
