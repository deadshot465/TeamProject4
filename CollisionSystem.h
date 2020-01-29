#pragma once
#include <type_traits>
#include <vector>
#include "Colliders.h"
#include "Controller.h"
#include "MapChipManager.h"

class CollisionSystem
{
public:
	CollisionSystem() = delete;
	~CollisionSystem() = default;

	template <typename ShieldType = CircleCollider, typename EnemyType = CircleCollider>
	static bool CheckShieldCollision(const ShieldType* shield, const std::vector<EnemyType*>& colliders, EnemyType* collidedObject = nullptr);

	template <typename T = CircleCollider>
	static bool CheckWallCollision(const T* colliderType, std::vector<RectangleCollider>& colliders);

	template <typename T = CircleCollider>
	static bool StartActivated(const T* collider);

	template <typename T = CircleCollider>
	static bool QuitActivated(const T* collider);
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
		if (collider->mEntity->Parent) continue;
		bool res = shield->CheckCollision(*collider);
		if (res)
		{
			if (collidedObject) *collidedObject = *collider;
			collider->mEntity->GetComponent<EnemyController>()->CurrentState = EnemyController::EnemyState::Attached;
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
			return res;
		}
	}

	return false;
}

template<typename T>
inline bool CollisionSystem::StartActivated(const T* collider)
{
	if constexpr (std::is_same_v<T, CircleCollider>)
		return collider->CheckRectangleCollision(MapChipManager::GetStartButton());
}

template<typename T>
inline bool CollisionSystem::QuitActivated(const T* collider)
{
	if constexpr (std::is_same_v<T, CircleCollider>)
		return collider->CheckRectangleCollision(MapChipManager::GetQuitButton());
}
