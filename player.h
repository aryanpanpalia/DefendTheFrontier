#pragma once

#include "vector2d.h"

class Game;

class Player {
    public:
        Game *game;
        int width, height;
        Vector2D pos, vel, force, velG, forceG;
        float mass;

        Player();
        Vector2D getCenter();
        bool pointInPlayer(int px, int py);
        void update();
        void shoot(float angle);
        void render();
};
