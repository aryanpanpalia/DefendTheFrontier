#include "enemy.h"
#include "game.h"
#include "FEHLCD.h"
#include <math.h>

#define PI 3.1415

/*
    Creates an Enemy object

    Parameters:
        initialX: initial x position
        initialY: initial y position
        a: angle the enemy is moving at
        g: pointer to game object

    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
Enemy::Enemy(float initialX, float initialY, float a, Game *g) {
    game = g;

    pos = Vector2D(initialX, initialY);
    vel = Vector2D(cos(a), -sin(a));

    if(game->theme == 0) {
        enemyImage.Open("SpaceEnemy.pic");
    } else if(game->theme == 1) {
        char fname[50];
        enemyType = (rand() % 2) + 1;

        if(a >= PI / 2 || a < -PI / 2) {
            snprintf(fname, 50, "WesternEnemy_%dFlipped.pic", enemyType);
            enemyImage.Open(fname);
            flipped = true;
        } else {
            snprintf(fname, 50, "WesternEnemy_%d.pic", enemyType);
            enemyImage.Open(fname);
            flipped = false;  
        }
    }
    
    width = enemyImage.cols;
    height = enemyImage.rows;
}

/*
    Returns a vector with the position of the center of the object

    Parameters: none
    Return value: Vector2D object containing position of the center of the object

    Authors: Aryan Panpalia
*/
Vector2D Enemy::getCenter() {
    Vector2D center(pos.x + width / 2, pos.y + height / 2);
    return center;
}

/*
    Returns a boolean of whether the point is within the rectangular bounding box for the object

    Parameters:
        px: x position
        py: y position

    Return value: boolean of whether the point is within the rectangular bounding box for the object

    Authors: Aryan Panpalia
*/
bool Enemy::pointInEnemy(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

/*
    Updates the position, velocities, and forces of the bullet

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Enemy::update() {
    // Updates x and y position
    pos = pos.add(vel);
}

/*
    Draws the object on the screen

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Enemy::render() {
    enemyImage.Draw(pos.x, pos.y);
    LCD.SetFontColor(WHITE);
}

/*
    Shoots a tracker bullet from the enemy

    Parameters: none
    Return value: none

    Authors: Thomas Banko
*/
void Enemy::shoot() {
    Vector2D center = getCenter();
    game->trackerBullets.push_back(TrackerBullet(center.x, center.y, &game->player, game->theme));

    // Determine orientation of enemy if using western theme
    if (game->theme == 1) {
        Vector2D direction = game->player.pos.sub(center);
        float angle = atan2(direction.y, direction.x);

        if(angle >= PI/2 || angle < -PI/2) {
            if (!flipped) {
                char fname[50];
                snprintf(fname, 50, "WesternEnemy_%dFlipped.pic", enemyType);
                enemyImage.Open(fname);
                flipped = true;
            }
        } else if(flipped) {
            char fname[50];
            snprintf(fname, 50, "WesternEnemy_%d.pic", enemyType);
            enemyImage.Open(fname);
            flipped = false;
        }
    }    
}
