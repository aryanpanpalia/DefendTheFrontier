#include "bullet.h"
#include "player.h"
#include "FEHLCD.h"
#include <math.h>

Bullet::Bullet(){}

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

TrackerBullet::TrackerBullet(float initialX, float initialY, Player *p){
    radius = 2;
    mass = 10;

    player = p;

    pos = Vector2D(initialX, initialY);
    vel = player->pos.sub(pos).norm();
}

void TrackerBullet::update() {
    Vector2D force = player->pos.sub(pos).norm();
    vel = vel.add(force.div(mass));
    pos = pos.add(vel);
}

void TrackerBullet::render() {
    LCD.SetFontColor(ORANGERED);
    LCD.DrawCircle(pos.x, pos.y, radius);
    LCD.SetFontColor(WHITE);
}
