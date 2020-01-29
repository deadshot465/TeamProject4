#include "SFX.h"

void SFX::Initialize()
{
	mSfxes.reserve(200);
	InitAudioDevice();
}

void SFX::Release()
{
	for (auto& [name, sound] : mSfxes)
		UnloadSound(sound);

	for (auto& [name, music] : mMusic)
		UnloadMusicStream(music);

	CloseAudioDevice();
}

void SFX::LoadSfx(std::string_view fileName, std::string_view sfxName)
{
	mSfxes[sfxName.data()] = LoadSound(fileName.data());
}

void SFX::LoadMusicFile(std::string_view fileName, std::string_view musicName)
{
	mMusic[musicName.data()] = LoadMusicStream(fileName.data());
}

void SFX::PlaySfx(std::string_view sfxName)
{
	PlaySound(mSfxes[sfxName.data()]);
}

void SFX::PlayMusic(std::string_view musicName)
{
	PlayMusicStream(mMusic[musicName.data()]);
}

void SFX::UpdateMusic(std::string_view musicName)
{
	UpdateMusicStream(mMusic[musicName.data()]);
}
