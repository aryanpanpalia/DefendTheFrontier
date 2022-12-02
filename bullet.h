#pragma once

#include <vector2d.h>
#include "image.h"

class Bullet {
    public:
        int width, height;
        Vector2D pos, vel, force, velG, forceG;
        float mass;
        float angle;
        Image bulletImages[4];
        int frameCount, imageIndex;

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
        Image trackerBulletImages[2];

        TrackerBullet(float initialX, float initialY, Player *player);
        bool pointInBullet(int px, int py);
        void update();
        void render();
};
