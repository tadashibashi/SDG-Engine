#pragma once

#include <deque>
#include <cassert>

namespace SDG {
    /* ========================================================================
     * struct PoolID
     * A struct that poolable objects must contain.
     * ======================================================================*/
    struct PoolID {
        PoolID() : id_(SIZE_MAX), inner_id_(SIZE_MAX), next_(SIZE_MAX), is_alive_(false)
        {}
        size_t id_;
        size_t inner_id_;
        size_t next_;
        bool is_alive_;

        // Compare id's
        bool operator==(const PoolID &other) const
        {
            return inner_id_ == other.inner_id_;
        }
    };

    /* ========================================================================
     * class IPoolable
     * All poolable classes must derive from this class.
     * ======================================================================*/
    class IPoolable {
    public:
        // Identification for management in Pool. Do not touch.
        PoolID pid;
    };

    template <typename T>
    concept Poolable = std::is_base_of_v<IPoolable, T>;

    /* ========================================================================
     * class Pool
     * Manages a pool of reusable objects. Prevent rapid construction/deletion.
     * Type T must have a default constructor and derive from IPoolable.
     * ======================================================================*/
    template <Poolable T>
    class Pool
    {
    public:
        explicit Pool(size_t init_cap)
                : free_list_(init_cap > 0 ? 0 : SIZE_MAX), v_(), ticket_(0)
        {
            ExpandPool(init_cap);
        }

        /**
         * Check out an object from the Pool.
         * @return ptr to a fresh object.
         */
        T* CheckOut()
        {
            if (free_list_ == SIZE_MAX) { // none free, expand pool.
                size_t next_free = v_.size();

                ExpandPool(v_.size() * 2 + 1);

                T *ret = &v_[next_free];
                ret->pid.is_alive_ = true;
                free_list_ = ret->pid.next_;

                return ret;
            } else {                      // free entities to take.
                T *ret = &v_[free_list_];
                ret->pid.is_alive_ = true;
                free_list_ = ret->pid.next_;

                return ret;
            }
        }

        /**
         * Relinquishes ownership of object back to the Pool.
         * Make sure any clean up is handled manually before returning.
         * @param item ptr to the object to put back.
         */
        void Return(T *item)
        {
            // Clean up object
            item->pid.is_alive_ = false;

            // Swap free list positions.
            item->pid.next_ = free_list_;
            free_list_ = item->pid.id_;
        }

        /**
         * Relinquishes ownership of all objects that have been previously
         * taken out.
         */
        void ReturnAll()
        {
            size_t vsize(CurrentMaxSize());
            for (size_t i = 0; i < vsize; ++i) {
                if (v_[i].pid.is_alive_) {
                    Return(&v_[i]);
                }
            }
        }

        [[nodiscard]]
        size_t CurrentMaxSize() const
        {
            return v_.size();
        }

    private:
        void ExpandPool(size_t new_size)
        {
            assert(new_size >= v_.size());
            for (size_t i = v_.size(); i < new_size; ++i) {
                v_.emplace_back();
                T &obj = v_.back();
                obj.pid.id_ = i;
                obj.pid.inner_id_ = ticket_++;
                obj.pid.next_ = (i < new_size - 1) ? i + 1 : SIZE_MAX;
            }
        }

        size_t free_list_, ticket_;
        std::deque<T> v_;
    };
}
