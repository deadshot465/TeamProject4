//
// Created by 周哲輝 on 2020/01/16.
//

#include "CircleCollider.h"
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

CircleCollider::CircleCollider() : Component()
{

}

CircleCollider::CircleCollider(std::string_view name, float radius)
    : Component(name), Radius(radius)
{

}

CircleCollider::~CircleCollider()
{

}

bool CircleCollider::CheckCollision(const glm::vec2 &otherCenter, float otherRadius)
{
    assert(mEntity);

    Vector2 center = { mEntity->Position.x, mEntity->Position.y };
    Vector2 other_center = { otherCenter.x, otherCenter.y };

    return CheckCollisionCircles(center, this->Radius, other_center, otherRadius);
}
