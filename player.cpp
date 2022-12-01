#include "player.h"
#include "game.h"
#include "bullet.h"
#include "FEHLCD.h"
#include "image.h"
#include <math.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

Player::Player() {
    width = 12;
    height = 20;

    pos = Vector2D((WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2);
    vel = Vector2D(0, 0);
    force = Vector2D(0, 0);

    velG = Vector2D(0, 0.5);
    forceG = Vector2D(0, 0.5);

    mass = 10;
    ammo = 10;

    playerImage.Open("Player.pic");
}

Vector2D Player::getCenter() {
    return Vector2D(pos.x + width / 2, pos.y + height / 2);
}

bool Player::pointInPlayer(int px, int py) {
    return pos.x <= px && px <= pos.x + width && pos.y <= py && py <= pos.y + height;
}

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

void Player::shoot(float angle) {
    // if player has ammo
    if (ammo > 0) {
        // Reset velocities after shooting
        velG.reset();
        vel.reset();

        // Add a force in the opposite direction of the shot
        Vector2D shotForce(-8.0 * cos(angle), 8.0 * sin(angle));
        force = force.add(shotForce);

        // Append Bullet shooting away from player to game object's bullet vector
        Vector2D center = getCenter();
        game->bullets.push_back(Bullet(center.x, center.y, angle));

        // decrement ammo
        ammo--;

        game->numShots++;
    }
}

void Player::render() {
    playerImage.Draw(pos.x, pos.y);
    LCD.SetFontColor(WHITE);
}
