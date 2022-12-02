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

        // Statistics to keep track of
        int score, numEnemiesKilled, numShots, numDeaths;
        
        // The difficulty mode and what it affects
        int difficulty;
        bool hasTrackerBullets;
        bool trackerBulletsKill;
        int knockBack;
        int ammoPerKill;
        int timeBetweenEnemySpawns;

        Image backgroundImage;
        int backgroundImageX, backgroundImageY;

        Game(int diff);
        void render();
        void update();
        void spawnEnemy();
        void playerOutOfBounds();
        void handleCollisions();
};
