#include <iostream>
#include <math.h>
#include "FEHLCD.h"
#include "FEHUtility.h"
#include "game.h"

// Defines the window width and height
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

using namespace FEHIcon;

// Stores game statistics
int highScore1 = 0, highScore2 = 0, highScore3 = 0, totalEnemiesKilled = 0, totalShots = 0, totalDeaths = 0;

/*
    Prompts user to select the difficulty at which to play the game

    Parameters: none
    Return value: the difficulty (0: easy, 1: medium, 2: hard, 3: extreme)

    Authors: Aryan Panpalia
*/
int SelectDifficulty() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int backButtonWidth = 60;
    int backButtonHeight = 25;

    // Initial XY position of back button    
    int buttonStartX = 7;
    int buttonStartY = WINDOW_HEIGHT - backButtonHeight - 10;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, backButtonWidth, backButtonHeight, RED, WHITE);

    // Create buttons
    Icon easyButton, mediumButton, hardButton, extremeButton;
    char easyString[] = "Easy";
    char mediumString[] = "Medium";
    char hardString[] = "Hard";
    char extremeString[] = "Extreme";

    // Setting button width, height, and buffer between them as constants
    int buttonWidth = 100;
    int buttonHeight = 100;
    int buffer = 10;

    // Initial XY positions of the buttons   
    int easyStartX = 20 + (WINDOW_WIDTH - (buttonWidth * 2 + buffer)) / 2;
    int easyStartY = (WINDOW_HEIGHT - (buttonHeight * 2 + buffer)) / 2;

    int mediumStartX = easyStartX + buttonWidth + buffer;
    int mediumStartY = easyStartY;

    int hardStartX = easyStartX;
    int hardStartY = easyStartY + buttonHeight + buffer;

    int extremeStartX = easyStartX + buttonWidth + buffer;
    int extremeStartY = easyStartY + buttonHeight + buffer;

    easyButton.SetProperties(easyString, easyStartX, easyStartY, buttonWidth, buttonHeight, GREEN, WHITE);
    mediumButton.SetProperties(mediumString, mediumStartX, mediumStartY, buttonWidth, buttonHeight, BLUE, WHITE);
    hardButton.SetProperties(hardString, hardStartX, hardStartY, buttonWidth, buttonHeight, ORANGE, WHITE);
    extremeButton.SetProperties(extremeString, extremeStartX, extremeStartY, buttonWidth, buttonHeight, RED, WHITE);

    float x, y;
    while(true) {
        // Clears the screen then draws all the buttons
        LCD.Clear();

        LCD.SetFontColor(GREEN);
        LCD.FillRectangle(easyStartX, easyStartY, buttonWidth, buttonHeight);
        easyButton.Draw();

        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(mediumStartX, mediumStartY, buttonWidth, buttonHeight);
        mediumButton.Draw();

        LCD.SetFontColor(ORANGE);
        LCD.FillRectangle(hardStartX, hardStartY, buttonWidth, buttonHeight);
        hardButton.Draw();

        LCD.SetFontColor(RED);
        LCD.FillRectangle(extremeStartX, extremeStartY, buttonWidth, buttonHeight);
        extremeButton.Draw();

        LCD.SetFontColor(RED);
        LCD.FillRectangle(buttonStartX, buttonStartY, backButtonWidth, backButtonHeight);
        backButton.Draw();

        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // If the player clicked on a certain button, return the difficulty clicked on
        if(easyStartX < x && x < easyStartX + buttonWidth && easyStartY < y && y < easyStartY + buttonHeight) {
            return 0;
        } else if(mediumStartX < x && x < mediumStartX + buttonWidth && mediumStartY < y && y < mediumStartY + buttonHeight) {
            return 1;
        } else if(hardStartX < x && x < hardStartX + buttonWidth && hardStartY < y && y < hardStartY + buttonHeight) {
            return 2;
        } else if(extremeStartX < x && x < extremeStartX + buttonWidth && extremeStartY < y && y < extremeStartY + buttonHeight) {
            return 3;
        } else if(x >= buttonStartX && x <= buttonStartX + backButtonWidth && y >= buttonStartY && y <= buttonStartY + backButtonHeight){
            return -1;
        }
    }
}

