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
        Component(bool isUpdatable, bool isDrawable): attr_(0), owner_(nullptr)
        {
            if (isUpdatable) attr_ |= Updatable;
            if (isDrawable) attr_  |= Drawable;
            attr_ |= Active;
        }

        // Cannot copy
        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;

        virtual ~Component() = default;

        virtual void PostUpdate() { }
        virtual void Draw() { }
        virtual void Close() { }

        static void Provide(SpriteBatch *spriteBatch) { spriteBatch_ = spriteBatch; }
        static void Provide(const GameTime *gameTime) { gameTime_ = gameTime; }
        static void Provide(ContentMgr *content) { contentMgr_ = content; }
        static void Provide(InputMgr *inputMgr) { inputMgr_ = inputMgr; }
        static void Provide(GraphicsDeviceMgr *graphics) { graphics_ = graphics; }

        [[nodiscard]] bool IsDrawable() const  { return attr_ & Drawable; }
        [[nodiscard]] bool IsUpdatable() const { return attr_ & Updatable; }
        [[nodiscard]] bool IsActive() const    { return attr_ & Active; }
        [[nodiscard]] bool IsRemoving() const  { return attr_ & ToBeRemoved; }

        [[nodiscard]] ComponentList *Owner() const { return owner_; }

        void SetActive(bool active)
        {
            if (active)
            {
                attr_ |= Active;
            }
            else
            {
                attr_ &= ~Active;
            }
        }

        void SetRemoving(bool removing)
        {
            if (removing)
            {
                attr_ |= ToBeRemoved;
            }
            else
            {
                attr_ &= ~ToBeRemoved;
            }
        }

        virtual void Init() { }
        virtual void Update() { }

    protected:
        // Gets a component that is exactly the same type as template arg or
        // nullptr if there are none.
        template <typename T>
        [[nodiscard]] T *GetComponent() const
        {
            assert(Owner() != nullptr);
            return Owner()->Get<T>();
        }

        // Gets a component that is either the same type or a child of arg or
        // nullptr if there are none.
        template <typename T>
        [[nodiscard]] T *GetTypeof() const
        {
            assert(Owner() != nullptr);
            return Owner()->GetTypeof<T>();
        }

        template <typename T>
        void RemoveComponent()
        {
            assert(Owner() != nullptr);
            Owner()->Remove<T>();
        }

        [[nodiscard]] static SpriteBatch *GetSpriteBatch()
        {
            assert(spriteBatch_ != nullptr);
            return spriteBatch_;
        }

        [[nodiscard]] static const GameTime *GetTime()
        {
            assert(gameTime_ != nullptr);
            return gameTime_;
        }

        [[nodiscard]] static ContentMgr *GetContent()
        {
            assert(contentMgr_ != nullptr);
            return contentMgr_;
        }

        [[nodiscard]] static InputMgr *GetInput()
        {
            assert(inputMgr_ != nullptr);
            return inputMgr_;
        }

        [[nodiscard]] static GraphicsDeviceMgr *GetGraphicsDeviceMgr()
        {
            assert(graphics_ != nullptr);
            return graphics_;
        }

        [[nodiscard]] Entity *GetEntity()
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