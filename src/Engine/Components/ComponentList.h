/* =============================================================================
 * ComponentList
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <map>
#include <vector>
#include <typeindex>
#include <stdexcept>

namespace SDG
{
    class Component;
    class Entity;

    class  ComponentList {
        friend class Entity;
    public:
        ComponentList();
        ~ComponentList();

        // Adding a component initializes it immediately. This means that all
        // dependencies must be attached to ComponentList in the correct order.
        template <typename T, typename... TArgs> requires
        std::is_base_of_v<Component, T> &&
        requires (TArgs && ...args) { T(std::forward<TArgs>(args)...); }
        T &Add(TArgs && ...args)
        {
            if (map_.contains(typeid(T)))
            {
                throw std::logic_error("Cannot add multiple of the same type components.");
            }

            T *component = new T(std::forward<TArgs>(args)...);
            component->owner_ = this;

            if (GetEntity() && GetEntityWasInit())
                component->Init();

            if (component->IsUpdatable())
                updatable_.emplace_back(component);

            if (component->IsDrawable())
                drawable_.emplace_back(component);

            map_[typeid(T)] = component;

            return *component;
        }

        void InitAll();

        // Get a component from this container, or nullptr if there are none.
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] T *Get() const
        {
            return (map_.count(typeid(T)) > 0)
                ? dynamic_cast<T *>(map_.at(typeid(T)))
                : nullptr;
        }

        [[nodiscard]] Component *Get(const std::type_index &type) const
        {
            return (map_.contains(type)) ? map_.at(type) : nullptr;
        }

        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] T *GetTypeof() const
        {
            T *ret;
            for (auto &[k, v]: map_)
            {
                if ((ret = dynamic_cast<T *>(v)))
                {
                    return ret;
                }
            }

            return nullptr;
        }

        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] bool Has() const
        {
            return map_.contains(typeid(T));
        }

        [[nodiscard]] bool Has(const std::type_index &type) const
        {
            return map_.contains(type);
        }

        // Removes and deletes a component from this container next frame.
        template <typename T> requires std::is_base_of_v<Component, T>
        void Remove()
        {
            auto it = map_.find(typeid(T));
            if (it != map_.end())
            {
                T *&c = it->second;
                c->SetRemoving(true);

                this->isRemoving_ |= 1u;

                if (c->IsUpdatable())
                    this->isRemoving_ |= 1u << 1u;
                if (c->IsDrawable())
                    this->isRemoving_ |= 1u << 2u;
            }
        }

        void Update();
        void PostUpdate();
        void Draw();

        // Gets the number of components in this container.
        [[nodiscard]]
        size_t GetLength() const { return map_.size(); }

        void Close();

        [[nodiscard]]
        Entity *GetEntity() { return entity_; }

        [[nodiscard]]
        bool GetEntityWasInit() const;

    private:
        std::vector<Component *> updatable_;
        std::vector<Component *> drawable_;
        std::map<std::type_index, Component *> map_;
        unsigned char isRemoving_;
        Entity *entity_;
        void ProcessRemovals();
    };
}