/*
    Prompts user to select the theme of the game

    Parameters: none
    Return value: the theme (0: space, 1: western)
*/
int SelectTheme() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int backButtonWidth = 60;
    int backButtonHeight = 25;

    // Initial XY position of back button    
    int buttonStartX = 7;
    int buttonStartY = WINDOW_HEIGHT - backButtonHeight - 10;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, backButtonWidth, backButtonHeight, RED, WHITE);

    // Create buttons
    Icon spaceButton, westernButton;
    char spaceString[] = "Space";
    char westernString[] = "Western";

    // Setting button width, height, and buffer between them as constants
    int buttonWidth = 100;
    int buttonHeight = 100;
    int buffer = 10;

    // Initial XY positions of the buttons   
    int spaceStartX = (WINDOW_WIDTH - (buttonWidth * 2 + buffer)) / 2;
    int spaceStartY = (WINDOW_HEIGHT - (buttonHeight + buffer)) / 2;

    int westernStartX = spaceStartX + buttonWidth + buffer;
    int westernStartY = spaceStartY;

    spaceButton.SetProperties(spaceString, spaceStartX, spaceStartY, buttonWidth, buttonHeight, BLUEVIOLET, WHITE);
    westernButton.SetProperties(westernString, westernStartX, westernStartY, buttonWidth, buttonHeight, ORANGE, WHITE);

    float x, y;
    while(true) {
        // Clears the screen then draws all the buttons
        LCD.Clear();

        LCD.WriteAt("SELECT YOUR FRONTIER", 40, 20);

        LCD.SetFontColor(BLUEVIOLET);
        LCD.FillRectangle(spaceStartX, spaceStartY, buttonWidth, buttonHeight);
        spaceButton.Draw();

        LCD.SetFontColor(ORANGE);
        LCD.FillRectangle(westernStartX, westernStartY, buttonWidth, buttonHeight);
        westernButton.Draw();

        LCD.SetFontColor(RED);
        LCD.FillRectangle(buttonStartX, buttonStartY, backButtonWidth, backButtonHeight);
        backButton.Draw();

        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Returns the theme selected depending on what was clicked or -1 if the back button was clicked
        if(spaceStartX < x && x < spaceStartX + buttonWidth && spaceStartY < y && y < spaceStartY + buttonHeight) {
            return 0;
        } else if(westernStartX < x && x < westernStartX + buttonWidth && westernStartY < y && y < westernStartY + buttonHeight) {
            return 1;
        } else if(x >= buttonStartX && x <= buttonStartX + backButtonWidth && y >= buttonStartY && y <= buttonStartY + backButtonHeight){
            return -1;
        }
    }
}

