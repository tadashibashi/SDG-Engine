#pragma once

#include <deque>
#include <cassert>

namespace SDG
{
    const int DefaultInitialPoolSize = 256;

    // Identification for management by a Pool object. A Poolable object contains this as a field.
    struct PoolID
    {
        PoolID() : id_{SIZE_MAX}, inner_id_{SIZE_MAX}, next_{SIZE_MAX}, is_alive_{false}, pool_id_{} {}

        // Indicates the index location in the data struction.
        size_t id_;
        // The unique individual ID.
        size_t inner_id_;
        // Free list that points to the next free Poolable's index.
        size_t next_;
        // Flag indicating whether or not the entity has been taken out or not.
        bool is_alive_;

        // Pointer to the containing pool.
        void *pool_id_;

        // Comparison with other id's. Checks actual unique inner ID.
        bool operator==(const PoolID &other) const
        {
            return (inner_id_ == other.inner_id_) && (pool_id_ == other.pool_id_);
        }
    };

    class IPoolable; // forward declaration

    template <typename T>
    concept Poolable = std::is_base_of_v<IPoolable, T>;

    /* ========================================================================
     * class IPoolable
     * All poolable classes must derive from this class.
     * ======================================================================*/
    class IPoolable
    {
        template <Poolable T> friend class Pool;
    private:
        // Identification for management by a Pool object.
        PoolID pid;
    };

    /* ========================================================================
     * class Pool
     * Manages a pool of reusable objects. Prevent rapid construction/deletion.
     * Type T must have a default constructor and derive from IPoolable.
     * ======================================================================*/
    template <Poolable T>
    class Pool
    {
    public:
        explicit Pool(size_t init_cap = DefaultInitialPoolSize)
                : free_list_(init_cap > 0 ? 0 : SIZE_MAX), v_{}, ticket_{}
        {
            ExpandPool(init_cap);
        }

        /**
         * Check out/take out an object from the Pool.
         * @return ptr to a fresh object.
         */
        T* CheckOut()
        {
            size_t next_free;
            if (free_list_ == SIZE_MAX) // no free poolable entities, expand the pool.
            {
                next_free = v_.size();

                // Every time the pool expands, increase its size by a factor of 2.
                ExpandPool(next_free * 2 + 1);
            }
            else                        // free entities to take.
            {
                next_free = free_list_;
            }

            return GetRefreshedEntity(next_free);
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
            for(T &entity : v_)
            {
                if (entity.pid.is_alive_)
                    Return(&entity);
            }
        }

        // The size of the pool.
        [[nodiscard]]
        size_t CurrentMaxSize() const { return v_.size(); }

    private:
        // Safely adds poolable entities to the pool, expanding its size.
        void ExpandPool(size_t new_size)
        {
            assert(new_size >= v_.size());
            for (size_t i = v_.size(); i < new_size; ++i) {
                v_.emplace_back();
                T &obj = v_.back();
                obj.pid.id_ = i;
                obj.pid.inner_id_ = ticket_++;
                obj.pid.next_ = (i < new_size - 1) ? i + 1 : SIZE_MAX;
                obj.pid.pool_id_ = (void *)this;
            }
        }

        // Helper that gets the next available entity in the pool
        T *GetRefreshedEntity(size_t index)
        {
            T *ret = &v_[index];
            ret->pid.is_alive_ = true;
            free_list_ = ret->pid.next_; // update the free list

            return ret;
        }

        // List pointing to the next free poolable entity.
        size_t free_list_;

        // The current id index that the pool uses to issue each unique ID starting at 0 and increasing upon assignment
        // each time a new entity is added to the pool.
        size_t ticket_;

        // A deque was chosen since we want to allow expandable pool.
        // The current implementation returns pointers to the various real elements.
        // Since we originally used vector, whenever it resized all the ptrs would become invalidated.
        std::deque<T> v_;

    }; // class Pool
} // namespace SDG
