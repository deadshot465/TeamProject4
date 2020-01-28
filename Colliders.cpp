#include "Colliders.h"
#include <cassert>

void CircleCollider::Initialize()
{

}

void CircleCollider::Update(float deltaTime)
{

}

void CircleCollider::Render()
{

}

CircleCollider::CircleCollider() : Collider()
{

}

CircleCollider::CircleCollider(std::string_view name, float radius)
    : Collider(name), Radius(radius)
{

}

CircleCollider::~CircleCollider()
{

}

bool CircleCollider::CheckCollision(const Collider& other) const
{
    assert(mEntity);

    if (!other.Enabled) return false;

    Vector2 center = { mEntity->Position.x, mEntity->Position.y };

    const CircleCollider& _other = dynamic_cast<const CircleCollider&>(other);

    Vector2 other_center = { _other.mEntity->Position.x, _other.mEntity->Position.y };

    return CheckCollisionCircles(center, this->Radius, other_center, _other.Radius);
}

bool CircleCollider::CheckRectangleCollision(const Collider& other) const noexcept
{
    assert(mEntity);
    if (!other.Enabled) return false;

    Vector2 center = { mEntity->Position.x, mEntity->Position.y };
    const RectangleCollider& _other = dynamic_cast<const RectangleCollider&>(other);

    return CheckCollisionCircleRec(center, this->Radius, _other.RectangleBox);
}

Collider::Collider()
    : Component()
{
}

Collider::Collider(std::string_view name)
    : Component(name)
{
}

RectangleCollider::RectangleCollider()
    : Collider()
{
}

RectangleCollider::RectangleCollider(std::string_view name, const Rectangle& rectangle)
    : Collider(name), RectangleBox(rectangle)
{
    
}

RectangleCollider::~RectangleCollider()
{
}

bool RectangleCollider::CheckCollision(const Collider& other) const
{
    return false;
}

void RectangleCollider::Initialize()
{
}

void RectangleCollider::Update(float deltaTime)
{
}

void RectangleCollider::Render()
{
}
