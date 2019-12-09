#pragma once
#include <raylib.h>
#include "Helper.h"

class IUpdatable
{
protected:
	Vector2 mPosition = {};

public:
	virtual void Update(int viewportWidth, int viewportHeight) = 0;
	virtual void Render() = 0;
};

