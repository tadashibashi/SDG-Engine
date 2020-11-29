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
    class SDG_API Entity : public IPoolable {
        friend class EntityMgr;
    public:
        explicit Entity();
        ~Entity();

        void Init()
        {
            toDestroy_ = false;
            if (!components_)
            {
                components_ = new ComponentList;
                components_->entity_ = this;
            }

        }

        void Update()
        {
            components_->Update();
        }

        void PostUpdate()
        {
            components_->PostUpdate();
        }

        void Draw()
        {
            components_->Draw();
        }

        // Equivalent to destructor without destroying this object
        void Close()
        {
            if (components_)
            {
                components_->Close();
                delete components_;
                components_ = nullptr;
            }
        }

        void SetTag(std::string tag)
        {
            this->tag_ = std::move(tag);
        }

        [[nodiscard]] bool IsPersistent() const { return isPersistent_; }
        void SetPersistent(bool isPersistent) { isPersistent_ = isPersistent; }

        ComponentList *Components() { return components_; }
        [[nodiscard]] std::string GetTag() const { return tag_; }
    private:
        ComponentList *components_;
        std::string tag_;
        bool toDestroy_{false};
        bool isPersistent_{false};
    };

}



