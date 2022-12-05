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
    
    // Goes through the bulletImages array
    for(int i = 0; i < 4; i++) {
        // String to store the image filename
        char fname[50];

        // If theme = 0 (space theme) load the space bullet images
        // Else if theme = 1 (western theme) load the western bullet images
        if(theme == 0) {
            // Stores the image filename in fname
            snprintf(fname, 50, "ElectroBall_%d.pic", i + 1);
            bulletImages[i].Open(fname);
        } else if(theme == 1) {
            // Calculates the approximate angle in degrees, as there are only unique images for angles 0 to 180 increasing by 10
            int angleInDegrees = (180 * angle) / PI;
            int imageAngle = ((int) angleInDegrees / 10) * 10;

            if(imageAngle > 0) {
                snprintf(fname, 50, "bulletImages/Bullet%d.pic", imageAngle);
                bulletImages[i].Open(fname);
            } else {
                // Opens up the image for an angle 180 degrees greater then rotates it 180 degrees
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
    // Every 10 frames, change which image is displayed for the bullet
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
        // Stores the filename of the image to open
        char fname[50];

        // Loads in a filename to fname depending on the theme
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
    Vector2D force = player->getCenter().sub(getCenter()).norm().mult(1.5);
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
    // Changes the image displayed for the tracker bullet every 10 frames
    if(frameCount % 10 == 0) {
        imageIndex++;
        imageIndex %= 2;
        frameCount = 0;
    }

    trackerBulletImages[imageIndex].Draw(pos.x, pos.y);

    frameCount++;
    LCD.SetFontColor(WHITE);
}