/*
    Sets up and runs the game

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
void Play(){
    // Prompt for theme
    SELECTTHEME:;
    int theme = SelectTheme();
    if (theme == -1) return;

    // Prompt for difficulty level
    int difficulty = SelectDifficulty();
    if (difficulty == -1) goto SELECTTHEME;

    // Stores whether the screen is being pressed
    bool pressed = false;

    // Location of where the screen has been pressed
    float x, y;

    // Game object
    Game game(difficulty, theme);

    // Alias/Reference to game's player object
    Player &player = game.player;

    // Render game
    game.render();

    // Keep running the game loop if the game has not ended
    while(!game.gameOver){
        // On click
        if(LCD.Touch(&x, &y) && !pressed) {
            pressed = true;

            // Checks if the player clicked outside the player object
            if(!player.pointInPlayer(x, y)){
                // Finds the angle at which to shoot at then shoots
                Vector2D center = player.getCenter();
                float dx = x - center.x;
                float dy = center.y - y;
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

        // Handle player out of bounds
        game.playerOutOfBounds();

        // Handle collisions
        game.handleCollisions();

        // Render game
        game.render();
    }

    // Close game's background image
    game.backgroundImage.Close();

    // Close player image
    player.playerImage.Close();
    
    // Close all the bullet images
    for(int i = 0; i < game.bullets.size(); i++) {
        for(int j = 0; j < 4; j++) {
            game.bullets[i].bulletImages[j].Close();
        }
    }

    // Close all the enemy images
    for(int i = 0; i < game.enemies.size(); i++) {
        game.enemies[i].enemyImage.Close();
    }

    // Close all the tracker bullet images
    for(int i = 0; i < game.trackerBullets.size(); i++) {
        for(int j = 0; j < 2; j++) {
            game.trackerBullets[i].trackerBulletImages[j].Close();
        }
    }

    // Adjusts statistics based on previous game's results
    if (game.score > highScore1) {
        highScore3 = highScore2;
        highScore2 = highScore1;
        highScore1 = game.score;
    } else if (game.score > highScore2) {
        highScore3 = highScore2;
        highScore2 = game.score;
    } else if (game.score > highScore3) {
        highScore3 = game.score;
    }

    totalEnemiesKilled += game.numEnemiesKilled;
    totalShots += game.numShots;
    totalDeaths += 1;

    // Waits a second before exiting the game
    Sleep(1.0);
}

/*
    Displays the statistics

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
void Statistics() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int buttonWidth = 100;
    int buttonHeight = 40;

    // Initial XY position of back button    
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonStartY = 175;

    // Calculates the accuracy (0 if no shots have been fired)
    float accuracy = totalShots == 0 ? 0 : (float) totalEnemiesKilled/totalShots;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, buttonWidth, buttonHeight, RED, WHITE);
    
    LCD.Clear();

    // Writes the statistics to the screen
    LCD.WriteAt("High Score 1:", 10, 10);       LCD.WriteAt(highScore1, WINDOW_WIDTH-100, 10);
    LCD.WriteAt("High Score 2:", 10, 30);       LCD.WriteAt(highScore2, WINDOW_WIDTH-100, 30);
    LCD.WriteAt("High Score 3:", 10, 50);       LCD.WriteAt(highScore3, WINDOW_WIDTH-100, 50);
    LCD.WriteAt("# Enemies Killed:", 10, 70);   LCD.WriteAt(totalEnemiesKilled, WINDOW_WIDTH-100, 70);
    LCD.WriteAt("# Shots fired:", 10, 90);      LCD.WriteAt(totalShots, WINDOW_WIDTH-100, 90);
    LCD.WriteAt("# Deaths:", 10, 110);          LCD.WriteAt(totalDeaths, WINDOW_WIDTH-100, 110);
    LCD.WriteAt("% Accuracy:", 10, 130);        LCD.WriteAt(accuracy, WINDOW_WIDTH-100, 130);
    LCD.WriteAt("", 10, 150);

    // Draws the back button to the screen
    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, buttonStartY, buttonWidth, buttonHeight);
    backButton.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Goes back to the main menu if the back button is clicked
        if(x >= buttonStartX && x <= buttonStartX + buttonWidth && y >= buttonStartY && y <= buttonStartY + buttonHeight){
            return;
        }
    }
}

/*
    Displays the instructions on how to play the game

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
void Instructions() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int buttonWidth = 100;
    int buttonHeight = 40;

    // Initial XY position of back button    
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonStartY = 175;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, buttonWidth, buttonHeight, RED, WHITE);
    
    LCD.Clear();

    // Writes the instructions to the screen
    LCD.WriteAt("To play, click anywhere", 10, 10);
    LCD.WriteAt("outside the player. Shoot", 10, 30);
    LCD.WriteAt("at enemies to kill them", 10, 50);
    LCD.WriteAt("and get ammo while moving", 10, 70);
    LCD.WriteAt("yourself in the opposite", 10, 90);
    LCD.WriteAt("direction. Don't run into", 10, 110);
    LCD.WriteAt("an enemy or off the", 10, 130);
    LCD.WriteAt("screen or you'll lose!", 10, 150);

    // Draws the back button to the screen
    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, buttonStartY, buttonWidth, buttonHeight);
    backButton.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Goes back to the main menu if the back button is clicked
        if(x >= buttonStartX && x <= buttonStartX + buttonWidth && y >= buttonStartY && y <= buttonStartY + buttonHeight){
            return;
        }
    }
}

/*
    Displays the credits

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia and Thomas Banko
*/
void Credits() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int buttonWidth = 100;
    int buttonHeight = 40;

    // Initial XY position of back button    
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonStartY = 175;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, buttonWidth, buttonHeight, RED, WHITE);
    
    LCD.Clear();

    // Writes the credits to the screen
    LCD.WriteAt("Frontier Defense", 10, 10);
    LCD.WriteAt("Created By:", 10, 30);
    LCD.WriteAt("Aryan Panpalia", 10, 50);
    LCD.WriteAt("and", 10, 70);
    LCD.WriteAt("Thomas Banko", 10, 90);
    LCD.WriteAt("Using:", 10, 110);
    LCD.WriteAt("Proteus Simulator", 10, 130);
    LCD.WriteAt("Libraries from OSU FEH", 10, 150);

    // Draws the back button
    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, buttonStartY, buttonWidth, buttonHeight);
    backButton.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Goes back to the main menu if the back button is clicked
        if(x >= buttonStartX && x <= buttonStartX + buttonWidth && y >= buttonStartY && y <= buttonStartY + buttonHeight){
            return;
        }
    }
}

