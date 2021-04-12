/* =============================================================================
 * Component
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Time/GameTime.h>
#include <Engine/Graphics/SpriteBatch.h>
#include <Engine/Content/ContentMgr.h>
#include <Engine/Input/InputMgr.h>
#include <Engine/Components/ComponentList.h>
#include <Engine/Graphics/GraphicsDeviceMgr.h>

namespace SDG
{
    class Component
    {
        friend class ComponentList;
        enum : unsigned char
        {
            Updatable   = 1u << 0u,
            Drawable    = 1u << 1u,
            Active      = 1u << 2u,
            ToBeRemoved = 1u << 3u,
        };
    public:
        Component(bool isUpdatable, bool isDrawable): attr_{}, owner_{}
        {
            if (isUpdatable) attr_ |= Updatable;
            if (isDrawable) attr_  |= Drawable;
            attr_ |= Active;
        }

        // Cannot copy
        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
        virtual ~Component() = default;

        // ===== Service Provider Functions =====
        static void Provide(SpriteBatch *spriteBatch) { spriteBatch_ = spriteBatch; }
        static void Provide(const GameTime *gameTime) { gameTime_ = gameTime; }
        static void Provide(ContentMgr *content) { contentMgr_ = content; }
        static void Provide(InputMgr *inputMgr) { inputMgr_ = inputMgr; }
        static void Provide(GraphicsDeviceMgr *graphics) { graphics_ = graphics; }

        // ===== Attributes / Properties =====
        [[nodiscard]]
        bool IsDrawable() const  { return attr_ & Drawable; }
        [[nodiscard]]
        bool IsUpdatable() const { return attr_ & Updatable; }
        [[nodiscard]]
        bool IsActive() const    { return attr_ & Active; }
        [[nodiscard]]
        bool IsRemoving() const  { return attr_ & ToBeRemoved; }

        [[nodiscard]]
        ComponentList *Owner() const { return owner_; }

        void SetActive(bool active);
        void SetRemoving(bool removing);

        // ===== Events =====

        virtual void Init() { }
        virtual void Update() { }
        virtual void PostUpdate() { }
        virtual void Draw() { }
        virtual void Close() { }

    protected:
        // ===== Convenience Getters =====

        // Gets a component that is exactly the same type as template arg or
        // nullptr if there are none.
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]]
        T *GetComponent() const
        {
            SDG_ASSERT(Owner());
            return Owner()->Get<T>();
        }

        // Gets a component that is either the same type or a child of arg or
        // nullptr if there are none.
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]]
        T *GetTypeof() const
        {
            SDG_ASSERT(Owner());
            return Owner()->GetTypeof<T>();
        }

        template <typename T> requires std::is_base_of_v<Component, T>
        void RemoveComponent()
        {
            SDG_ASSERT(Owner());
            Owner()->Remove<T>();
        }

        [[nodiscard]]
        static SpriteBatch *GetSpriteBatch()
        {
            SDG_ASSERT(spriteBatch_);
            return spriteBatch_;
        }

        [[nodiscard]]
        static const GameTime *GetTime()
        {
            SDG_ASSERT(gameTime_);
            return gameTime_;
        }

        [[nodiscard]]
        static ContentMgr *GetContent()
        {
            SDG_ASSERT(contentMgr_);
            return contentMgr_;
        }

        [[nodiscard]]
        static InputMgr *GetInput()
        {
            SDG_ASSERT(inputMgr_);
            return inputMgr_;
        }

        [[nodiscard]]
        static GraphicsDeviceMgr *GetGraphicsDeviceMgr()
        {
            SDG_ASSERT(graphics_);
            return graphics_;
        }

        [[nodiscard]]
        Entity *GetEntity()
        {
            SDG_ASSERT(owner_);
            SDG_ASSERT(owner_->GetEntity());
            
            return owner_->GetEntity();
        }

    private:
        unsigned char attr_;
        ComponentList *owner_;

        static SpriteBatch *spriteBatch_;
        static const GameTime *gameTime_;
        static ContentMgr *contentMgr_;
        static InputMgr *inputMgr_;
        static GraphicsDeviceMgr *graphics_;
    };
}