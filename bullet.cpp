#include "bullet.h"
#include "FEHLCD.h"
#include <math.h>

Bullet::Bullet(float initialX, float initialY, float a) {
    radius = 2;
    
    pos = Vector2D(initialX, initialY);
    vel = Vector2D(cos(a), -sin(a));
    angle = a;
}

Vector2D Bullet::getCenter() {
    return pos;
}

bool Bullet::pointInBullet(int px, int py) {
    return sqrt(pow(px - pos.x, 2) + pow(py - pos.y, 2)) <= radius;
}

void Bullet::update() {
    // Updates x and y position
    pos = pos.add(vel);
}

void Bullet::render() {
    LCD.FillCircle(pos.x, pos.y, radius);
}
