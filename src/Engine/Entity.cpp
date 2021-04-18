/* =============================================================================
 * Entity
 * 
 * 
 * ===========================================================================*/
#include "Entity.h"
#include <algorithm>

namespace SDG
{
    Entity::Entity(): components_(nullptr), tag_(), toDestroy_(false), isPersistent_(false)
    {
    }

    Entity::~Entity()
    {
        if (!IsPersistent())
        {
            this->Close();

            delete components_;
        }
    }

    void Entity::Init()
    {
        if (!components_)
        {
            components_ = new ComponentList;
        }

        // Keep this outside to reset the componentlist's owner for persistent Entities
        components_->entity_ = this;
    }

    // Called after factory creation
    void Entity::InitComponents()
    {
        components_->InitAll();
        wasInit_ = true;
    }

    void Entity::Swap(Entity &other)
    {
        std::swap(this->toDestroy_, other.toDestroy_);
        std::swap(this->components_, other.components_);
        std::swap(this->isPersistent_, other.isPersistent_);
        std::swap(this->tag_, other.tag_);
    }
}