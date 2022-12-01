#include "bullet.h"
#include "player.h"
#include "FEHLCD.h"
#include <math.h>

#define PI 3.1415

Bullet::Bullet(){}

Bullet::Bullet(float initialX, float initialY, float a) {
    pos = Vector2D(initialX, initialY);
    vel = Vector2D(3 * cos(a), -3 * sin(a));
    angle = a;

    char buf[50];
    int angleInDegrees = (180 * angle) / PI;
    int imageAngle = ((int) angleInDegrees / 10) * 10;

    if(imageAngle > 0) {
        snprintf(buf, 50, "bulletImages/Bullet%d.pic", imageAngle);
        bulletImage.Open(buf);
    } else {
        imageAngle += 180;
        snprintf(buf, 50, "bulletImages/Bullet%d.pic", imageAngle);
        bulletImage.Open(buf);
        bulletImage.Rotate180();
    }

    width = bulletImage.cols;
    height = bulletImage.rows; 
}

Vector2D Bullet::getCenter() {
    return pos;
}

bool Bullet::pointInBullet(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

void Bullet::update() {
    // Updates x and y position
    pos = pos.add(vel);
}

void Bullet::render() {
    bulletImage.Draw(pos.x, pos.y);
    LCD.SetFontColor(WHITE);
}

TrackerBullet::TrackerBullet(float initialX, float initialY, Player *p){
    radius = 2;
    mass = 10;

    player = p;

    pos = Vector2D(initialX, initialY);
    vel = player->pos.sub(pos).norm();
}

bool TrackerBullet::pointInBullet(int px, int py) {
    return sqrt(pow(px - pos.x, 2) + pow(py - pos.y, 2)) <= radius;
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
