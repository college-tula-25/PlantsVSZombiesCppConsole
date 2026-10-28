#pragma once
#include <vector>
#include <string>
#include "Types.cpp"

class GameObject
{
protected:
	Vector2 position{};
	std::vector<std::string> sprite{};
	int width{ 0 };
	int height{ 0 };

public:
	virtual ~GameObject() = default;

	GameObject(Vector2 pos, const std::vector<std::string>& spr)
		: position(pos), sprite(spr)
	{
		height = static_cast<int>(spr.size());
		// »справлено: ширина Ч это длина ѕ≈–¬ќ… строки, а не количество строк
		width = spr.empty() ? 0 : static_cast<int>(spr[0].size());
	}

	const std::vector<std::string>& GetSprite() const { return sprite; }
	Vector2 GetPosition() const { return position; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
};
