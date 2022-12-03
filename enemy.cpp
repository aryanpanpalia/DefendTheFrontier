#include "enemy.h"
#include "game.h"
#include "FEHLCD.h"
#include <math.h>

/*
    Creates an Enemy object

    Parameters:
        initialX: initial x position
        initialY: initial y position
        a: angle the enemy is moving at
        g: pointer to game object

    Return value: none

    Authors: Aryan Panpalia
*/
Enemy::Enemy(float initialX, float initialY, float a, Game *g) {
    game = g;

    pos = Vector2D(initialX, initialY);
    vel = Vector2D(cos(a), -sin(a));

    enemyImage.Open("Enemy.pic");
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
    game->trackerBullets.push_back(TrackerBullet(center.x, center.y, &game->player));
}
