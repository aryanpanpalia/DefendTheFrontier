#include <iostream>
#include <math.h>
#include "FEHLCD.h"
#include "main.h"

// Defines the window width and height
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

using namespace std;
using namespace FEHIcon;

Player::Player() {
    width = 20;
    height = 20;

    x = (WINDOW_WIDTH - width) / 2;
    y = (WINDOW_HEIGHT - height) / 2;

    velX = 0;
    velY = 0;
    velG = -0.5;
    forceX = 0;
    forceY = 0;
    forceG = -0.5;

    mass = 10;
}

int* Player::getCenter() {
    int* center = new int[2];
    center[0] = x + width / 2;
    center[1] = y + height / 2;
    return center;
}

bool Player::pointInPlayer(int px, int py) {
    return x <= px && px <= x + width && y <= py && py <= y + height;
}

void Player::applyPhysics() {
    float dragX = 0;
    float dragY = 0;

    // If there is a non-zero velocity, calculate a drag force
    if(velX || velY){
        float velAngle = atan2(velY, velX);
        float dragAngle = -velAngle;
        dragX = -cos(dragAngle);
        dragY = sin(dragAngle);
    }

    // Calculate net forces
    float netForceX = forceX + dragX;
    float netForceY = forceY + dragY;
    
    // Calculate net acceleration
    float netAccelX = netForceX / mass;
    float netAccelY = netForceY / mass;
    float netAccelG = forceG / mass;

    // Updates x and y position
    // y is inverted as a positive y force, which moves the player up, reduces the value of y on the screen
    x += velX;
    y -= velY;
    y -= velG;
    
    // Updates velocity
    velX += netAccelX;
    velY += netAccelY;
    velG += netAccelG;

    // Snaps velocities to 0 if it comes close enough
    float vMagnitude = sqrt(pow(velX, 2) + pow(velY, 2));
    if(vMagnitude < 0.1) {
        velX = 0;
        velY = 0;
    }  

    // If adding the drag would change the direciton of the force, set the force to 0
    // Else, add the drag to the force
    if((forceX >= 0 && forceX + dragX <= 0) || (forceX <= 0 && forceX + dragX >= 0)){
        forceX = 0;
    } else {
        forceX += dragX;
    }

    if((forceY >= 0 && forceY + dragY <= 0) || (forceY <= 0 && forceY + dragY >= 0)){
        forceY = 0;
    } else {
        forceY += dragY;
    }
}

void Player::shoot(float angle) {
    // Reset velocities after shooting
    velG = 0;
    velX = 0;
    velY = 0;

    // Add a force in the opposite direction of the shot
    forceX += -8.0 * cos(angle);
    forceY += -8.0 * sin(angle);

    int *center = getCenter();
    game->bullets[game->numBullets++] = Bullet(center[0], center[1], angle);
}

void Player::render() {
    LCD.FillRectangle(x, y, width, height);
}

Bullet::Bullet(float initialX, float initialY, float a) {
    radius = 2;
    active=true;
    
    x = initialX;
    y = initialY;
    angle = a;
    
    velX = cos(a);
    velY = sin(a);
}

int* Bullet::getCenter() {
    int* center = new int[2];
    center[0] = x;
    center[1] = y;
    return center;
}

bool Bullet::pointInBullet(int px, int py) {
    return sqrt(pow(px - x, 2) + pow(py - y, 2)) <= radius;
}

void Bullet::applyPhysics() {
    // Updates x and y position
    // y is inverted as a positive y force, which moves the player up, reduces the value of y on the screen
    x += velX;
    y -= velY;
}

void Bullet::render() {
    LCD.FillCircle(x, y, 2);
}

Game::Game() {
    player.game = this;
    numBullets = 0;
    bullets = (Bullet *) malloc(1000 * sizeof(Bullet));
}

Game::~Game() {
    free(bullets);
}

void Game::render() {
    LCD.Clear();
    player.render();

    for(int i = 0; i < numBullets; i++) {
        if(bullets[i].active){
            bullets[i].render();
        }
    }

    LCD.Update();
}

