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
		// Component ����Čp������܂���
		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
	};

	class Rectangle
		: public Component
	{
	public:
		Rectangle();
		Rectangle(std::string_view name, int width, int height, const Color& color = RED);
		~Rectangle();

		int Width = 50;
		int Height = 50;
		Color FillColor = RED;

	protected:
		// Component ����Čp������܂���
		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
	};
}