/* =============================================================================
 * Shader
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>
#include <Engine/GL.h>

namespace SDG
{
    // A class that compiles and links text written in GLSL at runtime into a
    // program that runs on the gpu.
    class SDG_API Shader {
    public:
        Shader();
        Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath);
        ~Shader();

        // Loads and compiles a shader program with vertex and fragment shaders.
        // Returns true if it succeeded and false if it did not.
        bool Load(const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath);
        bool LoadFromString(const std::string &vertexString,
                            const std::string &fragmentString);
        bool LinkProgram();
        void Unload();

        // Gets the uniform var ptr location or -1 on error.
        GLint GetUniformLocation(const std::string &varname) const;
        void AddAttribute(const std::string &attribName) const;

        void Use() const;
        void Unuse() const;

        bool IsLoaded() const { return program != 0; }

    private:
        // Compiles one shader. Returns the GLuint address if successful or 0
        // on failure.
        GLuint CompileShaderFromFile(const std::string &shaderPath,
                                     GLenum shaderType);
        GLuint CompileShaderString(const char *shaderStr, GLenum shaderType);
        GLuint program{0}, vertexShader{0}, fragmentShader{0};

        mutable GLuint attributeNumber{0};
    };

}