/*
    Displays the main menu

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Menu() {
    // Icon for title
    Icon gameTitle;
    char titleString[] = "Frontier Defense";

    // Icons for all the menu options
    Icon playButton;
    char playString[] = "Play Game";
    
    Icon viewStatistics;
    char statsString[] = "View Statistics";

    Icon viewInstructions;
    char instructionsString[] = "View Instructions";

    Icon viewCredits;
    char creditsString[] = "View Credits";

    // Setting variables with title properties
    int titleWidth = 270, titleHeight = 40;
    int titleStartX = (WINDOW_WIDTH - titleWidth) / 2, titleStartY = 7;

    // Setting button width and height as constants
    int buttonWidth = 220;
    int buttonHeight = 35;

    // All buttons start at the same X position
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;

    // Initial Y positions for all the buttons
    int playStartY = 55, statsStartY = 100, instructionsStartY = 145, creditsStartY = 190;

    // Sets properties for title
    gameTitle.SetProperties(titleString, titleStartX, titleStartY, titleWidth, titleHeight, 10052885, BROWN);

    // Sets properties of all the buttons
    playButton.SetProperties(playString, buttonStartX, playStartY, buttonWidth, buttonHeight, GREEN, WHITE);
    viewStatistics.SetProperties(statsString, buttonStartX, statsStartY, buttonWidth, buttonHeight, BLUE, WHITE);
    viewInstructions.SetProperties(instructionsString, buttonStartX, instructionsStartY, buttonWidth, buttonHeight, WHITE, BLACK);
    viewCredits.SetProperties(creditsString, buttonStartX, creditsStartY, buttonWidth, buttonHeight, RED, WHITE);

    float x, y;
    while(true){
        // Clears the screen then draws all the buttons
        LCD.Clear();
        LCD.SetFontColor(16631342);
        LCD.FillRectangle(titleStartX, titleStartY, titleWidth, titleHeight);
        gameTitle.Draw();

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

        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Depending on where the touch was at the end, do the following actions
        if(x >= buttonStartX && x <= buttonStartX + buttonWidth){
            if(y >= playStartY && y <= playStartY + buttonHeight){
                Play();
            } else if(y >= statsStartY && y <= statsStartY + buttonHeight){
                Statistics();
            } else if(y >= instructionsStartY && y <= instructionsStartY + buttonHeight){
                Instructions();
            } else if(y >= creditsStartY && y <= creditsStartY + buttonHeight){
                Credits();
            }
        }
    }
}

/* 
    Entry point to the application

    Parameters: none
    Return value: 0

    Authors: Aryan Panpalia
*/
int main() {
    // Draws the menu
    Menu();

    return 0;
}