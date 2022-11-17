#include "FEHLCD.h"
#include <iostream>

// Defines the window width and height
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

using namespace std;
using namespace FEHIcon;

void Play(){
    bool running = true;
    bool pressed = false;
    float x, y;
    while(running){
        LCD.Clear();

        int playerWidth = 20, playerHeight = 20;
        int playerX = (WINDOW_WIDTH - playerWidth) / 2;
        int playerY = (WINDOW_HEIGHT - playerHeight) / 2;

        LCD.DrawRectangle(playerX, playerY, playerWidth, playerHeight);

        // On click
        if(LCD.Touch(&x, &y) && !pressed) {
            pressed = true;
            cout << "Clicked at " << x << ", " << y << endl;
        } 
        // On hold
        else if(LCD.Touch(&x, &y) && pressed) {
            cout << "Held at " << x << ", " << y << endl;
        }
        // On Release
        else if(!LCD.Touch(&x, &y) && pressed) {
            pressed = false;
            cout << "Released at " << x << ", " << y << endl;
        }

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
