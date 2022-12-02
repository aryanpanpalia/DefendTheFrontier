#include "game.h"
#include "player.h"
#include "bullet.h"
#include "FEHLCD.h"
#include "FEHUtility.h"
#include <math.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240
#define PI 3.1415

Game::Game(int diff) {
    player.game = this;
    lastEnemySpawnTime = -1;
    gameOver = false;
    score = 0;
    numEnemiesKilled = 0;
    numShots = 0;
    numDeaths = 0;
    
    difficulty = diff;

    if(difficulty == 0) {
        player.ammo = 20;
        hasTrackerBullets = false;
        trackerBulletsKill = false;
        ammoPerKill = 4;
        timeBetweenEnemySpawns = 3;
    } else if(difficulty == 1) {
        player.ammo = 10;
        hasTrackerBullets = false;
        trackerBulletsKill = false;
        ammoPerKill = 2;
        timeBetweenEnemySpawns = 2;
    } else if(difficulty == 2) {
        player.ammo = 10;
        hasTrackerBullets = true;
        trackerBulletsKill = false;
        knockBack = 4;
        ammoPerKill = 2;
        timeBetweenEnemySpawns = 2;
    } else if(difficulty == 3) {
        player.ammo = 10;
        hasTrackerBullets = true;
        trackerBulletsKill = true;
        knockBack = 0;
        ammoPerKill = 1;
        timeBetweenEnemySpawns = 1;
    }
}

void Game::render() {
    LCD.Clear();

    player.render();

    for(Bullet &bullet: bullets) {
        bullet.render();
    }

    for(Enemy &enemy: enemies) {
        enemy.render();
    }

    for(TrackerBullet &trackerBullet: trackerBullets) {
        trackerBullet.render();
    }

    LCD.WriteAt("Ammo: ", 0 , 0);
    LCD.WriteAt(player.ammo, 60, 0);

    score = numEnemiesKilled * 10;
    LCD.WriteAt("Score: ", 200, 0);
    LCD.WriteAt(score, 284, 0);

    LCD.Update();
}

void Game::update() {
    // Update player object
    player.update();

    for(int i = 0; i < bullets.size(); i++) {
        // Create alias to bullets[i]
        Bullet &bullet = bullets[i];
        
        // Update bullet object
        bullet.update();

        // If the bullet touches the edge, remove it from bullets vector
        if(bullet.pos.x <= 0 || bullet.pos.x >= WINDOW_WIDTH - bullet.width || bullet.pos.y <= 0 || bullet.pos.y >= WINDOW_HEIGHT - bullet.height) {
            bullets.erase(bullets.begin() + i, bullets.begin() + i + 1);
            i--;
        }
    }

    for(int i = 0; i < enemies.size(); i++) {
        // Create alias for enemies[i]
        Enemy &enemy = enemies[i];

        // Update enemy object
        enemy.update();

        // If an enemy exits the screen, remove it from the enemies vector
        if(enemy.pos.x <= 0 || enemy.pos.x >= WINDOW_WIDTH - enemy.width || enemy.pos.y <= 0 || enemy.pos.y >= WINDOW_HEIGHT - enemy.height) {
            enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
            i--;
        } 
    }

    for(int i = 0; i < trackerBullets.size(); i++) {
        // Create alias to trackerBullets[i]
        TrackerBullet &trackerBullet = trackerBullets[i];
        
        // Update trackerBullet object
        trackerBullet.update();

        // If the trackerBullet touches the edge, remove it from trackerBullets vector
        if(trackerBullet.pos.x <= 0 || trackerBullet.pos.x >= WINDOW_WIDTH - trackerBullet.width || trackerBullet.pos.y <= 0 || trackerBullet.pos.y >= WINDOW_HEIGHT - trackerBullet.height) {
            trackerBullets.erase(trackerBullets.begin() + i, trackerBullets.begin() + i + 1);
            i--;
        }
    }

    // Gets time since last enemy was spawned
    float timeSinceNewEnemy = TimeNow() - lastEnemySpawnTime;

    // If it has been 2 seconds since an enemy was spawned, spawn a new enemy, reset the spawn timer, and shoot a tracker bullet
    if(timeSinceNewEnemy > timeBetweenEnemySpawns) {
        spawnEnemy();
        lastEnemySpawnTime = TimeNow();

        // Shoot a tracker bullet from a random enemy
        if(hasTrackerBullets){
            enemies[rand() % enemies.size()].shoot();
        }
    }
}

void Game::spawnEnemy() {
    float initialX, initialY, angle;

    int side = rand() % 4;
    switch(side) {
        case 0:
            initialX = 320 - Enemy::width;
            initialY = (rand() % (240 - Enemy::height));
            break;
        case 1:
            initialX = (rand() % (320 - Enemy::width));
            initialY = 0;
            break;
        case 2:
            initialX = 0;
            initialY = (rand() % (240 - Enemy::height));
            break;
        case 3:
            initialX = (rand() % 320 - Enemy::width);
            initialY = 220;
            break;
    }

    angle = atan2(initialY - player.pos.y, player.pos.x - initialX);

    // Append new enemy to enemies vector
    enemies.push_back(Enemy(initialX, initialY, angle, this));
}

