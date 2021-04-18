/* =============================================================================
 * Entity
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Components/ComponentList.h>

#include <string>
#include <Engine/Templates/Pool.h>

namespace SDG
{
    // Handle for a container of components
    class Entity : public IPoolable {
        friend class EntityMgr;
    public:
        explicit Entity();
        ~Entity();

        void Init();
        void InitComponents();
        void Update()
        {
            components_->Update();
        }

        // Effectively allows you to swap components and state with another
        // Entity while each maintains its owned-by relationship to its
        // respective Pool.
        void Swap(Entity &other);

        void PostUpdate()
        {
            components_->PostUpdate();
        }

        void Draw()
        {
            components_->Draw();
        }

        // Resets the Entity. Equivalent to dtor without deleting this object from memory.
        // NOTE: Do not call if the object is persistent, since it will unset its persistency.
        void Close()
        {
            RefreshEntity();
            wasInit_ = false;
        }

        void RefreshEntity()
        {
            toDestroy_ = false;
            isPersistent_ = false;

            if (components_)
                components_->Close();
        }

        void SetTag(std::string tag)
        {
            this->tag_ = std::move(tag);
        }

        [[nodiscard]]
        bool IsPersistent() const { return isPersistent_; }

        // Setting to true prevents this entity from being destroyed on Scene changes.
        void SetPersistent(bool isPersistent)
        {
            isPersistent_ = isPersistent;
        }

        [[nodiscard]]
        bool WasInitialized() const { return wasInit_; }

        // Accesses this entity's ComponentList
        ComponentList *Components() { return components_; }

        // Gets the entity's string tag. E.g. "Player", "Enemy", etc.
        [[nodiscard]]
        std::string GetTag() const { return tag_; }
    private:
        ComponentList *components_;
        std::string tag_;
        bool toDestroy_{false};
        bool isPersistent_{false};
        bool wasInit_{false};
    };
}
