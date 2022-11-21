#ifndef MAIN_H
#define MAIN_H

class Game;

class Player {
    public:
        Game *game;
        int width, height;
        float x, y, velX, velY, accelX, accelY, forceX, forceY, velG, forceG;
        float mass;

        Player();
        int* getCenter();
        bool pointInPlayer(int px, int py);
        void applyPhysics(float dt);
        void shoot(float angle);
        void render();
};

class Bullet {
    public:
        int radius;
        float x, y, velX, velY, accelX, accelY, forceX, forceY, velG, forceG;
        float mass;
        float angle;

        Bullet(float initialX, float initialY, float angle);
        int* getCenter();
        bool pointInBullet(int px, int py);
        void applyPhysics(float dt);
        void render();
};

class Game {
    public:
        Player player;
        int numBullets;
        Bullet *bullets;

        Game();
        ~Game();
        void render();
        void update();
};

#endif