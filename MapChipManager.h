#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <raylib.h>
#include <vector>

using MapChipMap = std::vector<std::vector<int>>;

struct TagMapData
{
	Texture Map;
	std::vector<Rectangle> Chips;

	TagMapData();
	explicit TagMapData(const TagMapData& tagMapData) = delete;
	TagMapData(TagMapData&& tagMapData) noexcept;
	TagMapData& operator=(const TagMapData& tagMapData) = delete;
	TagMapData& operator=(TagMapData&& tagMapData) noexcept;
};

struct MapInfo
{
	TagMapData* MapData;
	MapChipMap ChipMap;
};

class MapChipManager
{
public:
	MapChipManager() = delete;
	~MapChipManager() = delete;

	static void Initialize();
	static void Release();

	static void LoadMapChip(std::string_view csvFileName, std::string_view textureFileName, std::string_view mapName, size_t xCount, size_t yCount);

	static void DrawMapChips(std::string_view mapName);

private:
	static MapChipMap ReadMap(std::string_view name);

	inline static std::vector<TagMapData> mMapData;
	inline static std::unordered_map<std::string, MapInfo> mMaps;
};

