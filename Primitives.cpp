#include "Primitives.h"
#include <raylib.h>
#include <cassert>

Primitives::Circle::Circle()
	: Component()
{
}

Primitives::Circle::Circle(std::string_view name, float radius, const Color& color)
	: Component(name), Radius(radius), FillColor(color)
{
}

Primitives::Circle::~Circle()
{
}

void Primitives::Circle::Initialize()
{
}

void Primitives::Circle::Update(float deltaTime)
{
}

void Primitives::Circle::Render()
{
	assert(mEntity);

	Vector2 center = { mEntity->Position.x, mEntity->Position.y };
	DrawCircleV(center, Radius, FillColor);
}

Primitives::Rectangle::Rectangle()
	: Component()
{
}

Primitives::Rectangle::Rectangle(std::string_view name, int width, int height, const Color& color)
	: Component(name), Width(width), Height(height), FillColor(color)
{

}

Primitives::Rectangle::~Rectangle()
{
}

void Primitives::Rectangle::Initialize()
{
}

void Primitives::Rectangle::Update(float deltaTime)
{
}

void Primitives::Rectangle::Render()
{
	assert(mEntity);

	DrawRectangle(mEntity->Position.x, mEntity->Position.y, Width, Height, FillColor);
}
