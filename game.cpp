#include "game.h"
#include "player.h"
#include "FEHLCD.h"
#include "FEHUtility.h"
#include <math.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240
#define PI 3.1415

Game::Game() {
    player.game = this;
    lastEnemySpawnTime = TimeNow();
    gameOver = false;
    numEnemiesKilled = 0;
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

    LCD.WriteAt("Ammo: ", 0 , 0);
    LCD.WriteAt(player.ammo, 60, 0);

    int score = numEnemiesKilled * 10;
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

        int bRadius = bullet.radius;
        
        // Update bullet object
        bullet.update();

        // If the bullet touches the edge, remove it from bullets vector
        if(bullet.pos.x <= bRadius || bullet.pos.x >= WINDOW_WIDTH - bRadius || bullet.pos.y <= bRadius || bullet.pos.y >= WINDOW_HEIGHT - bRadius) {
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
            }
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
        enemies.push_back(Enemy(initialX, initialY, angle));
        lastEnemySpawnTime = TimeNow();
    }
}

void Game::handleCollisions() {
    // initialize variables to store player attributes
    Vector2D playerCenter = player.getCenter();
    float halfPlayerHeight = player.height / 2.0;
    float halfPlayerWidth = player.width / 2.0;
    
    // calculate radius of circle circumscribing player square
    float playerRadius = sqrt(halfPlayerHeight*halfPlayerHeight + halfPlayerWidth*halfPlayerWidth);

    // for each enemy
    for (int i = 0; i < enemies.size(); i++) {
        // initialize variables with enemy attributes
        Vector2D enemyPosition = enemies[i].getCenter();
        float halfEnemyHeight = enemies[i].height/2.0; 
        float halfEnemyWidth = enemies[i].width/2.0;
        
        // calculate radius of circle circumscribing enemy square
        float enemyRadius = sqrt(pow(halfEnemyHeight, 2) + pow(halfEnemyWidth, 2));

        // if player and enemy objects are within their radii
        if (playerCenter.sub(enemyPosition).magnitude() < playerRadius+enemyRadius) {
            // if player and enemy objects intersect
            if (playerCenter.y-halfPlayerHeight < enemyPosition.y+halfEnemyHeight && 
                    playerCenter.y+halfPlayerHeight > enemyPosition.y-halfEnemyHeight && 
                    playerCenter.x-halfPlayerWidth < enemyPosition.x+halfEnemyWidth && 
                    playerCenter.x+halfPlayerWidth > enemyPosition.x-halfEnemyWidth) {
                // end game
                gameOver = true;
            }
        }

        // for each bullet
        for (int j = 0; j < bullets.size(); j++) {
            // initialize variables with bullet attributes
            Vector2D bulletPosition = bullets[j].getCenter();
            float bulletRadius = bullets[j].radius;

            // if enemy and bullet objects are within their radii
            if (enemyPosition.sub(bulletPosition).magnitude() < enemyRadius+bulletRadius) {
                // if enemy and bullet objects intersect
                if (bulletPosition.y-bulletRadius < enemyPosition.y+halfEnemyHeight && 
                        bulletPosition.y+bulletRadius > enemyPosition.y-halfEnemyHeight && 
                        bulletPosition.x-bulletRadius < enemyPosition.x+halfEnemyWidth && 
                        bulletPosition.x+bulletRadius > enemyPosition.x-halfEnemyWidth) {
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
