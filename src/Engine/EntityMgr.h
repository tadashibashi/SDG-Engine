/* =============================================================================
 * EntityMgr
 * 
 * 
 * ===========================================================================*/
#pragma once
#include "Entity.h"
#include <Engine/Core.h>
#include <Engine/Templates/Pool.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace SDG
{
    // Houses and manages a pool of Entities
    class EntityMgr {
    public:
        explicit EntityMgr(size_t initPoolSize = 127u);
        ~EntityMgr();

        Entity &CreateEntity(const std::function<void(Entity &)>&factory);
        Entity &CreateEntity(std::string tag = "");
        void DestroyEntity(Entity &entity);

        std::vector<Entity *>::iterator begin() { return active_.begin(); }
        std::vector<Entity *>::iterator end()   { return active_.end(); }

        // Swaps entity on the free store into this entity manager. Relinquishes ownership.
        void AddExistingEntity(Entity &e);

        void ProcessChanges()
        {
            this->ProcessRemovals();
            this->ProcessCreations();
        }

        // Updates all active entities.
        void Update()
        {
            ProcessChanges();

            for (Entity *entity : active_)
            {
                entity->Update();
            }
        }

        void PostUpdate()
        {
            for (Entity *entity : active_)
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
            this->ProcessRemovals();
            this->ProcessCreations();

            for (auto &activeEntity: active_)
            {
                if (!activeEntity->IsPersistent())
                    activeEntity->Close();
            }

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
        std::vector<Entity *> toAdd_;
        mutable std::map<std::string, std::vector<Entity *>> tagLists_;
        bool toDestroy_{false};

        // Destroys entities marked for destruction
        void ProcessRemovals();
        void ProcessCreations();
    };
}



