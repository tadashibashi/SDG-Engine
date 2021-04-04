/* =============================================================================
 * SceneStore
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <typeindex>
#include <map>
#include "Scene.h"

template <typename T>
concept SceneType = requires { T(); } &&
                    std::is_base_of_v<SDG::Scene, T>;

namespace SDG
{
    class SceneCache {
    public:
        SceneCache();
        ~SceneCache();

        template <SceneType T>
        void RegisterScene()
        {
            if (scenes_.contains(typeid(T)))
            {
                SDG_ERR("SceneStore does not allow duplicate types of scenes");
            }
            else
            {
                scenes_[typeid(T)] = new T();
            }
        }

        template <SceneType T>
        T *GetScene()
        {
            return (scenes_.contains(typeid(T)))
                ? dynamic_cast<T *>(scenes_.at(typeid(T))) : nullptr;
        }

        [[nodiscard]] size_t Count() const
        {
            return scenes_.size();
        }

    private:
        std::map<std::type_index, Scene *> scenes_;
    };

}



