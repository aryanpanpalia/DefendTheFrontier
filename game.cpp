#include "game.h"
#include "player.h"
#include "bullet.h"
#include "FEHLCD.h"
#include "FEHUtility.h"
#include <math.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240
#define PI 3.1415

/*
    Creates and sets up the game based on the difficulty and theme passed as parameters

    Parameters:
        diff: the difficulty level of the game
        thm: the theme of the game

    Return value: none

    Authors: Aryan Panpalia
*/
Game::Game(int diff, int thm) : player(thm) {
    player.game = this;
    lastEnemySpawnTime = -1;
    gameOver = false;
    score = 0;
    numEnemiesKilled = 0;
    numShots = 0;
    
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

    theme = thm;

    if(theme == 0) {
        backgroundImage.Open("SpaceBackground.pic");
    } else if (theme == 1) { 
        backgroundImage.Open("WesternBackground.pic");
    }

    backgroundImageX = 0; 
    backgroundImageY = 0;
}

/*
    Draws the game and game objects to the screen

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
void Game::render() {
    LCD.Clear();

    // Tile the background images so that as it ends on one side it starts on another
    backgroundImage.Draw(backgroundImageX, backgroundImageY);
    backgroundImage.Draw(backgroundImageX + backgroundImage.cols, backgroundImageY);
    
    // Scroll the image
    backgroundImageX--;
    backgroundImageX %= backgroundImage.cols;

    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

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

    LCD.WriteAt("Ammo: ", 5, 5);
    LCD.WriteAt(player.ammo, 65, 5);

    score = numEnemiesKilled * 10;
    
    int digitsInScore = 1;
    if(score >= 10) {
        digitsInScore = log10(score) + 1;
    }

    int scoreDigitsWidth = 12 * digitsInScore;
    int scoreTotalWidth = 84 + scoreDigitsWidth;
    
    LCD.WriteAt("Score: ", WINDOW_WIDTH - scoreTotalWidth - 5, 5);
    LCD.WriteAt(score, WINDOW_WIDTH - scoreDigitsWidth - 5, 5);

    LCD.Update();
}

/*
    Updates the game variables and game objects

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
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
            // Close the bullet images
            for(int c = 0; c < 4; c++) {
                bullet.bulletImages[c].Close();
            }

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
            // Close the enemy image
            enemy.enemyImage.Close();
            
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
            // Close the tracker bullet images
            for(int c = 0; c < 2; c++) {
                trackerBullet.trackerBulletImages[c].Close();
            }
            
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

/*
    Spawns an enemy

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Game::spawnEnemy() {
    float initialX, initialY, angle;

    Enemy tempEnemy(0, 0, 0, this);

    int side = rand() % 4;
    switch(side) {
        case 0:
            initialX = WINDOW_WIDTH - tempEnemy.width - 1;
            initialY = rand() % (WINDOW_HEIGHT - tempEnemy.height - 1);
            break;
        case 1:
            initialX = rand() % (WINDOW_WIDTH - tempEnemy.width - 1);
            initialY = 1;
            break;
        case 2:
            initialX = 1;
            initialY = (rand() % (WINDOW_HEIGHT - tempEnemy.height - 1));
            break;
        case 3:
            initialX = rand() % (WINDOW_WIDTH - tempEnemy.width - 1);
            initialY = WINDOW_HEIGHT - tempEnemy.height - 1;
            break;
    }

    angle = atan2(initialY - player.pos.y, player.pos.x - initialX);

    // Append new enemy to enemies vector
    enemies.push_back(Enemy(initialX, initialY, angle, this));
}

/*
    Handles collisions between game objects

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
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
                    if(enemyImageArray[enemyRow * enemy.width + enemyCol] != -1 && playerImageArray[playerRow * player.width + playerCol] != -1) {
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
                        if(enemyImageArray[enemyRow * enemy.width + enemyCol] != -1 && bulletImageArray[bulletRow * bullet.width + bulletCol] != -1) {
                            // Close the enemy image
                            enemy.enemyImage.Close();
                            
                            // Close the bullet images
                            for(int c = 0; c < 4; c++) {
                                bullet.bulletImages[c].Close();
                            }

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
        TrackerBullet &trackerBullet = trackerBullets[i];
        int *trackerBulletImageArray = trackerBullet.trackerBulletImages[trackerBullet.imageIndex].saved_image;

        // Tracks whether the current trackerBullet has hit the player
        bool trackerBulletHit = false;

        // Go through the rows and columns of player object
        for(int playerRow = 0; playerRow < player.height && !trackerBulletHit; playerRow++) {
            for(int playerCol = 0; playerCol < player.width; playerCol++) {
                // Find corresponding (x, y) position on the screen for the playerRow and playerCol
                int x = player.pos.x + playerCol;
                int y = player.pos.y + playerRow;

                // Checks if this (x, y) is in the tracker bullet's general zone and can be accessed through its image
                if(trackerBullet.pointInBullet(x, y)) {
                    // Finds the corresponding row and column on the tracker bullet image
                    int trackerBulletRow = y - trackerBullet.pos.y;
                    int trackerBulletCol = x - trackerBullet.pos.x;

                    // Checks if both image arrays contain a drawn pixel at this (x, y)
                    if(trackerBulletImageArray[trackerBulletRow * trackerBullet.width + trackerBulletCol] != -1 && playerImageArray[playerRow * player.width + playerCol] != -1) {
                        // Closes the tracker bullet images
                        for(int c = 0; c < 2; c++) {
                            trackerBullet.bulletImages[c].Close();
                        }
                        
                        if(trackerBulletsKill) {
                            gameOver = true;
                        } else {
                            // Reset's player's force and set's its velocity to the trackerBullet's
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
}

/*
    Checks if player is out of bounds and if so, sets their position to a close point on the edge and ends the game

    Parameters: none
    Return value: none

    Authors: Thomas Banko
*/
void Game::playerOutOfBounds() {
    // Checks if player is out of bounds
    if (player.pos.x <= 1 || player.pos.x >= WINDOW_WIDTH - player.width - 1 || 
            player.pos.y <= 1 || player.pos.y >= WINDOW_HEIGHT - player.height - 1) {
        // initializes varables for calculations
        Vector2D prevPos = player.pos.sub(player.vel.add(player.velG));
        float slope, tempX = player.pos.x, tempY = player.pos.y;
        
        // recalculates position if player x pos is out of bounds
        if (tempX <= 1) {
            slope = (player.vel.y + player.velG.y)/player.vel.x;
            tempY = prevPos.y + slope * (1 - prevPos.x);
            tempX = 1;
        } else if (tempX >= WINDOW_WIDTH - player.width - 1) {
            slope = (player.vel.y + player.velG.y)/player.vel.x;
            tempY = prevPos.y + slope * ((WINDOW_WIDTH - player.width - 1) - prevPos.x);
            tempX = WINDOW_WIDTH - player.width - 1;
        }
        
        // recalculates position if player y pos is out of bounds
        if (tempY <= 1) {
            slope = player.vel.x/(player.vel.y + player.velG.y);
            tempX = prevPos.x + slope * (1 - prevPos.y);
            tempY = 1;
        } else if (tempY >= WINDOW_HEIGHT - player.height - 1) {
            slope = player.vel.x/(player.vel.y + player.velG.y);
            tempX = prevPos.x + slope * ((WINDOW_HEIGHT - player.height - 1) - prevPos.y);
            tempY = WINDOW_HEIGHT - player.height - 1;
        }
        
        // sets player pos to the calculated pos
        player.pos.x = tempX;
        player.pos.y = tempY;
        
        // end game
        gameOver = true;
    }
}
