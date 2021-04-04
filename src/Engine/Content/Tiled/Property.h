/* =============================================================================
 * PropertyType
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <string>
#include <Engine/Graphics/Color.h>
#include <any>
#include <utility>

// Forward declaration
namespace tinyxml2
{
    class XMLElement;
}

namespace SDG::Tiled
{
    struct Property
    {
    private:
        std::string name;

        enum class Type
        {
            Boolean, // bool
            String,  // no type field
            File,    // file
            Color,   // color
            Integer, // int
            Float,   // float
            Undefined
        } type;

        std::any value;

    public:
        static Property Parse(tinyxml2::XMLElement *property);
        void Log();

        Property(std::string name, Type type, std::any value):
            name(std::move(name)), type(type), value(std::move(value))
        {}

        Property(): name(), type(Type::Undefined), value() { }

        [[nodiscard]] std::string GetName() const
        {
            return name;
        }

        [[nodiscard]] Type GetType() const
        {
            return type;
        }

        bool GetBool(bool *val) const;
        bool GetString(std::string *str) const;
        bool GetFile(std::string *filePath) const;
        bool GetColor(Color *color) const;
        bool GetInt(int *num) const;
        bool GetFloat(float *num) const;
    };

}