#include "enemy.h"
#include "game.h"
#include "FEHLCD.h"
#include <math.h>

Enemy::Enemy(float initialX, float initialY, float a, Game *g) {
    game = g;

    pos = Vector2D(initialX, initialY);
    vel = Vector2D(cos(a), -sin(a));

    enemyImage.Open("Enemy.pic");
}

Vector2D Enemy::getCenter() {
    Vector2D center(pos.x + width / 2, pos.y + height / 2);
    return center;
}

bool Enemy::pointInEnemy(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

void Enemy::update() {
    // Updates x and y position
    pos = pos.add(vel);
}

void Enemy::render() {
    enemyImage.Draw(pos.x, pos.y);
    LCD.SetFontColor(WHITE);
}

void Enemy::shoot() {
    Vector2D center = getCenter();
    game->trackerBullets.push_back(TrackerBullet(center.x, center.y, &game->player));
}
