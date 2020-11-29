/* =============================================================================
 * Property
 * 
 * 
 * ===========================================================================*/
#include "Property.h"
#include <Engine/Log.h>
#include <tinyxml2/tinyxml2.h>

namespace SDG::Tiled
{
    bool Property::GetBool(bool *val) const
    {
        if (type == Type::Boolean)
        {
            *val = std::any_cast<bool>(value);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Property::GetString(std::string *str) const
    {
        if (type == Type::String)
        {
            *str = std::any_cast<std::string>(value);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Property::GetFile(std::string *filePath) const
    {
        if (type == Type::File)
        {
            *filePath = std::any_cast<std::string>(value);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Property::GetColor(Color *color) const
    {
        if (type == Type::Color)
        {
            *color = std::any_cast<Color>(value);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Property::GetInt(int *num) const
    {
        if (type == Type::File)
        {
            *num = std::any_cast<int>(value);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Property::GetFloat(float *num) const
    {
        if (type == Type::File)
        {
            *num = std::any_cast<int>(value);
            return true;
        }
        else
        {
            return false;
        }
    }

    Property Property::Parse(tinyxml2::XMLElement *property)
    {
        const char *name, *type;
        tinyxml2::XMLError result;

        // Query name
        result = property->QueryAttribute("name", &name);
        if (result != tinyxml2::XML_SUCCESS)
        {
            throw std::runtime_error("Failed to parse Tiled Property name.");
        }

        // Query type
        result = property->QueryAttribute("type", &type);
        if (result != tinyxml2::XML_SUCCESS)
        {
            type = "string";
        }

        // Query value
        std::string typeStr(type);  // type as a string for easy comparison
        std::any value;             // temp value to store
        Type valueType;
        if (typeStr == "color")
        {
            const char *colorStr;
            result = property->QueryAttribute("value", &colorStr);
            if (result != tinyxml2::XML_SUCCESS)
            {
                throw std::runtime_error("Failed to parse Tiled Property "
                                         "color value.");
            }

            size_t len = std::strlen(colorStr);
            if (len == 7)
            {
                value = Color::FromRGBString(colorStr);
            }
            else if (len == 9)
            {
                value = Color::FromRGBAString(colorStr);
            }
            else
            {
                throw std::runtime_error("Failed to parse Tiled Property"
                                         "color: Invalid color string \""
                                         + std::string(colorStr) + "\""
                                         " must match format #rrggbbaa"
                                         " or #rrggbb");
            }

            valueType = Type::Color;
        }
        else if (typeStr == "float")
        {
            float pFloat;
            result = property->QueryAttribute("value", &pFloat);
            if (result != tinyxml2::XML_SUCCESS)
            {
                throw std::runtime_error("Failed to parse Tiled Property "
                                         "float value.");
            }

            value = pFloat;
            valueType = Type::Float;
        }
        else
        if (typeStr == "int")
        {
            int pInt;
            result = property->QueryAttribute("value", &pInt);
            if (result != tinyxml2::XML_SUCCESS)
            {
                throw std::runtime_error("Failed to parse Tiled Property "
                                         "int value.");
            }

            value = pInt;
            valueType = Type::Integer;
        }
        else
        if (typeStr == "bool")
        {
            const char *pBool;
            result = property->QueryAttribute("value", &pBool);
            if (result != tinyxml2::XML_SUCCESS)
            {
                throw std::runtime_error("Failed to parse Tiled Property "
                                         "int value.");
            }

            value = (std::strcmp(pBool, "true") == 0);
            valueType = Type::Boolean;
        }
        else
        if (typeStr == "string" || typeStr == "file")
        {
            const char *pString;
            result = property->QueryAttribute("value", &pString);
            if (result != tinyxml2::XML_SUCCESS)
            {
                throw std::runtime_error("Failed to parse Tiled Property "
                                         "string value.");
            }

            value = std::string(pString);
            valueType = typeStr == "string" ? Type::String : Type::File;
        }

        return Property(name, valueType, value);
    }

    void Property::Log()
    {
        SDG_LOG("\"" + name + "\"");

        switch(type)
        {
            case Type::Color:
                SDG_LOG("type: Color, value: " + std::any_cast<Color>(value).ToString());
                break;
            case Type::Boolean:
                SDG_LOG("type: Boolean, value: " +
                    std::string(std::any_cast<bool>(value) ? "true" : "false"));
                break;
            case Type::File:
                SDG_LOG("type: File, value:", std::any_cast<std::string>(value));
                break;
            case Type::String:
                SDG_LOG("type: String, value: " + std::any_cast<std::string>(value));
                break;
            case Type::Float:
                SDG_LOG("type: Float, value:", std::any_cast<float>(value));
                break;
            case Type::Integer:
                SDG_LOG("type: Integer, value:", std::any_cast<int>(value));
                break;
            case Type::Undefined:
            default:
                SDG_LOG("type: Unknown, value: Undefined");
                break;
        }
    }
}
