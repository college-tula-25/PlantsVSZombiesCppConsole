#pragma once
#include "../InstantEffect.h"

class CherryBomb : public InstantEffect
{
public:
    CherryBomb(Vector2 pos)
        : InstantEffect(pos, {
            "  _   _  ",
            " (o) (o) ",
            "  \\___/  "
            }, 300, 150, 1, 1800)
    {
    }

    void Update(float deltaTime) override
    {
        if (timeToExplode > 0.0f && timeToExplode <= 0.3f)
        {
            sprite = {
                "  _   _  ",
                " (I) (I) ",
                "  \\!!!/  "
            };
        }

        InstantEffect::Update(deltaTime);
    }
};
