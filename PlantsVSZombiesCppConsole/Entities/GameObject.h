#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "../Core/Types/Vector2/Vector2.h"

class GameObject
{
protected:
	Vector2 position{};
	std::vector<std::wstring> sprite{};
	int width{ 0 };
	int height{ 0 };
	bool activeState{ true };

public:
	virtual ~GameObject() = default;

	GameObject(Vector2 pos, const std::vector<std::wstring>& spr)
		: position(pos), sprite(spr), height(static_cast<int>(spr.size()))
	{
		size_t maxWidth = 0;
		for (const auto& row : spr)
		{
			maxWidth = std::max(maxWidth, row.size());
		}
		width = spr.empty() ? 0 : static_cast<int>(maxWidth);
	}

	explicit GameObject(Vector2 pos)
		: position(pos), sprite({}), width(0), height(0) {
	}

	virtual void Update(float deltaTime) {}

	const std::vector<std::wstring>& GetSprite() const { return sprite; }
	Vector2 GetPosition() const { return position; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	bool IsActive() const { return activeState; }

	void SetPosition(const Vector2& pos) { this->position = pos; }
	void Move(const Vector2& offset) { this->position += offset; }
	void SetActive(bool state) { activeState = state; }
};
