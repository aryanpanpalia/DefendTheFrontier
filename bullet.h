#pragma once

#include <vector2d.h>
#include "image.h"

/*
    Bullet Class: 

    Contains variables that stores:
        width: the width of the object on the screen
        height: the height of the object on the screen
        pos: the position of the object on the screen
        vel: the velocity that the object is moving at
        force: the force that is being exterted on the object
        mass: the mass of the bullet
        angle: the angle the bullet is moving at
        bulletImages: the different images that the bullet is rendered as
        frameCount: the number of frames the current images has been rendered for the object
        imageIndex: the index of the bullet image being rendered

    Contains methods that:
        Bullet(): creates a bullet
        Bullet(float, float, float): creates a bullet given the arguments
        getCenter(): returns the position of the center of the bullet
        pointInBullet(int, int): returns whether the coordinate given in the arguments is within the rectangle bounding box for the bullet
        update(): updates the position, velocities, and forces of the bullet
        render(): draws the object on the screen

    Authors: Aryan Panpalia and Thomas Banko
*/
class Bullet {
    public:
        int width, height;
        Vector2D pos, vel, force;
        float mass;
        float angle;
        Image bulletImages[4];
        int frameCount, imageIndex;

        Bullet();
        Bullet(float initialX, float initialY, float angle);
        Vector2D getCenter();
        bool pointInBullet(int px, int py);
        void update();
        void render();
};

// Forward declaration of Player class
class Player;

/*
    TrackerBullet Class: 

    Contains variables that stores:
        player: pointer to the player object
        trackerBulletImages: the different images that the bullet is rendered as

    Contains methods that:
        TrackerBullet(float, float, Player*): creates a tracker bullet given the arguments
        update(): updates the position, velocities, and forces of the tracker bullet
        render(): draws the object on the screen

    Authors: Aryan Panpalia and Thomas Banko
*/
class TrackerBullet: public Bullet {
    public:
        Player *player;
        Image trackerBulletImages[2];

        TrackerBullet(float initialX, float initialY, Player *player);
        void update();
        void render();
};
