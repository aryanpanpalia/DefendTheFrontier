#pragma once

#include <vector2d.h>
#include "image.h"

// Forward declaration of Game class
class Game;

/*
    Enemy Class: 

    Contains variables that stores:
        game: pointer to game object
        width: the width of the object on the screen
        height: the height of the object on the screen
        pos: the position of the object on the screen
        vel: the velocity that the object is moving at
        force: the force that is being exterted on the object
        mass: the mass of the enemy
        enemyImage: the image that the enemy is rendered as
        enemyType: for western theme, which enemy image to render
        flipped: whether the image is flipped horizontally

    Contains methods that:
        Enemy(float, float, float): creates a enemy given the arguments
        getCenter(): returns the position of the center of the enemy
        pointInEnemy(int, int): returns whether the coordinate given in the arguments is within the rectangle bounding box for the enemy
        update(): updates the position, velocities, and forces of the enemy
        render(): draws the object on the screen
        shoot(): shoots a tracker bullet from the enemy

        Authors: Aryan Panpalia
*/
class Enemy {
    public:
        Game *game;
        int width = 15, height = 15;
        Vector2D pos, vel, force;
        float mass;
        Image enemyImage;
        int enemyType;
        bool flipped;

        Enemy(float initialX, float initialY, float angle, Game *game);
        Vector2D getCenter();
        bool pointInEnemy(int px, int py);
        void update();
        void render();
        void shoot(); 
};
