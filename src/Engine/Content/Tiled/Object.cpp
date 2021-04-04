#include "Object.h"
#include <tinyxml2.h>
#include <vector>
#include <stdexcept>

namespace SDG::Tiled
{
    Object Object::Parse(tinyxml2::XMLElement *element, const std::string &rootPath)
    {
        Object obj;
        int id;
        float x, y, width, height, rotation;
        const char *name, *type, *templateName;

        bool hasTemplate = false;

        Object oTemplate;

        // Template Parsing
        tinyxml2::XMLError result = element->QueryAttribute("template", &templateName);
        if (result == tinyxml2::XML_SUCCESS)
        {
            hasTemplate = true;

            tinyxml2::XMLDocument doc;
            std::string path = rootPath.substr(0,
               rootPath.find_last_of('/') + 1) + templateName;
            result = doc.LoadFile(path.c_str());
            if (result != tinyxml2::XML_SUCCESS)
            {
                throw std::runtime_error("Failed to open Tiled Template file at: " +
                    path);
            }

            tinyxml2::XMLElement *root = doc.RootElement();
            if (!root)
            {
                throw std::runtime_error("Failed to get root element from Tiled template "
                                   "file at path: " + path);
            }

            tinyxml2::XMLElement *objectElement = root->FirstChildElement("object");
            if (!objectElement)
            {
                throw std::runtime_error("Failed to get object element from Tiled "
                                   "template at path: " + path);
            }

            oTemplate = Object::Parse(objectElement, path);
        }

        // Parse object info

        // id
        result = element->QueryAttribute("id", &id);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                id = oTemplate.id;
            else
                id = -1;
        }

        // name
        result = element->QueryAttribute("name", &name);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                name = oTemplate.name.c_str();
            else
                name = "";
        }

        // type
        result = element->QueryAttribute("type", &type);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                type = oTemplate.type.c_str();
            else
                type = "";
        }

        // x
        result = element->QueryAttribute("x", &x);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                x = oTemplate.x;
            else
                x = 0;
        }

        // y
        result = element->QueryAttribute("y", &y);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                y = oTemplate.y;
            else
                y = 0;
        }

        // width
        result = element->QueryAttribute("width", &width);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                width = oTemplate.width;
            else
                width = 0;
        }

        // height
        result = element->QueryAttribute("height", &height);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                height = oTemplate.height;
            else
                height = 0;
        }

        // rotation
        result = element->QueryAttribute("rotation", &rotation);
        if (result != tinyxml2::XML_SUCCESS)
        {
            if (hasTemplate)
                rotation = oTemplate.rotation;
            else
                rotation = 0;
        }

        // Parse custom properties
        std::map<std::string, Property> props;
        {
            tinyxml2::XMLElement *propsElement = element->FirstChildElement("properties");
            if (propsElement)
            {
                for (tinyxml2::XMLElement *it = propsElement->FirstChildElement();
                    it != nullptr; it = it->NextSiblingElement())
                {
                    Property p = Property::Parse(it);
                    props[p.GetName()] = std::move(p);
                }
            }

            // Add any properties in template not already contained by current props
            if (hasTemplate)
            {
                for (auto &[k, v]: oTemplate.properties)
                {
                    if (!props.contains(k))
                    {
                        props[k] = v;
                    }
                }
            }
        }

        // Commit Data
        obj.id = id;
        obj.name = name;
        obj.type = type;
        obj.x = x;
        obj.y = y;
        obj.width = width;
        obj.height = height;
        obj.rotation = rotation;
        obj.properties.swap(props);

        return obj;
    }

    void Object::Log()
    {
        SDG_LOG("========Object========");
        SDG_LOG("id:", id);
        SDG_LOG("name: \"" + name + "\"");
        SDG_LOG("type: \"" + type + "\"");
        SDG_LOG("x:", x);
        SDG_LOG("y:", y);
        SDG_LOG("width:", width);
        SDG_LOG("height:", height);
        SDG_LOG("rotation:", rotation);
        SDG_LOG("---other-properties---");
        for(auto &[k, v]: properties)
        {
            v.Log();
        }
    }
}