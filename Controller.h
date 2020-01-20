#pragma once
#include "ECS.h"

class Controller :
	public Component
{
public:
	Controller(float speed);
	~Controller();

	// Component ����Čp������܂���
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	float Speed = 0.0f;
};

class EnemyController
	: public Component
{
public:
	EnemyController(float speed);
	~EnemyController();

	// Component ����Čp������܂���
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	float Speed = 0.0f;
	glm::vec2 Velocity = {};
private:
	glm::vec2 mPlayerPosition = {};
};

