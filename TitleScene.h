#pragma once
#include "SceneBase.h"
#include <string_view>

class TitleScene :
	public SceneBase
{
public:
	TitleScene();
	explicit TitleScene(std::string_view name);
	~TitleScene();

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};

