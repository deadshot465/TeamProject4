#include "Projectile.h"

Projectile::Projectile(float x, float y, float destX, float destY, float speed)
    : mPosition(x, y), mDestination(destX, destY)
{
    mSpeed = mDestination - mPosition;
    mSpeed = glm::normalize(mSpeed) * speed;
}

Projectile::~Projectile()
{
}

bool Projectile::CheckBound(int viewportWidth, int viewportHeight) const noexcept
{
    return mPosition.x > viewportWidth || mPosition.x < 0 ||
        mPosition.y > viewportHeight || mPosition.y < 0;
}

void Projectile::Update(int viewportWidth, int viewportHeight)
{
    mPosition += mSpeed;
}

void Projectile::Render()
{
    //DrawCircleV(ToRayLibVector2(mPosition), 10.0f, DARKGREEN);
    DrawText("FUCK OFF!!!", static_cast<int>(mPosition.x), 
        static_cast<int>(mPosition.y), 20, LIGHTGRAY);
}
