#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <raylib.h>

class SFX
{
public:
	SFX() = delete;
	~SFX() = delete;

	static void Initialize();
	static void Release();
	static void LoadSfx(std::string_view fileName, std::string_view sfxName);
	static void LoadMusicFile(std::string_view fileName, std::string_view musicName);
	static void PlaySfx(std::string_view sfxName);
	static void PlayMusic(std::string_view musicName);
	static void UpdateMusic(std::string_view musicName);

private:
	inline static std::unordered_map<std::string, Sound> mSfxes;
	inline static std::unordered_map<std::string, Music> mMusic;
};

