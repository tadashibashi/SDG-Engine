/* =============================================================================
 * Entity
 * 
 * 
 * ===========================================================================*/
#include "Entity.h"

namespace SDG
{
    Entity::Entity():
        components_(new ComponentList), tag_()
    {
        components_->entity_ = this;
    }

    Entity::~Entity()
    {
    }
}