#include "Animator.h"
#include <algorithm>
#include <stdexcept>
#include "Helper.h"

Animator::Animator(std::string_view fileName, int xCount, int yCount)
{
	assert(xCount > 0);
	assert(yCount > 0);

	mTextures.emplace_back(TextureData());
	mTextures[0].Texture = LoadTexture(fileName.data());

	mTextures[0].FrameSize.x = 0;
	mTextures[0].FrameSize.y = 0;
	mTextures[0].FrameSize.width = mTextures[0].Texture.width / xCount;
	mTextures[0].FrameSize.height = mTextures[0].Texture.height / yCount;

	mTextures[0].Animations.resize(static_cast<size_t>(yCount * xCount));

	size_t index = 0;
	for (auto y = 0; y < yCount; ++y)
	{
		for (auto x = 0; x < xCount; ++x)
		{
			mTextures[0].Animations[index].x = mTextures[0].FrameSize.width * x;
			mTextures[0].Animations[index].y = mTextures[0].FrameSize.height * y;
			mTextures[0].Animations[index].width = mTextures[0].FrameSize.width;
			mTextures[0].Animations[index].height = mTextures[0].FrameSize.height;
			++index;
		}
	}
}

Animator::~Animator()
{
	for (auto& texture : mTextures)
		UnloadTexture(texture.Texture);
}

void Animator::SetAnimation(size_t textureIndex, std::string_view animationName, const std::vector<size_t>& animationIndices, int frameSpeed)
{
	assert(textureIndex < mTextures.size());

	auto& texture = mTextures[textureIndex];
	texture.AnimationData[animationName.data()].AnimationIndices.resize(animationIndices.size());
	std::copy(animationIndices.cbegin(), animationIndices.cend(),
		texture.AnimationData[animationName.data()].AnimationIndices.begin());
	texture.AnimationData[animationName.data()].FrameSpeed = frameSpeed;
}

void Animator::Play(size_t textureIndex, std::string_view animationName) noexcept
{
	auto iter = mTextures[textureIndex].AnimationData.find(animationName.data());
	if (iter != mTextures[textureIndex].AnimationData.end())
	{
		mAnimationInfo.CurrentTextureIndex = textureIndex;
		mAnimationInfo.CurrentAnimation = animationName;
		mAnimationInfo.AnimationIndices = &(iter->second.AnimationIndices);
		mAnimationInfo.FrameSpeed = iter->second.FrameSpeed;
		
		mCounter = 0;
		mCurrentFrame = 0;
		mCurrentAnimationIndex = (*mAnimationInfo.AnimationIndices)[0];
	}
	else
		throw std::runtime_error("No such animation.\n");
}

void Animator::SetAngle(float angle)
{
	mAngle = angle;
}

void Animator::Initialize()
{
}

void Animator::Update(float deltaTime)
{
	static constexpr int FPS = 60;
	auto size = mAnimationInfo.AnimationIndices->size();

	++mCounter;

	if (mCounter > (FPS / mAnimationInfo.FrameSpeed))
	{
		mCounter = 0;
		++mCurrentFrame;
		mCurrentFrame = mCurrentFrame % size;
	}

	mCurrentAnimationIndex = (*mAnimationInfo.AnimationIndices)[mCurrentFrame];
}

void Animator::Render()
{
	assert(mEntity);

	auto pos = mTextures[mAnimationInfo.CurrentTextureIndex].Animations[mCurrentAnimationIndex];
	auto& texture = mTextures[mAnimationInfo.CurrentTextureIndex].Texture;
	
	DrawTextureRec(texture, pos, ToRaylibVector2(mEntity->Position), WHITE);

	/*if (mAngle != 0)
	{
		float frame_width = mTextures[mAnimationInfo.CurrentTextureIndex].FrameSize.width;
		float frame_height = mTextures[mAnimationInfo.CurrentTextureIndex].FrameSize.height;
		Rectangle dest_rec = {};
		dest_rec.x = mEntity->Position.x - (frame_width / 2.0f);
		dest_rec.y = mEntity->Position.y - (frame_height / 2.0f);
		dest_rec.width = frame_width;
		dest_rec.height = frame_height;

		Vector2 origin = {};
		origin.x = frame_width / 2.0f;
		origin.y = frame_height / 2.0f;

		DrawTexturePro(texture, pos, dest_rec,
			origin, 0, WHITE);
	}
	else
	{
		DrawTextureRec(texture, pos, ToRaylibVector2(mEntity->Position), WHITE);
	}*/
}