void Game::update() {
    player.applyPhysics();

    for(int i = 0; i < numBullets; i++) {
        // Create alias to bullets[i]
        Bullet &bullet = bullets[i];

        if(bullet.active) {
            int bRadius = bullet.radius;
            
            // Apply the physics to the bullet
            bullet.applyPhysics();

            // If the bullet touches the edge, make it inactive
            if(bullet.x <= bRadius || bullet.x >= WINDOW_WIDTH - bRadius || bullet.y <= bRadius || bullet.y >= WINDOW_HEIGHT - bRadius) {
                bullet.active = false;
            }
        }
    }
}

bool Game::hasEnded() {
    return player.x <= 0 || player.x >= WINDOW_WIDTH - player.width || player.y <= 0 || player.y >= WINDOW_HEIGHT - player.height;
}

void Play(){
    // Stores whether the screen is being pressed
    bool pressed = false;

    // Location of where the screen has been pressed
    float x, y;

    // Game object
    Game game;

    // Alias/Reference to game's player object
    Player &player = game.player;

    // Keep running the game loop if the game has not ended
    while(!game.hasEnded()){
        // Render game
        game.render();

        // On click
        if(LCD.Touch(&x, &y) && !pressed) {
            pressed = true;
            if(!player.pointInPlayer(x, y)){
                int* center = player.getCenter();
                float dx = x - center[0];
                float dy = center[1] - y;
                float angle = atan2(dy, dx);
                
                player.shoot(angle);
            }
        } 
        // On Release
        else if(!LCD.Touch(&x, &y) && pressed) {
            pressed = false;
        }

        // Update game
        game.update();
    }
}

int Menu() {
    // Icons for all the menu options
    Icon playButton;
    char playString[] = "Play Game";
    
    Icon viewStatistics;
    char statsString[] = "View Statistics";

    Icon viewInstructions;
    char instructionsString[] = "View Instructions";

    Icon viewCredits;
    char creditsString[] = "View Credits";

    // Setting button width and height as constants
    int buttonWidth = 220;
    int buttonHeight = 40;
    
    // All buttons start at the same X position
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;

    // Initial Y positions for all the buttons
    int playStartY = 25, statsStartY = 75, instructionsStartY = 125, creditsStartY = 175;

    // Sets properties of all the buttons
    playButton.SetProperties(playString, buttonStartX, playStartY, buttonWidth, buttonHeight, GREEN, WHITE);
    viewStatistics.SetProperties(statsString, buttonStartX, statsStartY, buttonWidth, buttonHeight, BLUE, WHITE);
    viewInstructions.SetProperties(instructionsString, buttonStartX, instructionsStartY, buttonWidth, buttonHeight, WHITE, BLACK);
    viewCredits.SetProperties(creditsString, buttonStartX, creditsStartY, buttonWidth, buttonHeight, RED, WHITE);

    // Clears the screen then draws all the buttons
    LCD.Clear();

    LCD.SetFontColor(GREEN);
    LCD.FillRectangle(buttonStartX, playStartY, buttonWidth, buttonHeight);
    playButton.Draw();

    LCD.SetFontColor(BLUE);
    LCD.FillRectangle(buttonStartX, statsStartY, buttonWidth, buttonHeight);
    viewStatistics.Draw();

    LCD.SetFontColor(WHITE);
    LCD.FillRectangle(buttonStartX, instructionsStartY, buttonWidth, buttonHeight);
    viewInstructions.Draw();

    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, creditsStartY, buttonWidth, buttonHeight);
    viewCredits.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Depending on where the touch was at the end, do the following actions
        if(x >= buttonStartX && x <= buttonStartX + buttonWidth){
            if(y >= playStartY && y <= playStartY + buttonHeight){
                cout << "Play\n";
                return 0;
            } else if(y >= statsStartY && y <= statsStartY + buttonHeight){
                cout << "Stats\n";
                return 1;
            } else if(y >= instructionsStartY && y <= instructionsStartY + buttonHeight){
                cout << "Instructions\n";
                return 2;
            } else if(y >= creditsStartY && y <= creditsStartY + buttonHeight){
                cout << "Credits\n";
                return 3;
            }
        }
    }
}

/* Entry point to the application */
int main() {
    // Draws the menu
    int selection = Menu();

    if(selection == 0){
        Play();
    }

    return 0;
}