/* =============================================================================
 * EntityMgr
 * 
 * 
 * ===========================================================================*/
#pragma once
#include "Entity.h"
#include <Engine/Core.h>
#include <Engine/Templates/Pool.h>

#include <vector>
#include <map>
#include <string>
#include <functional>

namespace SDG
{
    class SDG_API EntityMgr {
    public:
        explicit EntityMgr(size_t initPoolSize = 127u);
        ~EntityMgr();

        Entity &CreateEntity(const std::function<void(Entity &)>&factory);
        Entity &CreateEntity(std::string tag);
        void DestroyEntity(Entity &entity);

        void Update()
        {
            this->ProcessRemovals();

            for (auto &entity : active_)
            {
                entity->Update();
            }
        }

        void PostUpdate()
        {
            for (auto &entity : active_)
            {
                entity->PostUpdate();
            }
        }

        void Draw()
        {
            for (auto &entity : active_)
            {
                entity->Draw();
            }
        }

        void Close()
        {
            entities_.ReturnAll();
            active_.clear();
            tagLists_.clear();
        }

        Entity *GetFirstEntityWithTag(const std::string &tag)
        {
            if (tagLists_.contains(tag) && !tagLists_[tag].empty())
            {
                return *tagLists_[tag].begin();
            }
            else
            {
                return nullptr;
            }
        }

        const std::vector<Entity *> &GetEntityTagList(const std::string &tag) const
        {
            return tagLists_[tag];
        }

    private:
        Pool<Entity> entities_;
        std::vector<Entity *> active_;
        mutable std::map<std::string, std::vector<Entity *>> tagLists_;
        bool toDestroy_{false};

        // Destroys entities marked for destruction
        void ProcessRemovals();
    };
}



