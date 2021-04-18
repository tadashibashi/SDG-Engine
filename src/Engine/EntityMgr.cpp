/* =============================================================================
 * EntityMgr
 * 
 * 
 * ===========================================================================*/
#include "EntityMgr.h"
#include "Log.h"
#include <algorithm>

namespace SDG
{
    EntityMgr::EntityMgr(size_t initPoolSize) : entities_(initPoolSize)
    {
    }

    EntityMgr::~EntityMgr()
    {
        Close();
    }

    Entity &EntityMgr::CreateEntity(std::string tag)
    {
        auto &e = CreateEntity([&tag](Entity &e)->void {
           e.SetTag(tag);
        });

        return e;
    }

    Entity &EntityMgr::CreateEntity(const std::function<void(Entity &)>&factory)
    {
        // Initialization
        auto *entity = entities_.CheckOut();
        entity->Init(); // ComponentList is created inside Entity, must be called before factory function

        // Perform "construction"
        factory(*entity);
        entity->InitComponents();

        toAdd_.emplace_back(entity);

        return *entity;
    }

    void EntityMgr::DestroyEntity(Entity &entity)
    {
        entity.toDestroy_ = true;
        this->toDestroy_ = true;
    }

    void EntityMgr::ProcessRemovals()
    {
        if (toDestroy_)
        {
            // Remove-erase all entities marked for destruction.
            active_.erase(
                    std::remove_if(active_.begin(), active_.end(),
                                   [this](Entity *entity) {

                                       // Check if this entity should be destroyed
                                       bool destroyThisEntity = entity->toDestroy_;
                                       if (destroyThisEntity)
                                       {
                                           // Remove from tagList if there is a tag
                                           if (!entity->GetTag().empty())
                                           {
                                                auto &tagList = tagLists_[entity->GetTag()];
                                                for (auto it = tagList.begin(), end = tagList.end();
                                                    it != end; ++it)
                                                {
                                                    if (entity == *it)
                                                    {
                                                        tagList.erase(it);
                                                        break;
                                                    }
                                                }
                                           }

                                           // Clean up entity
                                           entity->Close();
                                           entities_.Return(entity);
                                       }

                                       return destroyThisEntity;
                                   }),
                    active_.end());

            // Reset destroy flag.
            toDestroy_ = false;
        }
    }

    void EntityMgr::ProcessCreations()
    {
        for (auto &entity : toAdd_)
        {
            // Put into entity list
            active_.emplace_back(entity);

            // Put into the correct tag bank
            std::string tag = entity->GetTag();
            if (!tag.empty())
            {
                if (!tagLists_.contains(tag))
                {
                    tagLists_[tag] = std::move(std::vector<Entity *>());
                }

                tagLists_[tag].emplace_back(entity);
            }
        }

        toAdd_.clear();
    }

    void EntityMgr::AddExistingEntity(Entity &e)
    {
        Entity &entity = CreateEntity(e.GetTag());
        entity.Swap(e); // swap keeps pool id intact.
        entity.Components()->InitAll();
    }
}