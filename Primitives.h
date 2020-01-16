#pragma once
#include "ECS.h"

namespace Primitives
{
	class Circle
		: public Component
	{
	public:
		Circle();
		Circle(std::string_view name, float radius = 50.0f, const Color& color = RED);
		~Circle();

		float Radius = 50.0f;
		Color FillColor = RED;

	protected:
		// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
	};
}