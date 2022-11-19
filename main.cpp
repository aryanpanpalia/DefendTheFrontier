#include "FEHLCD.h"
#include <iostream>
#include <math.h>

// Defines the window width and height
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

using namespace std;
using namespace FEHIcon;

class Player {
    public:
        int width, height;
        float x, y, velX, velY, accelX, accelY, forceX, forceY, forceG;
        float mass;
        
        Player(){
            width = 20;
            height = 20;

            x = (WINDOW_WIDTH - width) / 2;
            y = (WINDOW_HEIGHT - height) / 2;

            velX = 0;
            velY = 0;
            forceX = 0;
            forceY = 0;

            mass = 10;
        }

        int* getCenter(){
            int* center = new int[2];
            center[0] = x + width / 2;
            center[1] = y + height / 2;
            return center;
        }

        bool pointInPlayer(int px, int py) {
            return x <= px && px <= x + width && y <= py && py <= y + height;
        }

        void applyPhysics(float dt) {
            float dragX = 0;
            float dragY = 0;

            float dragRatio;
            if(velX == 0 || velY == 0){
                dragRatio = 1;
            } else {
                dragRatio = fabs(velX / velY);
            }

            // if dragRatio < 1, |velX| < |velY| so dragX should be scaled down
            // else, dragY should be scaled down
            // Only have a drag force is the velocity is significant
            if(dragRatio < 1){
                if(velX < -0.5 || velX > 0.5) {
                    dragX = velX > 0 ? -30 : 30;
                    dragX *= dragRatio;
                }

                if(velY < -0.5 || velY > 0.5) {
                    dragY = velY > 0 ? -30 : 30;
                }
            } else {                
                if(velX < -0.5 || velX > 0.5) {
                    dragX = velX > 0 ? -30 : 30;
                }

                if(velY < -0.5 || velY > 0.5) {
                    dragY = velY > 0 ? -30 : 30;
                    dragY /= dragRatio;
                }
            }   

            // Snaps velocities to 0 if it comes close enough
            if(-0.5 < velX && velX < 0.5){
                velX = 0;
            }

            if(-0.5 < velY && velY < 0.5){
                velY = 0;
            }  

            // Calculate net forces
            float netForceX = forceX + dragX;
            float netForceY = forceY + dragY;
            
            // Calculate net acceleration
            float netAccelX = netForceX / mass;
            float netAccelY = netForceY / mass;

            // Updates x and y position
            // y is inverted as a positive y force, which moves the player up, reduces the value of y on the screen
            x += velX * dt;
            y -= velY * dt;
            
            // Updates velocity
            velX += netAccelX * dt;
            velY += netAccelY * dt;

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
};

void Play(){
    // Stores whether game loop is running
    bool running = true;

    // Stores whether the screen is being pressed
    bool pressed = false;

    // Location of where the screen has been pressed
    float x, y;

    // Player object
    Player player;

    while(running){
        // Clears the screen then draws the player
        LCD.Clear();
        LCD.DrawRectangle(player.x, player.y, player.width, player.height);

        // On click
        if(LCD.Touch(&x, &y) && !pressed) {
            pressed = true;
            if(!player.pointInPlayer(x, y)){
                int* center = player.getCenter();
                float dx = x - center[0];
                float dy = center[1] - y;
                float angle = atan2(dy, dx);
                
                // Add a force in the opposite direction of the click to the player
                player.forceX += -250 * cos(angle);
                player.forceY += -250 * sin(angle);
            }
        } 
        // On Release
        else if(!LCD.Touch(&x, &y) && pressed) {
            pressed = false;
        }

        // Apply physics
        player.applyPhysics(0.1);

        // Updates the screen
        LCD.Update();
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
