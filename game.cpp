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
    lastEnemySpawnTime = TimeNow();
    gameOver = false;
    score = 0;
    numEnemiesKilled = 0;
    numShots = 0;
    numDeaths = 0;
    difficulty = diff;
}

void Game::render() {
    LCD.Clear();

    player.render();

    for(Bullet &bullet: bullets) {
        bullet.render();
    }

    for(Enemy &enemy: enemies) {
        if(enemy.onScreen) {
            enemy.render();
        }
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

        // int bRadius = bullet.radius;
        int bWidth = bullet.width;
        int bHeight = bullet.height;
        
        // Update bullet object
        bullet.update();

        // If the bullet touches the edge, remove it from bullets vector
        if(bullet.pos.x <= 0 || bullet.pos.x >= WINDOW_WIDTH - bWidth || bullet.pos.y <= 0 || bullet.pos.y >= WINDOW_HEIGHT - bHeight) {
            bullets.erase(bullets.begin() + i, bullets.begin() + i + 1);
            i--;
        }
    }

    for(int i = 0; i < enemies.size(); i++) {
        // Create alias for enemies[i]
        Enemy &enemy = enemies[i];

        enemy.update();

        // If an enemy exits the screen, remove it from the enemies vector
        // If an enemy enters the screen, set onScreen property to true
        if(enemy.pos.x <= 0 || enemy.pos.x >= WINDOW_WIDTH - enemy.width || enemy.pos.y <= 0 || enemy.pos.y >= WINDOW_HEIGHT - enemy.height) {
            if(enemy.onScreen) {
                enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
                i--;
            }
        } else {
            if(!enemy.onScreen) {
                enemy.onScreen = true;
                enemies[rand() % enemies.size()].shoot();
            }
        }
    }

    for(int i = 0; i < trackerBullets.size(); i++) {
        // Create alias to trackerBullets[i]
        TrackerBullet &trackerBullet = trackerBullets[i];

        int bRadius = trackerBullet.radius;
        
        // Update trackerBullet object
        trackerBullet.update();

        // If the trackerBullet touches the edge, remove it from trackerBullets vector
        if(trackerBullet.pos.x <= bRadius || trackerBullet.pos.x >= WINDOW_WIDTH - bRadius || trackerBullet.pos.y <= bRadius || trackerBullet.pos.y >= WINDOW_HEIGHT - bRadius) {
            trackerBullets.erase(trackerBullets.begin() + i, trackerBullets.begin() + i + 1);
            i--;
        }
    }

    // Gets time since last enemy was spawned
    float timeSinceNewEnemy = TimeNow() - lastEnemySpawnTime;

    // If it has been 2 seconds since an enemy was spawned, spawn a new enemy and reset the spawn timer
    if(timeSinceNewEnemy > 2) {
        float initialX, initialY, angle;

        int side = rand() % 4;
        switch(side) {
            case 0:
                initialX = 340;
                initialY = (rand() % 280) - 20;
                break;
            case 1:
                initialX = (rand() % 360) - 20;
                initialY = -20;
                break;
            case 2:
                initialX = -20;
                initialY = (rand() % 280) - 20;
                break;
            case 3:
                initialX = (rand() % 360) - 20;
                initialY = 260;
                break;
        }

        angle = atan2(initialY - player.pos.y, player.pos.x - initialX);

        // Append new enemy to enemies vector
        enemies.push_back(Enemy(initialX, initialY, angle, this));
        lastEnemySpawnTime = TimeNow();
    }
}

void Game::handleCollisions() {    
    // for each enemy
    for (int i = 0; i < enemies.size(); i++) {
        int *playerImageArray = player.playerImage.saved_image;
        for(int row = 0; row < player.height && !gameOver; row++) {
            for(int col = 0; col < player.width; col++){
                int x = player.pos.x + col;
                int y = player.pos.y + row;

                if(enemies[i].pointInEnemy(x, y) && playerImageArray[row * player.height + col] != -1) {
                    gameOver = true;
                    break;
                }
            }
        }

        // for each bullet
        for(int j = 0; j < bullets.size(); j++) {
            Bullet &bullet = bullets[j];
            int *bulletImageArray = bullet.bulletImage.saved_image;

            for(int row = 0; row < bullet.height; row++) {
                for(int col = 0; col < bullet.width; col++){
                    int x = bullet.pos.x + col;
                    int y = bullet.pos.y + row;

                        if(enemies[i].pointInEnemy(x, y) && bulletImageArray[row * bullet.height + col] != -1) {
                        // remove bullet and enemy
                        bullets.erase(bullets.begin() + j, bullets.begin() + j + 1);
                        j--;

                        enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
                        i--;

                        // update number of enemies killed
                        numEnemiesKilled++;

                        // give player 2 ammo
                        player.ammo += 2;
                    }
                }
            }
        }
    }

    // for each tracker bullet
    for (int i = 0; i < trackerBullets.size(); i++) {
        int *playerImageArray = player.playerImage.saved_image;
        bool trackerBulletHit = false;
        for(int row = 0; row < player.height && !trackerBulletHit; row++) {
            for(int col = 0; col < player.width; col++){
                int x = player.pos.x + col;
                int y = player.pos.y + row;

                if(trackerBullets[i].pointInBullet(x, y) && playerImageArray[row * player.height + col] != -1) {
                    player.force.reset();
                    player.vel = player.vel.add(trackerBullets[i].vel);
                    trackerBullets.erase(trackerBullets.begin() + i, trackerBullets.begin() + i + 1);
                    i--;
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
