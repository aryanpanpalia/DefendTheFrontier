#pragma once

#include <vector>
#include "player.h"
#include "bullet.h"
#include "enemy.h"

/*
    Game Class: 

    Contains variables that stores:
        player: player object
        bullets: vector containing the bullet objects on the screen
        trackerBullets: vector containing the tracker bullet objects on the screen
        enemies: vector containing the enemy objects on the screen
        lastEnemySpawnTime: the time the last enemy was spawned at
        gameOver: whether the game has ended
        score: the score the player has achieved
        numEnemiesKilled: the number of enemies the player has killed
        numShots: the number of shots the player has made
        difficulty: the difficulty level (0: easy, 1: medium: 2: hard, 3: extreme)
        hasTrackerBullets: whether the game will have tracker bullets
        trackerBulletsKill: whether the tracker bullets will kill the player
        knockback: how far trackerBullets will knock the player back
        ammoPerKill: the amount of ammo the player gets upon killing an enemy
        timeBetweenEnemySpawns: the amount of time between enemy spawns
        theme: the theme of the game (0: space, 1: western)
        backgroundImage: the background image for the game
        backgroundImageX: the x position the background image will be rendered at
        backgroundImageY: the y position the background image will be rendered at

    Contains methods that:
        Game(int): creates and sets up the game based on the difficulty passed as a parameter
        render(): draws the game and game objects to the screen
        update(): updates the game variables and game objects
        spawnEnemy(): spawns an enemy
        playerOutOfBounds: checks if player is out of bounds and if so, sets their position to a close point on the edge and ends the game
        handleCollisions: handles collisions between game objects

        Authors: Aryan Panpalia
*/
class Game {
    public:
        Player player;
        std::vector<Bullet> bullets;
        std::vector<TrackerBullet> trackerBullets;
        std::vector<Enemy> enemies;
        float lastEnemySpawnTime;
        bool gameOver;

        // Statistics to keep track of
        int score, numEnemiesKilled, numShots;
        
        // The difficulty mode and what it affects
        int difficulty;
        bool hasTrackerBullets;
        bool trackerBulletsKill;
        int knockBack;
        int ammoPerKill;
        int timeBetweenEnemySpawns;

        int theme;

        Image backgroundImage;
        int backgroundImageX, backgroundImageY;

        Game(int diff, int thm);
        void render();
        void update();
        void spawnEnemy();
        void playerOutOfBounds();
        void handleCollisions();
};
