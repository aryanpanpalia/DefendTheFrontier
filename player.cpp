#include "player.h"
#include "game.h"
#include "bullet.h"
#include "FEHLCD.h"
#include "image.h"
#include <math.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240
#define PI 3.1415

/*
    Creates a player

    Parameters:
        thm: the theme of the game

    Return value: none

    Authors: Aryan Panpalia
*/
Player::Player(int thm) {
    theme = thm;

    // Opens a player image depending on the theme
    if(theme == 0) {
        playerImage.Open("SpacePlayer.pic");
    } else if(theme == 1) {
        playerImage.Open("WesternPlayer.pic");
        flipped = false;
    }
    
    width = playerImage.cols;
    height = playerImage.rows;

    pos = Vector2D((WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2);
    vel = Vector2D(0, 0);
    force = Vector2D(0, 0);

    velG = Vector2D(0, 0.5);
    forceG = Vector2D(0, 0.5);

    mass = 10;
    ammo = 10;
}

/*
    Returns a vector with the position of the center of the object

    Parameters: none
    Return value: Vector2D object containing position of the center of the object

    Authors: Aryan Panpalia
*/
Vector2D Player::getCenter() {
    return Vector2D(pos.x + width / 2, pos.y + height / 2);
}

/*
    Returns a boolean of whether the point is within the rectangular bounding box for the object

    Parameters:
        px: x position
        py: y position

    Return value: boolean of whether the point is within the rectangular bounding box for the object

    Authors: Aryan Panpalia
*/
bool Player::pointInPlayer(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

/*
    Updates the position, velocities, and forces of the player

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Player::update() {
    Vector2D drag(0, 0);

    // If there is a non-zero velocity, calculate a drag force
    if(vel.x || vel.y){
        float velAngle = atan2(vel.y, vel.x);
        float dragAngle = -velAngle;
        drag.set(-cos(dragAngle), sin(dragAngle));
    }

    // Calculate net forces
    Vector2D netForce = force.add(drag);
    
    // Calculate net acceleration
    Vector2D netAccel = netForce.div(mass);
    Vector2D netAccelG = forceG.div(mass);

    // Updates position
    pos = pos.add(vel);
    pos = pos.add(velG);
    
    // Updates velocity
    vel = vel.add(netAccel);
    velG = velG.add(netAccelG);

    // Snaps velocities to 0 if it comes close enough
    if(vel.magnitude() < 0.1) {
        vel.reset();
    }  

    // If adding the drag would change the direciton of the force, set the force to 0
    // Else, add the drag to the force
    if((force.x >= 0 && force.x + drag.x <= 0) || (force.x <= 0 && force.x + drag.x >= 0)){
        force.x = 0;
    } else {
        force.x += drag.x;
    }

    if((force.y >= 0 && force.y + drag.y <= 0) || (force.y <= 0 && force.y + drag.y >= 0)){
        force.y = 0;
    } else {
        force.y += drag.y;
    }
}

/*
    Shoots a bullet from the player at the given angle

    Parameters:
        angle: angle to shoot the bullet at

    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
void Player::shoot(float angle) {
    // if player has ammo
    if (ammo > 0) {
        // Reset velocities after shooting
        velG.reset();
        vel.reset();

        // Add a force in the opposite direction of the shot
        Vector2D shotForce;
        
        // If shooting down, have a larger force in the Y direction and a smaller force in the X direction
        if(angle < 0) {
            shotForce.set(-7.0 * cos(angle), 9.0 * sin(angle));
        } else {
            shotForce.set(-8.0 * cos(angle), 8.0 * sin(angle));
        }

        force = force.add(shotForce);

        // Append Bullet shooting away from player to game object's bullet vector
        Vector2D center = getCenter();
        game->bullets.push_back(Bullet(center.x, center.y, angle, theme));

        // Decrement ammo
        ammo--;

        // Increment numShots
        game->numShots++;

        // Determine orientation of player image if using western theme
        // Flips the player to face the direction of the shot if necessary
        if (game->theme != 0) {
            if (angle >= PI/2 || angle < -PI/2) {
                if (!flipped) {
                    playerImage.Open("WesternPlayerFlipped.pic");
                    flipped = true;
                }
            } else if (flipped) {
                playerImage.Open("WesternPlayer.pic");
                flipped = false;
            }
        }
    }
}

/*
    Draws the object on the screen

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Player::render() {
    playerImage.Draw(pos.x, pos.y);
    LCD.SetFontColor(WHITE);
}
