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
    frameCount = 0;
    
    for(int i = 0; i < 4; i++) {
        char fname[50];
        snprintf(fname, 50, "ElectroBall_%d.pic", i + 1);
        bulletImages[i].Open(fname);
    }

    width = bulletImages[0].cols;
    height = bulletImages[0].rows;

    for (int i = 0; i < 4; i++) {
        imageIndex = rand() % 4;
    }
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
    if(frameCount % 10 == 0) {
        imageIndex++;
        imageIndex %= 4;
        frameCount = 0;
    }

    bulletImages[imageIndex].Draw(pos.x, pos.y);

    frameCount++;
    LCD.SetFontColor(WHITE);
}

TrackerBullet::TrackerBullet(float initialX, float initialY, Player *p){
    pos = Vector2D(initialX, initialY);
    mass = 10;
    frameCount = 0;
    player = p;

    vel = player->pos.sub(pos).norm();

    for(int i = 0; i < 2; i++) {
        char fname[50];
        snprintf(fname, 50, "EnemyBullet_%d.pic", i + 1);
        trackerBulletImages[i].Open(fname);
    }

    width = trackerBulletImages[0].cols;
    height = trackerBulletImages[0].rows;

    imageIndex = 0;
}

bool TrackerBullet::pointInBullet(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

void TrackerBullet::update() {
    Vector2D force = player->pos.sub(pos).norm();
    vel = vel.add(force.div(mass));
    pos = pos.add(vel);
}

void TrackerBullet::render() {
    if(frameCount % 10 == 0) {
        imageIndex++;
        imageIndex %= 2;
        frameCount = 0;
    }

    trackerBulletImages[imageIndex].Draw(pos.x, pos.y);

    frameCount++;
    LCD.SetFontColor(WHITE);
}
