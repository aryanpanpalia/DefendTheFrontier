#include "bullet.h"
#include "player.h"
#include "FEHLCD.h"
#include <math.h>

#define PI 3.1415

/*
    No argument constructor for the Bullet class

    Authors: Thomas Banko
*/
Bullet::Bullet(){}

/*
    Creates a Bullet object

    Parameters:
        initialX: initial x position for the bullet
        initialY: initial y position for the bullet
        a: angle for the bullet to move at
        theme: the game theme (0: space, 1: western)

    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/

Bullet::Bullet(float initialX, float initialY, float a, int theme) {
    pos = Vector2D(initialX, initialY);
    vel = Vector2D(3 * cos(a), -3 * sin(a));
    angle = a;
    frameCount = 0;
    
    for(int i = 0; i < 4; i++) {
        char fname[50];
        if(theme == 0) {
            snprintf(fname, 50, "ElectroBall_%d.pic", i + 1);
            bulletImages[i].Open(fname);
        } else if(theme == 1) {
            int angleInDegrees = (180 * angle) / PI;
            int imageAngle = ((int) angleInDegrees / 10) * 10;

            if(imageAngle > 0) {
                snprintf(fname, 50, "bulletImages/Bullet%d.pic", imageAngle);
                bulletImages[i].Open(fname);
            } else {
                imageAngle += 180;
                snprintf(fname, 50, "bulletImages/Bullet%d.pic", imageAngle);
                bulletImages[i].Open(fname);
                bulletImages[i].Rotate180();
            }
        }
    }

    width = bulletImages[0].cols;
    height = bulletImages[0].rows;

    imageIndex = rand() % 4;
}

/*
    Returns a vector with the position of the center of the object

    Parameters: none
    Return value: Vector2D object containing position of the center of the object

    Authors: Aryan Panpalia
*/
Vector2D Bullet::getCenter() {
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
bool Bullet::pointInBullet(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

/*
    Updates the position, velocities, and forces of the bullet

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Bullet::update() {
    // Updates x and y position
    pos = pos.add(vel);
}

/*
    Draws the object on the screen

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
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

/*
    Creates a tracker bullet object

    Parameters:
        initialX: initial x position
        initialY: initial y position
        p: pointer to player object

    Authors: Aryan Panpalia and Thomas Banko
*/
TrackerBullet::TrackerBullet(float initialX, float initialY, Player *p, int theme){
    pos = Vector2D(initialX, initialY);
    mass = 10;
    frameCount = 0;
    player = p;

    vel = player->pos.sub(pos).norm();

    for(int i = 0; i < 2; i++) {
        char fname[50];
        if(theme == 0) {
            snprintf(fname, 50, "SpaceEnemyBullet_%d.pic", i + 1);
        } else if (theme == 1) { 
            strcpy(fname, "WesternEnemyPellet.pic");
        }
        
        trackerBulletImages[i].Open(fname);
    }

    width = trackerBulletImages[0].cols;
    height = trackerBulletImages[0].rows;

    imageIndex = 0;

}

/*
    Updates the position, velocities, and forces of the tracker bullet

    Parameters: none
    Return value: none

    Authors: Thomas Banko
*/
void TrackerBullet::update() {
    Vector2D force = player->pos.sub(pos).norm();
    vel = vel.add(force.div(mass));
    pos = pos.add(vel);
}

/*
    Draws the object on the screen

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
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
