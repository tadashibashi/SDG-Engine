/* =============================================================================
 * ComponentList
 * 
 * 
 * ===========================================================================*/
#include "ComponentList.h"
#include "Component.h"
#include <algorithm>
#include <Engine/Entity.h>

namespace SDG
{
    ComponentList::ComponentList() : isRemoving_(false), entity_(nullptr)
    {
    }

    ComponentList::~ComponentList()
    {
        Close();
    }

    void ComponentList::Update()
    {
        // Process removals from last frame before updating this one.
        this->ProcessRemovals();

        // Call Update on all components in the update list.
        for (Component *c : updatable_)
        {
            c->Update();
        }
    }

    void ComponentList::PostUpdate()
    {
        // Call PostUpdate on all components in the update list.
        for (auto &component : updatable_)
        {
            component->PostUpdate();
        }
    }

    void ComponentList::Draw()
    {
        // Call Draw on all components in the draw list.
        for (Component *c : drawable_)
        {
            c->Draw();
        }
    }

    void ComponentList::Close()
    {
        for (auto &[k, v]: map_)
        {
            v->Close();
            delete v;
        }

        map_.clear();
        updatable_.clear();
        drawable_.clear();
    }



    void ComponentList::ProcessRemovals()
    {
        // Process removals
        if (isRemoving_)
        {
            if (isRemoving_ & (1u << 1u)) // Updatable
            {
                // Remove-erase all components in update list marked for removal.
                updatable_.erase(
                    std::remove_if(updatable_.begin(), updatable_.end(),
                                        [](const Component *c) {
                                            return c->IsRemoving();
                                        }),
                    updatable_.end());
            }

            if (isRemoving_ & (1u << 2u)) // Drawable
            {
                // Remove-erase all components in draw list marked for removal.
                drawable_.erase(
                    std::remove_if(drawable_.begin(), drawable_.end(),
                                        [](const Component *c) {
                                            return c->IsRemoving();
                                        }),
                    drawable_.end());
            }

            // Clear the map of any components marked for removal.
            for (auto &[k, v]: map_)
            {
                if (v->IsRemoving())
                {
                    v->Close();
                    delete v;
                }
            }

            // Clear all removal flags
            isRemoving_ = 0;
        }
    }

    void ComponentList::InitAll()
    {
        for (auto &[k, v] : map_)
        {
            v->Init();
        }
    }


}