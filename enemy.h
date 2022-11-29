#pragma once

#include <vector2d.h>

class Enemy {
    public:
        int width, height;
        Vector2D pos, vel, force, velG, forceG;
        float mass;
        bool onScreen;

        Enemy(float initialX, float initialY, float angle);
        Vector2D getCenter();
        bool pointInEnemy(int px, int py);
        void update();
        void render();  
};
