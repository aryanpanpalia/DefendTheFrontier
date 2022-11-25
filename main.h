#ifndef MAIN_H
#define MAIN_H

#include <vector>

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
        void update();
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
        void update();
        void render();
};

class Enemy {
    public:
        int width, height;
        float x, y, velX, velY, accelX, accelY, forceX, forceY, velG, forceG;
        float mass;
        bool onScreen;

        Enemy(float initialX, float initialY, float angle);
        int* getCenter();
        bool pointInEnemy(int px, int py);
        void update();
        void render();  
};

class Game {
    public:
        Player player;
        std::vector<Bullet> bullets;
        std::vector<Enemy> enemies;

        float lastEnemySpawnTime;

        Game();
        ~Game();
        void render();
        void update();
        bool hasEnded();
};

#endif