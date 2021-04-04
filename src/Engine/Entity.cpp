/* =============================================================================
 * Entity
 * 
 * 
 * ===========================================================================*/
#include "Entity.h"
#include <algorithm>

namespace SDG
{
    Entity::Entity(): components_(new ComponentList), tag_()
    {
        components_->entity_ = this;
    }

    Entity::~Entity()
    {
        this->Close();
        delete components_;
    }

    void Entity::Init()
    {
        toDestroy_ = false;
        isPersistent_ = false;
        if (!components_) // just in case... I don't think we need this since constructor handles it.
        {
            components_ = new ComponentList;
            components_->entity_ = this;
        }

    }

    void Entity::Swap(Entity &other)
    {
        std::swap(this->toDestroy_, other.toDestroy_);
        std::swap(this->components_, other.components_);
        std::swap(this->isPersistent_, other.isPersistent_);
        std::swap(this->tag_, other.tag_);
    }
}