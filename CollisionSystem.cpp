#include "CollisionSystem.h"

bool CollisionSystem::CheckCollision(const Collider& principal, const std::vector<Collider>& colliders,
	const Collider* collidedObject)
{
	for (const auto& collider : colliders)
	{
		bool res = principal.CheckCollision(collider);
		if (res)
		{
			if (collidedObject) collidedObject = &collider;
			return res;
		}
	}
	return false;
}
