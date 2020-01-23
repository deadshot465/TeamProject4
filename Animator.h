#pragma once
#include "ECS.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <cassert>

class Animator :
	public Component
{
public:
	Animator(std::string_view fileName, int xCount, int yCount);

	template <size_t N>
	Animator(const std::string_view(&fileNames)[N], int (&xCount)[N], int (&yCount)[N]);

	explicit Animator(const Animator& animator) = delete;

	~Animator();

	void SetAnimation(size_t textureIndex, std::string_view animationName,
		const std::vector<size_t>& animationIndices, int frameSpeed = 8);

	void Play(size_t textureIndex, std::string_view animationName) noexcept;

	void SetAngle(float angle);

	Animator& operator=(const Animator& animator) = delete;

protected:
	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

private:

	struct TagAnimationData
	{
		std::vector<size_t> AnimationIndices;
		int FrameSpeed;
	};

	struct TextureData
	{
		Texture2D Texture;
		Rectangle FrameSize;
		std::vector<Rectangle> Animations;
		std::unordered_map<std::string, TagAnimationData> AnimationData;
	};

	struct
	{
		size_t CurrentTextureIndex;
		std::string CurrentAnimation;
		std::vector<size_t>* AnimationIndices;
		int FrameSpeed;
	} mAnimationInfo;

	std::vector<TextureData> mTextures;
	size_t mCurrentFrame = 0;
	size_t mCurrentAnimationIndex = 0;
	int mCounter = 0;
	int mAngle = 0;
};

template<size_t N>
inline Animator::Animator(const std::string_view(&fileNames)[N], int (&xCount)[N], int (&yCount)[N])
{
	static_assert(N > 0);

	mTextures.resize(N);
	for (auto i = 0; i < N; ++i)
	{
		mTextures[i].Texture = LoadTexture(fileNames[i].data());
		mTextures[i].FrameSize.x = 0;
		mTextures[i].FrameSize.y = 0;
		mTextures[i].FrameSize.width = mTextures[i].Texture.width / xCount[i];
		mTextures[i].FrameSize.height = mTextures[i].Texture.height / yCount[i];

		mTextures[i].Animations.resize(yCount[i] * xCount[i]);

		size_t index = 0;
		for (auto y = 0; y < yCount[i]; ++y)
		{
			for (auto x = 0; x < xCount[i]; ++x)
			{
				mTextures[i].Animations[index].x = mTextures[i].FrameSize.width * x;
				mTextures[i].Animations[index].y = mTextures[i].FrameSize.height * y;
				mTextures[i].Animations[index].width = mTextures[i].FrameSize.width;
				mTextures[i].Animations[index].height = mTextures[i].FrameSize.height;
				++index;
			}
		}
	}
}
