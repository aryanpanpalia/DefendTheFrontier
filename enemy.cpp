#include "enemy.h"
#include "FEHLCD.h"
#include <math.h>

Enemy::Enemy(float initialX, float initialY, float a) {
    width = 15;
    height = 15;
    onScreen = false;

    pos = Vector2D(initialX, initialY);
    vel = Vector2D(cos(a), -sin(a));
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
    LCD.SetFontColor(RED);
    LCD.FillRectangle(pos.x, pos.y, width, height);
    LCD.SetFontColor(WHITE);
}
