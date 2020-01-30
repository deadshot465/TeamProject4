#include "UIManager.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "GameScene.h"

void UIManager::Initialize()
{
	mItems["Shield"].ItemTexture = LoadTexture("./sprite/UI.png");
	mItems["Shield"].ItemRectangle = { 310, 480, 128, 128 };

	mItems["Hp"].ItemTexture = LoadTexture("./sprite/UI.png");
	mItems["Hp"].ItemRectangle = { 550, 480, 128, 128 };

	mItems["Time"].ItemTexture = LoadTexture("./sprite/UI.png");
	mItems["Time"].ItemRectangle = { 790, 480, 128, 128 };
}

void UIManager::Release()
{
	for (auto& [name, item] : mItems)
	{
		UnloadTexture(item.ItemTexture);
	}
}

void UIManager::Reset()
{
	mInputString.clear();
	mLetterCount = 0;
	mFrameCounter = 0;
	mIsInputting = true;
}

void UIManager::AddItem(Scene* scene)
{
}

void UIManager::ShowItems(Scene* scene)
{
	Color color = { 0, 0, 0, 128 };
	DrawRectangle(0, 0, 1280, 960, color);
	DrawText("CHOOSE A POWER-UP", 260, 400, 72, WHITE);

	DrawTexturePro(mItems["Shield"].ItemTexture, { 192, 0, 64, 64 }, mItems["Shield"].ItemRectangle, { 0, 0 }, 0.0f, WHITE);
	DrawText("1", mItems["Shield"].ItemRectangle.x + 8, mItems["Shield"].ItemRectangle.y, 36, RAYWHITE);

	DrawTexturePro(mItems["Hp"].ItemTexture, { 0, 0, 64, 64 }, mItems["Hp"].ItemRectangle, { 0, 0 }, 0.0f, WHITE);
	DrawText("2", mItems["Hp"].ItemRectangle.x - 16, mItems["Hp"].ItemRectangle.y, 36, RAYWHITE);

	DrawText("TIME", mItems["Time"].ItemRectangle.x, mItems["Time"].ItemRectangle.y + 32, 56, GOLD);
	DrawText("3", mItems["Time"].ItemRectangle.x - 16, mItems["Time"].ItemRectangle.y, 36, RAYWHITE);
}

void UIManager::RegisterRanking(Scene* scene)
{
	static constexpr int MAX_COUNT = 20;
	static constexpr Color color = { 0, 0, 0, 128 };
	static constexpr Rectangle text_box = { 430, 480, 400, 64 };

	if (!mIsInputting) return;

	bool mouse_on_text = false;
	
	if (CheckCollisionPointRec(GetMousePosition(), text_box)) mouse_on_text = true;
	else mouse_on_text = false;

	if (mouse_on_text)
	{
		auto key = GetKeyPressed();

		while (key > 0)
		{
			if ((key >= 32) && (key <= 125) && (mLetterCount < MAX_COUNT))
			{
				mInputString += static_cast<char>(key);
				++mLetterCount;
				break;
			}
		}

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			--mLetterCount;
			mInputString = mInputString.substr(0, mInputString.size() - 1);

			if (mLetterCount < 0) mLetterCount = 0;
		}
	}

	if (mouse_on_text) ++mFrameCounter;
	else mFrameCounter = 0;

	DrawRectangle(0, 0, 1280, 960, color);
	DrawText("ENTER YOUR NAME", 260, 400, 72, WHITE);
	DrawRectangleRec(text_box, LIGHTGRAY);
	DrawText(mInputString.c_str(), text_box.x + 5, text_box.y + 8, 40, MAROON);
	DrawText(FormatText("TEXT COUNT: %i/%i", mLetterCount, MAX_COUNT), 480, 560, 30, DARKGRAY);

	if (mouse_on_text)
	{
		if (mLetterCount < MAX_COUNT)
		{
			if ((mFrameCounter / 20) % 2 == 0)
				DrawText("_", text_box.x + 8 + MeasureText(mInputString.c_str(), 40), text_box.y + 12, 40, MAROON);
		}
	}

	if (mInputString.size() > 0 && IsKeyPressed(KEY_ENTER))
	{
		mIsInputting = false;
		ResultData data;
		data.Name = mInputString;
		data.Score = dynamic_cast<GameScene*>(scene)->GetScore();
		mResults.Results.emplace_back(data);
		//WriteToJson();

		std::stable_sort(mResults.Results.begin(), mResults.Results.end(),
			[&](const ResultData& a, const ResultData& b) {
				return a.Score < b.Score;
			});

		std::reverse(mResults.Results.begin(), mResults.Results.end());
	}
}

void UIManager::ShowResult(Scene* scene)
{
	static constexpr Color color = { 0, 0, 0, 128 };
	static constexpr int y_offset = 80;

	DrawRectangle(0, 0, 1280, 960, color);
	DrawText("RANKINGS", 460, 200, 80, WHITE);

	int y = 280;

	for (auto i = 0; i < mResults.Results.size(); ++i)
	{
		if (i > 5) break;

		DrawText(FormatText("%d.\t%s\t\t\t\t\t%d", i + 1, mResults.Results[i].Name.c_str(), mResults.Results[i].Score), 260, y, 64, WHITE);

		y += y_offset;
	}
}

void UIManager::ShowGameOver(Scene* scene)
{
	static constexpr Color color = { 0, 0, 0, 128 };
	DrawRectangle(0, 0, 1280, 960, color);
	DrawText("GAME OVER", 430, 440, 80, RED);
	DrawText("Press Enter to get back to the title.", 260, 520, 40, RAYWHITE);
}

void UIManager::ReadFromJson()
{
	nlohmann::json j;
	auto fs = std::ifstream();
	fs.open("rankings.json", std::ios_base::in);
	if (fs.good()) {
		while (!fs.eof())
		{
			fs >> j;
			auto result = j.get<ResultData>();
			mResults.Results.emplace_back(result);
		}
	}
	fs.close();
}

void UIManager::WriteToJson()
{
	auto fs = std::ofstream();
	fs.open("rankings.json", std::ios_base::out);

	for (const auto& result : mResults.Results)
	{
		nlohmann::json j = result;
		fs << std::setw(4) << j << std::endl;
	}

	fs.close();
}

void to_json(nlohmann::json& j, const ResultData& result)
{
	j = nlohmann::json{
		{ "Name", result.Name },
		{ "Score", result.Score }
	};
}

void from_json(const nlohmann::json& j, ResultData& result)
{
	j.at("Name").get_to(result.Name);
	j.at("Score").get_to(result.Score);
}
