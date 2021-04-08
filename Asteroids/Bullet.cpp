/* =============================================================================
 * Bullet
 * 
 * 
 * ===========================================================================*/
#include "Bullet.h"

void SDG::Bullet::MakeBullet(SDG::Entity &entity) {
    entity.Components()->Add<Transform>(32.f, 32.f, 1.f, 1.f);
    auto &body = entity.Components()->Add<Body>();
    body.SetUseSpriteMask(false);
    body.size = Vector2(1.f, 1.f);
    entity.Components()->Add<Collider2D>();
    entity.Components()->Add<Bullet>();
    entity.SetTag("Bullet");
}
