#pragma once
#include <glm/glm.hpp>
#include <raylib.h>

struct RelativePosition {
	glm::vec2 Parent;
	glm::vec2 Offset;
};

union Position {
	glm::vec2 Absolute = glm::vec2(0.0f);
	RelativePosition Relative;

	Position() = default;

	Position(const glm::vec2& parent, const glm::vec2& offset)
		: Relative({ parent, offset })
	{

	}

	explicit Position(const glm::vec2& absolute)
		: Absolute(absolute)
	{

	}

	explicit Position(const RelativePosition& relative)
		: Relative(relative)
	{

	}

	Position& operator=(const Position& position)
	{
		if (&position == this) return *this;

		Absolute = position.Absolute;
		Relative = position.Relative;

		return *this;
	}

	Position& operator=(const glm::vec2& absolute)
	{
		Absolute = absolute;
		return *this;
	}

	Position& operator=(const RelativePosition& relative)
	{
		Relative = relative;
		return *this;
	}

	operator glm::vec2()
	{
		return Absolute;
	}

	operator RelativePosition()
	{
		return Relative;
	}
};

inline Vector2 ToRayLibVector2(const glm::vec2& vector) {
	auto vec = Vector2();
	vec.x = vector.x;
	vec.y = vector.y;
	return vec;
}

inline glm::vec2 ToGlmVector2(const Vector2& vector) {
	return glm::vec2(vector.x, vector.y);
}