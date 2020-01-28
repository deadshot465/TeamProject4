#pragma once
#include <vector>
#include "Colliders.h"
#include "Controller.h"

class CollisionSystem
{
public:
	CollisionSystem() = delete;
	~CollisionSystem() = default;

	template <typename ShieldType = CircleCollider, typename EnemyType = CircleCollider>
	static bool CheckShieldCollision(const ShieldType* shield, const std::vector<EnemyType*>& colliders, EnemyType* collidedObject = nullptr);

	template <typename T = CircleCollider>
	static bool CheckWallCollision(const T* colliderType, std::vector<RectangleCollider>& colliders);
};

template<typename ShieldType, typename EnemyType>
inline bool CollisionSystem::CheckShieldCollision(const ShieldType* shield, const std::vector<EnemyType*>& colliders, EnemyType* collidedObject)
{
	auto shield_parent = shield->mEntity->Parent;
	auto player_controller = shield_parent ? shield_parent->GetComponent<PlayerController>() : nullptr;
	if (player_controller && !player_controller->IsAttacking())
	{
		return false;
	}

	for (auto& collider : colliders)
	{
		bool res = shield->CheckCollision(*collider);
		if (res)
		{
			if (collidedObject) *collidedObject = *collider;
			collider->Enabled = false;
			return res;
		}
	}
	return false;
}

template<typename T>
inline bool CollisionSystem::CheckWallCollision(const T* colliderType, std::vector<RectangleCollider>& colliders)
{
	for (auto& collider : colliders)
	{
		auto res = colliderType->CheckRectangleCollision(collider);
		if (res)
		{
			collider.Enabled = false;
			return res;
		}
	}

	return false;
}
