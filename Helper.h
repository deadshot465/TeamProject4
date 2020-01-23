#pragma once
#include <sstream>
#include <string>
#include <random>
#include <raylib.h>

inline constexpr Color ENEMY_COLORS[] = {
	RED, GREEN, BLUE
};

inline constexpr auto SCREEN_LEFT_BOUND = -2500;
inline constexpr auto SCREEN_RIGHT_BOUND = 2500;
inline constexpr auto SCREEN_UPPER_BOUND = -2500;
inline constexpr auto SCREEN_LOWER_BOUND = 2500;

template <typename T = std::mt19937, size_t N = T::state_size>
inline auto GetRandomSeededEngine() -> typename std::enable_if<N != 0, T>::type
{
	typename T::result_type data[N];
	std::random_device rd;
	std::generate(std::begin(data), std::end(data), std::ref(rd));
	std::seed_seq seeds(std::begin(data), std::end(data));
	T engine(seeds);
	return engine;
}

inline auto RANDOM_ENGINE = GetRandomSeededEngine();

template <typename T = float>
inline T GetRandomNumber(T lower, T upper)
{
	std::uniform_real_distribution<T> rng(lower, upper);
	return rng(RANDOM_ENGINE);
}

template <>
inline int GetRandomNumber<int>(int lower, int upper)
{
	std::uniform_int_distribution<int> rng(lower, upper);
	return rng(RANDOM_ENGINE);
}

inline std::string GetRandomString(size_t length)
{
	static const std::string random_chars = "01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::uniform_int_distribution<> rng(0, static_cast<int>(random_chars.size()) - 1);

	auto ss = std::stringstream();
	
	for (auto i = 0; i < length; ++i) {
		ss << random_chars[rng(RANDOM_ENGINE)];
	}

	return ss.str();
}

inline bool operator==(const Color& color1, const Color& color2)
{
	return color1.r == color2.r &&
		color1.g == color2.g &&
		color1.b == color2.b &&
		color1.a == color2.a;
}

inline constexpr bool CheckBound(const glm::vec2& position)
{
	return position.x > SCREEN_RIGHT_BOUND ||
		position.x < SCREEN_LEFT_BOUND ||
		position.y < SCREEN_UPPER_BOUND ||
		position.y > SCREEN_LOWER_BOUND;
}

inline Vector2 ToRaylibVector2(const glm::vec2& vec)
{
	Vector2 _vec;
	_vec.x = vec.x;
	_vec.y = vec.y;
	return _vec;
}

inline glm::vec2 ToGlmVector2(const Vector2& vec)
{
	return glm::vec2(vec.x, vec.y);
}