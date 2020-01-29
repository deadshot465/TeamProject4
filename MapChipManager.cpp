#include "MapChipManager.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include "ConfigurationManager.h"
#include "Helper.h"

void MapChipManager::Initialize()
{
	mMapData.reserve(200);
	mStartButton = RectangleCollider(GetRandomString(10), { 512, 128, 320, 64 });
	mQuitButton = RectangleCollider(GetRandomString(10), { 512, 768, 256, 64 });
}

void MapChipManager::Release()
{
	for (auto& mapData : mMapData)
		UnloadTexture(mapData.Map);
}

void MapChipManager::LoadMapChip(std::string_view csvFileName, std::string_view textureFileName, std::string_view mapName, size_t xCount, size_t yCount)
{
	assert(!csvFileName.empty());
	assert(!textureFileName.empty());
	assert(!mapName.empty());
	assert(xCount > 0);
	assert(yCount > 0);

	TagMapData map_data = {};
	map_data.Map = LoadTexture(textureFileName.data());
	size_t chip_width = map_data.Map.width / xCount;
	size_t chip_height = map_data.Map.height / yCount;
	map_data.Chips.resize(yCount * xCount);

	size_t index = 0;
	for (size_t y = 0; y < yCount; ++y)
	{
		for (size_t x = 0; x < xCount; ++x)
		{
			map_data.Chips[index].x = static_cast<float>(x * chip_width);
			map_data.Chips[index].y = static_cast<float>(y * chip_height);
			map_data.Chips[index].width = static_cast<float>(chip_width);
			map_data.Chips[index].height = static_cast<float>(chip_height);

			++index;
		}
	}

	auto& map_data_ref = mMapData.emplace_back(std::move(map_data));

	auto chip_map = ReadMap(csvFileName);
	size_t map_x_count = chip_map[0].size();
	size_t map_y_count = chip_map.size();

	mMaps[mapName.data()].MapData = &map_data_ref;
	mMaps[mapName.data()].ChipMap.resize(map_y_count);
	for (auto& row : mMaps[mapName.data()].ChipMap)
		row.resize(map_x_count);

	std::move(chip_map.begin(), chip_map.end(), mMaps[mapName.data()].ChipMap.begin());
}

void MapChipManager::DrawMapChips(std::string_view mapName)
{
	auto& ptr = mMaps[mapName.data()].MapData;
	static const auto screen_width = ConfigurationManager::GetWidth();
	static const auto screen_height = ConfigurationManager::GetHeight();

	bool is_colliders_empty = mMaps[mapName.data()].Colliders.empty();

	float screen_x = 0;
	float screen_y = 0;
	Vector2 position = {};
	for (const auto& y : mMaps[mapName.data()].ChipMap)
	{
		for (const auto& x : y)
		{
			if (x < 0)
			{
				screen_x += ptr->Chips[0].width;
				continue;
			}
			position = { screen_x, screen_y };
			DrawTextureRec(ptr->Map, ptr->Chips[x], position, WHITE);
			if (is_colliders_empty && IsCollidableChip(x))
			{
				Rectangle rectangle = {};
				rectangle.x = position.x;
				rectangle.y = position.y;
				rectangle.width = ptr->Chips[x].width;
				rectangle.height = ptr->Chips[x].height;
				mMaps[mapName.data()].Colliders.emplace_back(GetRandomString(10), rectangle);
			}
			screen_x += ptr->Chips[x].width;
		}
		screen_x = 0;
		screen_y += ptr->Chips[0].height;
	}
}

std::vector<RectangleCollider>& MapChipManager::GetAllColliders(std::string_view mapName)
{
	return mMaps[mapName.data()].Colliders;
}

MapChipMap MapChipManager::ReadMap(std::string_view name)
{
	std::string output{};
	size_t x = 0;
	size_t y = 0;

	auto fs = std::ifstream(name.data());
	if (fs.good())
	{
		while (std::getline(fs, output))
		{
			if (y == 0)
			{
				x = std::count(output.cbegin(), output.cend(), ',') + 1;
			}
			++y;
		}
	}

	fs.close();

	auto _map = MapChipMap(y);
	for (auto& row : _map)
		row.resize(x);
	size_t width = x;
	size_t height = y;
	x = 0;
	y = 0;

	fs.open(name.data(), std::ios_base::in);

	if (fs.good())
	{
		while (std::getline(fs, output))
		{
			auto iss = std::istringstream(output);
			x = 0;
			while (std::getline(iss, output, ','))
			{
				assert(x < width);
				assert(y < height);
				_map[y][x] = std::stoi(output);
				++x;
			}
			++y;
		}
	}

	fs.close();
	return _map;
}

TagMapData::TagMapData()
	: Map({})
{
}

TagMapData::TagMapData(TagMapData&& tagMapData) noexcept
{
	this->Chips.resize(tagMapData.Chips.size());
	memset(&this->Map, 0, sizeof(Texture2D));
	memset(this->Chips.data(), 0, sizeof(Rectangle) * this->Chips.size());
	
	memcpy(&this->Map, &tagMapData.Map, sizeof(Texture2D));
	std::move(tagMapData.Chips.begin(), tagMapData.Chips.end(), this->Chips.begin());
}

TagMapData& TagMapData::operator=(TagMapData&& tagMapData) noexcept
{
	if (this == &tagMapData) return *this;

	this->Chips.resize(tagMapData.Chips.size());
	memset(&this->Map, 0, sizeof(Texture2D));
	memset(this->Chips.data(), 0, sizeof(Rectangle) * this->Chips.size());

	memcpy(&this->Map, &tagMapData.Map, sizeof(Texture2D));
	std::move(tagMapData.Chips.begin(), tagMapData.Chips.end(), this->Chips.begin());

	return *this;
}
