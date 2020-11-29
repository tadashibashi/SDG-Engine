/* =============================================================================
 * CollisionMgr
 * 
 * 
 * ===========================================================================*/
#include "CollisionMgr.h"
#include <Engine/Entity.h>
#include <Engine/Components/Body.h>
#include <Engine/Log.h>
#include <Engine/Components/Collider.h>

namespace SDG
{
    CollisionMgr::CollisionMgr(Point hashSize): EntityComponent(true, false), hashSize_(hashSize)
    {

    }

    CollisionMgr::~CollisionMgr()
    {
    }

    void CollisionMgr::RegisterCollider(Collider2D *collider)
    {
        colliders_.emplace_back(collider);
    }

    void CollisionMgr::UnregisterCollider(Collider2D *collider)
    {
        for (auto it = colliders_.begin(), end = colliders_.end(); it != end; ++it)
        {
            if (*it == collider)
            {
                colliders_.erase(it);
                return;
            }
        }
    }

    void CollisionMgr::ProcessCollisions()
    {
        colls_.clear();
        // Put entities in hash map
        for (auto &coll: colliders_)
        {
            coll->collided_.clear();

            auto *body = coll->GetComponent<Body>();
            Rectangle bounds = body->GetBounds();
            int x = bounds.x / hashSize_.x;
            int y = bounds.y / hashSize_.y;

            // center
            colls_[ x ][ y ].emplace_back(coll);
            // left
            colls_[x-1][ y ].emplace_back(coll);
            // right
            colls_[x+1][ y ].emplace_back(coll);
            // up
            colls_[ x ][y-1].emplace_back(coll);
            // down
            colls_[ x ][y+1].emplace_back(coll);
            // up-left
            colls_[x-1][y-1].emplace_back(coll);
            // up-right
            colls_[x+1][y-1].emplace_back(coll);
            // down-left
            colls_[x-1][y+1].emplace_back(coll);
            // down-right
            colls_[x+1][y+1].emplace_back(coll);
        }

        auto camBounds = GetCurrentScene()->GetCamera()->GetWorldBounds();

        // Check collisions
        for (auto &[x, columns]: colls_)             // for every column of collider lists
        {
            // cull all collisions outside of camera view plus a half.
            if ((float)x * hashSize_.x < camBounds.GetLeft() - camBounds.w * .5f || (float)x * hashSize_.x > camBounds.GetRight() + camBounds.w * .5f)
                continue;

            for (auto &[y, colliderList]: columns)   // for every list of colliders
            {
                // cull all collisions outside of camera view plus a half.
                if ((float)y * hashSize_.y < camBounds.GetTop() - camBounds.h * .5f || (float)y * hashSize_.y > camBounds.GetBottom() + camBounds.h * .5f)
                    continue;

                for (auto &thisColl: colliderList)     // for every collider in its list
                {
                    for (auto &otherColl: colliderList)
                    {
                        // Ensure entity is not checking collision with itself
                        if (thisColl != otherColl)
                        {

                            if (thisColl->CheckCollision(otherColl) && !thisColl->collided_.contains(otherColl))
                            {
                                if (thisColl->callback_)
                                    thisColl->callback_(thisColl->GetEntity(), otherColl->GetEntity());
                                thisColl->collided_.emplace(otherColl);
                            }
                        }
                    }
                }
            }
        }
    }

}
