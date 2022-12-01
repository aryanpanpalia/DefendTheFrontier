#pragma once

#include <vector2d.h>
#include "image.h"

class Game;

class Enemy {
    public:
        Game *game;
        int width, height;
        Vector2D pos, vel, force, velG, forceG;
        float mass;
        bool onScreen;
        Image enemyImage;

        Enemy(float initialX, float initialY, float angle, Game *game);
        Vector2D getCenter();
        bool pointInEnemy(int px, int py);
        void update();
        void render();
        void shoot(); 
};
