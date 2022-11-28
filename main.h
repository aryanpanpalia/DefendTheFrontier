#ifndef MAIN_H
#define MAIN_H

#include <vector>

class Game;

class Vector2D {
    public:
        float x, y;

        Vector2D();
        Vector2D(float a, float b);
        Vector2D add(Vector2D other);
        Vector2D sub(Vector2D other);
        Vector2D mult(float other);
        Vector2D div(float other);
        float magnitude();
        void set(float a, float b);
        void reset();
};

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

class Bullet {
    public:
        int radius;
        Vector2D pos, vel, force, vegG, forceG;
        float mass;
        float angle;

        Bullet(float initialX, float initialY, float angle);
        Vector2D getCenter();
        bool pointInBullet(int px, int py);
        void update();
        void render();
};

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