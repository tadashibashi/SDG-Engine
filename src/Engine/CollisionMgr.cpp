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
        toAdd_.emplace_back(collider);
    }

    void CollisionMgr::UnregisterCollider(Collider2D *collider)
    {
        collider->toRemove_ = true;
        toRemove_ = true;
    }

    void CollisionMgr::ProcessCollisions()
    {
        ProcessRemovals();
        ProcessAdditions();

        colls_.clear();

        Point hashsize = (colliders_.size() > 256) ? hashSize_/2 : hashSize_;
        // Put entities in hash map
        for (auto &coll: colliders_)
        {
            coll->collided_.clear();

            auto *body = coll->GetComponent<Body>();
            Rectangle bounds = body->GetBounds();

            if (hashsize.x <= 0) hashsize.x = 1;
            if (hashsize.y <= 0) hashsize.y = 1;

            int x = bounds.x / hashsize.x;
            int y = bounds.y / hashsize.y;
            int w = (bounds.x % hashsize.x + bounds.w) / hashsize.x;
            int h = (bounds.y % hashsize.y + bounds.h) / hashsize.y;

            for (int i = 0; i <= w; ++i)
            {
                for (int k = 0; k <= h; ++k)
                {
                    colls_[x + i][y + k].emplace_back(coll);
                }
            }
        }

        auto camBounds = GetCurrentScene()->GetCamera()->GetWorldBounds();

        // Check collisions
        for (auto &[x, columns]: colls_)             // for every column of collider lists
        {
            // cull all collisions outside of camera view plus a half.
            if ((float)x * hashsize.x < camBounds.GetLeft() - camBounds.w * .5f || (float)x * hashsize.x > camBounds.GetRight() + camBounds.w * .5f)
                continue;

            for (auto &[y, colliderList]: columns)   // for every list of colliders
            {
                // cull all collisions outside of camera view plus a half.
                if ((float)y * hashsize.y < camBounds.GetTop() - camBounds.h * .5f || (float)y * hashsize.y > camBounds.GetBottom() + camBounds.h * .5f)
                    continue;

                for (Collider2D *thisColl: colliderList)     // for every collider in its list
                {
                    for (Collider2D *otherColl: colliderList)
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

    void CollisionMgr::ProcessRemovals()
    {
        if (toRemove_)
        {
            colliders_.erase(std::remove_if(colliders_.begin(), colliders_.end(),
                                            [](Collider2D *coll) { return coll->toRemove_; }), colliders_.end());
            toRemove_ = false;
        }
    }

    void CollisionMgr::ProcessAdditions()
    {
        if (!toAdd_.empty())
        {
            for (Collider2D *coll : toAdd_)
            {
                colliders_.emplace_back(coll);
            }

            toAdd_.clear();
        }
    }

}
