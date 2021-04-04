/* =============================================================================
 * Shader
 * 
 * 
 * ===========================================================================*/
#include "Shader.h"
#include <Engine/File/IO.h>
#include <Engine/Log.h>

namespace SDG
{
    Shader::Shader()
    {
        program = glCreateProgram();
    }

    Shader::~Shader()
    {
        if (program)
        {
            glDeleteProgram(program);
        }

        if (vertexShader)
        {
            glDeleteShader(vertexShader);
        }

        if (fragmentShader)
        {
            glDeleteShader(fragmentShader);
        }
    }

    Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
    {
        Load(vertexShaderPath, fragmentShaderPath);
    }

    bool
    Shader::Load(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
    {
        GLuint vertex = CompileShaderFromFile(vertexShaderPath, GL_VERTEX_SHADER);
        if (!vertex)
        {
            SDG_ERR("Vertex Shader failed to compile.");
            return false;
        }

        GLuint fragment = CompileShaderFromFile(fragmentShaderPath, GL_FRAGMENT_SHADER);
        if (!fragment)
        {
            SDG_ERR("Fragment Shader failed to compile.");
            glDeleteShader(vertex);
            return false;
        }

        vertexShader = vertex;
        fragmentShader = fragment;

        return true;
    }

    void
    Shader::Use() const
    {
        SDG_ASSERT(IsLoaded());

        glUseProgram(program);

        // Enable each attribute. Necessary on some graphics cards.
        // Most should be okay without this loop.
        for (GLuint i = 0; i < attributeNumber; ++i)
        {
            glEnableVertexAttribArray(i);
        }

    }

    void
    Shader::Unuse() const
    {
        glUseProgram(0);

        // Disable each attribute. Necessary on some graphics cards.
        // Most should be okay without this loop.
        for (GLuint i = 0; i < attributeNumber; ++i)
        {
            glDisableVertexAttribArray(i);
        }
    }

    void
    Shader::Unload()
    {
        glDeleteProgram(program);
        program = 0;
        attributeNumber = 0;
    }

    GLuint
    Shader::CompileShaderFromFile(const std::string &shaderPath, GLenum shaderType)
    {

        std::vector<unsigned char> buffer;
        if (!IO::ReadFile(shaderPath, &buffer))
        {
            SDG_ERR("Shader at file ", shaderPath, "failed to load.");
            return 0;
        }

        return CompileShaderString((char *)buffer.data(), shaderType);
    }

    GLuint
    Shader::CompileShaderString(const char *shaderString, GLenum shaderType)
    {
        // Create shader and load source code string.
        GLuint shader = glCreateShader(shaderType);
        if (!shader)
        {
            SDG_ERR("Failed to create gl shader object. glError code:",
                    glGetError());
            return 0;
        }

        glShaderSource(shader, 1, &shaderString, nullptr); // null makes it stop at the first null terminator instead of traversing an array of strings.

        // Compile shader source and check for errors.
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            char resultMessage[512];
            glGetShaderInfoLog(shader, 512, nullptr, resultMessage);
            SDG_ERR("Shader compilation error: {0}", resultMessage);
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLint
    Shader::GetUniformLocation(const std::string &varname) const
    {
        SDG_ASSERT(IsLoaded());

        GLint location = glGetUniformLocation(program, 
            static_cast<const GLchar *>(varname.c_str()));

        if (location == GL_INVALID_VALUE)
        {
            SDG_ERR("Could not find the location of uniform ", varname,
                    "in shader program.");
            return -1;
        }

        return location;
    }

    void
    Shader::AddAttribute(const std::string &attribName) const
    {
        SDG_ASSERT(IsLoaded());

        glBindAttribLocation(program, attributeNumber++,
            static_cast<const GLchar *>(attribName.c_str()));
    }

    bool
    Shader::LoadFromString(const std::string &vertexString, const std::string &fragmentString)
    {
        GLuint vertex = CompileShaderString(vertexString.c_str(), GL_VERTEX_SHADER);
        if (!vertex)
        {
            SDG_CORE_ERR("Vertex Shader failed to compile.");
            return false;
        }

        GLuint fragment = CompileShaderString(fragmentString.c_str(), GL_FRAGMENT_SHADER);
        if (!fragment)
        {
            SDG_ERR("Fragment Shader failed to compile.");
            glDeleteShader(vertex);
            return false;
        }

        vertexShader = vertex;
        fragmentShader = fragment;
        return true;
    }

    bool
    Shader::LinkProgram()
    {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Check for errors.
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLchar buffer[512];
            glGetProgramInfoLog(program, 512, nullptr, buffer);

            SDG_ERR("Shader program failed to link:", buffer);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            vertexShader = 0;
            fragmentShader = 0;
            glDeleteProgram(program);

            return false;
        }

        // Shaders are no longer needed once the program has compiled.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        vertexShader = 0;
        fragmentShader = 0;

        return true;
    }
}
