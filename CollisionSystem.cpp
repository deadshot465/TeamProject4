#include "CollisionSystem.h"

bool CollisionSystem::CheckCollision(const Collider& principal, const std::vector<Collider*>& colliders, Collider* collidedObject)
{
	for (auto& collider : colliders)
	{
		bool res = principal.CheckCollision(*collider);
		if (res)
		{
			if (collidedObject) *collidedObject = *collider;
			collider->Enabled = false;
			return res;
		}
	}
	return false;
}
