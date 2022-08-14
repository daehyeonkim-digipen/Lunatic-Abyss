#include "Collision.h"
#include "Texture.h"
#include "GameObject.h"
#include "TextureManager.h"


RectCollision::RectCollision(math::rect2 rect, GameObject* owner) : rect(rect), owner(owner) {}

void RectCollision::draw(math::TransformMatrix camera)
{
    Texture* a = Engine::get_GE_component<TextureManager>()->load("assets/UI/collisionBox.png");
    math::TransformMatrix tm = math::ScaleMatrix(math::vec2(GetWorldCoorRect().Size() / 16.));
    a->draw(camera * math::TranslateMatrix({GetWorldCoorRect().Left(), GetWorldCoorRect().Bottom()})* tm);
}

math::rect2 RectCollision::GetWorldCoorRect()
{
    math::rect2 collisionRect = math::rect2(rect);
    const math::TransformMatrix objectMatrix = owner->get_matrix();
    collisionRect.point1 = objectMatrix * collisionRect.point1;
    collisionRect.point2 = objectMatrix * collisionRect.point2;
    return collisionRect;
}

bool RectCollision::does_collide(GameObject* obj)
{
    Collision* collideAgainst = obj->get_GO_components<Collision>();
    math::rect2 rectA = GetWorldCoorRect();
    math::rect2 rectB = dynamic_cast<RectCollision*>(collideAgainst)->GetWorldCoorRect();
    if (rectA.Right() > rectB.Left() && rectA.Left() < rectB.Right() &&
        rectA.Bottom() < rectB.Top() && rectA.Top() > rectB.Bottom()) {
        return true;
    }
    return false;
}


bool RectCollision::does_collide(math::vec2 point)
{
    math::rect2 worldRect = GetWorldCoorRect();
    return point.x > worldRect.Left() && point.x < worldRect.Right() && point.y < worldRect.Top() && point.y > worldRect.Bottom();
}

void RectCollision::will_collide(GameObject* obj, bool collisions[])
{
    const math::vec2 objVelocityr = obj->get_velocity();
    const auto objWCR = obj->get_GO_components<RectCollision>()->GetWorldCoorRect();
    const double yGap = objWCR.Size().y / 3.;
    const math::vec2 objLF = { objWCR.Left(), objWCR.Bottom() };
    const math::vec2 objRF = { objWCR.Right(), objWCR.Bottom() };
    const math::vec2 objLH = { objWCR.Left(), objWCR.Top() };
    const math::vec2 objRH = { objWCR.Right(), objWCR.Top() };
    const math::vec2 objLMU = { objWCR.Left(), objWCR.Bottom() + yGap * 2. };
    const math::vec2 objRMU = { objWCR.Right(), objWCR.Bottom() + yGap * 2. };
    const math::vec2 objLMD = { objWCR.Left(), objWCR.Bottom() + yGap};
    const math::vec2 objRMD = { objWCR.Right(), objWCR.Bottom() + yGap};
    collisions[4] = does_collide(objLF + objVelocityr);
    collisions[5] = does_collide(objRF + objVelocityr);
    collisions[0] = does_collide(objLH + objVelocityr);
    collisions[1] = does_collide(objRH + objVelocityr);
    collisions[2] = does_collide(objLMU + objVelocityr) || does_collide(objLMD + objVelocityr);
    collisions[3] = does_collide(objRMU + objVelocityr) || does_collide(objRMD + objVelocityr);
    collisions[6] = collisions[0] || collisions[1] | collisions[2] || collisions[3] || collisions[4] || collisions[5];
}
