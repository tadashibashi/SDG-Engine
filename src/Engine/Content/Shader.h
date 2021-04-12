/* =============================================================================
 * Shader
 * A class that manages a GLSL shader compilation, linkage, usage, and cleanup.
 * ===========================================================================*/
#pragma once
#include <string>
#include <Engine/GraphicsLibrary.h>

namespace SDG
{
    class Shader {
    public:
        // ========== CTOR/DTOR ==========

        // Create an empty shader object.
        Shader();
        // Create and load shader from vertex and fragment shader paths.
        Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath);
        ~Shader();

        // ========== LOADING/UNLOADING ==========

        // Loads and compiles a shader program with vertex and fragment shaders.
        // Returns true if it succeeded and false if it did not.
        bool Load(const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath);
        // Loads and compiles a shader program from raw text strings.
        // Returns true if it succeeded and false if not.
        bool LoadFromString(const std::string &vertexString,
                            const std::string &fragmentString);
        // Links the program after compilation. Displays errors to the console.
        // On success, the shader is ready for use.
        bool LinkProgram();

        // Unloads and cleans up shader program.
        void Unload();

        // ========== VARIABLE ATTRIBUTES ==========

        // Gets the uniform var ptr location or -1 on error.
        GLint GetUniformLocation(const std::string &varname) const;

        // Set an attribute correlating to shader code.
        // Set this before shader usage.
        void AddAttribute(const std::string &attribName) const;

        // ========== USAGE ==========

        // Use as current shader.
        void Use() const;
        // Remove shader from usage.
        void Unuse() const;

        bool IsLoaded() const { return program != 0; }

    private:
        // Compiles one shader. Returns the GLuint address if successful or 0
        // on failure.
        GLuint CompileShaderFromFile(const std::string &shaderPath,
                                     GLenum shaderType);
        // Compiles shader from raw string.
        GLuint CompileShaderString(const char *shaderStr, GLenum shaderType);

        GLuint program{0}, vertexShader{0}, fragmentShader{0};
        mutable GLuint attributeNumber{0};
    };
}
