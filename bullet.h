#pragma once

#include <vector2d.h>

class Bullet {
    public:
        int radius;
        Vector2D pos, vel, force, velG, forceG;
        float mass;
        float angle;

        Bullet();
        Bullet(float initialX, float initialY, float angle);
        Vector2D getCenter();
        bool pointInBullet(int px, int py);
        void update();
        void render();
};

class Player;

class TrackerBullet: public Bullet {
    public:
        Player *player;

        TrackerBullet(float initialX, float initialY, Player *player);
        void update();
        void render();
};