void Game::handleCollisions() {
    // Stores player's image array
    int *playerImageArray = player.playerImage.saved_image;
    
    for (int i = 0; i < enemies.size(); i++) {
        Enemy &enemy = enemies[i];
        int *enemyImageArray = enemies[i].enemyImage.saved_image;

        /*
            Enemy - Player Collisions
        */

        // Go through the rows and columns of player object
        for(int playerRow = 0; playerRow < player.height && !gameOver; playerRow++) {
            for(int playerCol = 0; playerCol < player.width; playerCol++){
                // Find corresponding (x, y) position on the screen for the playerRow and playerCol
                int x = player.pos.x + playerCol;
                int y = player.pos.y + playerRow;

                // Checks if this (x, y) is in the enemy's general zone and can be accessed through its image
                if(enemies[i].pointInEnemy(x, y)){
                    // Finds the corresponding row and column on the enemy image
                    int enemyRow = y - enemy.pos.y;
                    int enemyCol = x - enemy.pos.x;

                    // Checks if both image arrays contain a drawn pixel at this (x, y)
                    if(enemyImageArray[enemyRow * enemy.height + enemyCol] != -1 && playerImageArray[playerRow * player.height + playerCol] != -1) {
                        gameOver = true;
                        break;
                    }
                }
            }
        }

        /*
            Enemy - Bullet collisions
        */

        for(int j = 0; j < bullets.size(); j++) {
            Bullet &bullet = bullets[j];

            // Stores bullet's image array
            int *bulletImageArray = bullet.bulletImages[bullet.imageIndex].saved_image;

            // Go through the rows and columns of the bullet object
            for(int bulletRow = 0; bulletRow < bullet.height; bulletRow++) {
                for(int bulletCol = 0; bulletCol < bullet.width; bulletCol++){
                    // Find the corresponding (x, y) position on the screen for the bulletRow and bulletCol
                    int x = bullet.pos.x + bulletCol;
                    int y = bullet.pos.y + bulletRow;

                    // Checks if this (x, y) is in the enemy's general zone and can be accessed through its image
                    if(enemies[i].pointInEnemy(x, y)){
                        // Finds the corresponding row and column on the enemy image
                        int enemyRow = y - enemy.pos.y;
                        int enemyCol = x - enemy.pos.x;

                        // Checks if both image arrays contain a drawn pixel at this (x, y)
                        if(enemyImageArray[enemyRow * enemy.height + enemyCol] != -1 && bulletImageArray[bulletRow * bullet.height + bulletCol] != -1) {
                            // Remove both the bullet and the enemy
                            enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
                            bullets.erase(bullets.begin() + j, bullets.begin() + j + 1);
                            
                            // Update indices
                            i--;
                            
                            // Update number of enemies killed
                            numEnemiesKilled++;

                            // Give player ammo
                            player.ammo += ammoPerKill;

                            goto breakOutOfBulletLoop;
                        }
                    }
                }
            }
        } breakOutOfBulletLoop:;
    }

    /*
        Player - TrackerBullet collisions
    */

    for (int i = 0; i < trackerBullets.size(); i++) {
        // Tracks whether the current trackerBullet has hit the player
        bool trackerBulletHit = false;

        // Go through the rows and columns of player object
        for(int playerRow = 0; playerRow < player.height && !trackerBulletHit; playerRow++) {
            for(int playerCol = 0; playerCol < player.width; playerCol++) {
                // Find corresponding (x, y) position on the screen for the playerRow and playerCol
                int x = player.pos.x + playerCol;
                int y = player.pos.y + playerRow;

                // Checks if both the trackerBullet object is at (x, y) and the playerImage is rendered there
                if(trackerBullets[i].pointInBullet(x, y) && playerImageArray[playerRow * player.height + playerCol] != -1) {
                    // Reset's player's force and set's its velocity to the trackerBullet's
                    if(trackerBulletsKill) {
                        gameOver = true;
                    } else {
                        player.force.reset();
                        player.vel = player.vel.add(trackerBullets[i].vel.norm().mult(knockBack));

                        // Removes the tracker bullet
                        trackerBullets.erase(trackerBullets.begin() + i, trackerBullets.begin() + i + 1);
                        i--;
                    }

                    trackerBulletHit = true;
                    break;
                }
            }
        }
    }
}

void Game::playerOutOfBounds() {
    if (player.pos.x <= 0) {
        player.pos.x = 0;
        gameOver = true;
    }
    if (player.pos.x >= WINDOW_WIDTH - player.width) {
        player.pos.x = WINDOW_WIDTH - player.width;
        gameOver = true;
    }
    if (player.pos.y <= 0) {
        player.pos.y = 0;
        gameOver = true;
    }
    if (player.pos.y >= WINDOW_HEIGHT - player.height) {
        player.pos.y = WINDOW_HEIGHT - player.height;
        gameOver = true;
    }
}
