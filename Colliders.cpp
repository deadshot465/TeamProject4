//
// Created by 周哲輝 on 2020/01/16.
//

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

    Vector2 center = { mEntity->Position.x, mEntity->Position.y };

    const CircleCollider& _other = dynamic_cast<const CircleCollider&>(other);

    Vector2 other_center = { _other.mEntity->Position.x, _other.mEntity->Position.y };

    return CheckCollisionCircles(center, this->Radius, other_center, _other.Radius);
}

Collider::Collider()
    : Component()
{
}

Collider::Collider(std::string_view name)
    : Component(name)
{
}
