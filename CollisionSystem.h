#pragma once
#include <vector>
#include "Colliders.h"

class CollisionSystem
{
public:
	CollisionSystem() = delete;
	~CollisionSystem() = default;

	static bool CheckCollision(const Collider& principal, const std::vector<Collider>& colliders,
		const Collider* collidedObject = nullptr);
};

