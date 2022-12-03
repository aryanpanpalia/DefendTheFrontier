#pragma once

#include "vector2d.h"
#include "image.h"

// Forward declaration of Game class
class Game;

/*
    Player Class: 

    Contains variables that stores:
        game: pointer to game object
        width: the width of the object on the screen
        height: the height of the object on the screen
        pos: the position of the object on the screen
        vel: the velocity that the object is moving at
        force: the force that is being exterted on the object
        velG: the gravitational component of velocity
        forceG: the gravitational component of force
        mass: the mass of the bullet
        ammo: amount of ammo the player has available to shoot
        playerImage: the image that the player is rendered as

    Contains methods that:
        Player(): creates a player
        getCenter(): returns the position of the center of the player
        pointInPlayer(int, int): returns whether the coordinate given in the arguments is within the rectangle bounding box for the player
        update(): updates the position, velocities, and forces of the player
        shoot(): shoots a bullet from the player at the given angle
        render(): draws the object on the screen
*/
class Player {
    public:
        Game *game;
        int width, height;
        Vector2D pos, vel, force, velG, forceG;
        float mass;
        int ammo;
        Image playerImage;
        
        Player();
        Vector2D getCenter();
        bool pointInPlayer(int px, int py);
        void update();
        void shoot(float angle);
        void render();
};
