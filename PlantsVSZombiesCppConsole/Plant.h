#pragma once
#include "GameObject.h"

class Plant : public GameObject
{
protected:
	int hp;
public:
	Plant(Vector2 pos, const std::vector<std::string>& spr, int health)
		: GameObject(pos, spr), hp(health) {
	}

	virtual ~Plant() = default;

	int GetHp() const { return hp; }
	void TakeDamage(int amount) { hp -= amount; }
};
