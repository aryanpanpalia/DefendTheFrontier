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
        bool active;

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
        bool active, activated;

        Enemy(float initialX, float initialY, float angle);
        int* getCenter();
        bool pointInEnemy(int px, int py);
        void update();
        void render();  
};

class Game {
    public:
        Player player;
        int numBullets, numEnemies;
        Bullet *bullets;
        Enemy *enemies;

        float lastEnemySpawnTime;

        Game();
        ~Game();
        void render();
        void update();
        bool hasEnded();
};

#endif