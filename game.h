#pragma once

#include <vector>
#include "player.h"
#include "bullet.h"
#include "enemy.h"

class Game {
    public:
        Player player;
        std::vector<Bullet> bullets;
        std::vector<TrackerBullet> trackerBullets;
        std::vector<Enemy> enemies;
        float lastEnemySpawnTime;
        bool gameOver;
        int score;
        int numEnemiesKilled;
        int numShots;
        int numDeaths;
        int difficulty;

        Game(int diff);
        void render();
        void update();
        void spawnEnemy();
        void playerOutOfBounds();
        void handleCollisions();
};
