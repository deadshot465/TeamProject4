#pragma once
#include <json.hpp>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "ECS.h"

struct ResultData
{
	std::string Name;
	int Score;
};

struct Result
{
	std::vector<ResultData> Results;
};

void to_json(nlohmann::json& j, const ResultData& result);
void from_json(const nlohmann::json& j, ResultData& result);
void to_json(nlohmann::json& j, const Result& result);
void from_json(const nlohmann::json& j, Result& result);

class UIManager
{
public:
	UIManager() = delete;
	~UIManager() = delete;

	static void Initialize();
	static void Release();
	static void Reset();
	static void AddItem(Scene* scene);
	static void ShowItems(Scene* scene);
	static void RegisterRanking(Scene* scene);
	static void ShowResult(Scene* scene);
	static void ShowGameOver(Scene* scene);

	static constexpr bool IsInputting() noexcept { return mIsInputting; }

private:
	struct Item
	{
		Texture ItemTexture;
		Rectangle ItemRectangle;
	};

	static void ReadFromJson();
	static void WriteToJson();

	inline static std::unordered_map<std::string, Item> mItems;
	inline static std::string mInputString;
	inline static int mLetterCount = 0;
	inline static int mFrameCounter = 0;
	inline static bool mIsInputting = true;
	inline static Result mResults;
};

