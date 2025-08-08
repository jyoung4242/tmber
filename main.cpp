#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <array>
#include <memory>
#include "util.h"



void updateBranches(int seed);

const int NUM_BRANCHES = 6;
enum class side {LEFT,RIGHT,NONE};
side branchPositions[NUM_BRANCHES];



int main()
{
    std::array<std::optional<sf::Sprite>, NUM_BRANCHES> branches;
    //Game Constants
    const unsigned int SCREEN_WIDTH = 1920;
    const unsigned int SCREEN_HEIGHT = 1080;

    const float TREE_HORIZONTAL_POSITION = 810.0f;
    const float TREE_VERTICAL_POSITION = 0.f;
    const float BEE_STARTING_X = 0.0f;
    const float BEE_STARTING_Y = 800.0f;
    const float CLOUD1_STARTING_X = 0.0f;
    const float CLOUD1_STARTING_Y = 0.0f;
    const float CLOUD2_STARTING_X = 0.0f;
    const float CLOUD2_STARTING_Y = 250.0f;
    const float CLOUD3_STARTING_X = 0.0f;
    const float CLOUD3_STARTING_Y = 500.0f;
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;


    // Rendering call
    sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "TIMBER!!!!");

    // Setting up UI font
    sf::Font myFont;
    if (!myFont.openFromFile("fonts/KOMIKAP_.ttf")) {
        return 1;
    }
    sf::Text myStartingText(myFont, "Press Enter To Start!");
    myStartingText.setCharacterSize(75);
    myStartingText.setFillColor(sf::Color::White);
    centerTextBox(myStartingText);
    
    sf::Text myScoreText(myFont, "Score: 0");
    myScoreText.setCharacterSize(25);
    myScoreText.setFillColor(sf::Color::White);
    myScoreText.setPosition(sf::Vector2f(25, 25));


    // Setting up Timer Bar
    sf::RectangleShape timerBar;
    float timerBarWidth = 400;
    float timerBarHeight = 80;
    timerBar.setSize(sf::Vector2f(timerBarWidth, timerBarHeight));
    timerBar.setFillColor(sf::Color::Red);
    timerBar.setPosition(sf::Vector2f((1920 / 2) - timerBarWidth / 2, 980));

    // Setting up Background Sprite
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> maybeBackgroundSprite;
    if (maybeBackgroundSprite = loadSpriteFromPath(std::string("graphics/background.png"), sf::Vector2f(0.f, 0.f), backgroundTexture); !maybeBackgroundSprite) return 1;
    sf::Sprite spriteBackground = *maybeBackgroundSprite;

    // Setting up Tree Sprite
    sf::Texture tree1Texture;
    std::optional<sf::Sprite> maybeTree1Sprite;
    if (maybeTree1Sprite = loadSpriteFromPath(std::string("graphics/tree.png"), sf::Vector2f(TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION), tree1Texture); !maybeTree1Sprite) return 1;
    sf::Sprite spriteTree1 = *maybeTree1Sprite;

    // Setting up the Bee Sprite
    sf::Texture beeTexture;
    std::optional<sf::Sprite> maybeBeeSprite;
    if (maybeBeeSprite = loadSpriteFromPath(std::string("graphics/bee.png"), sf::Vector2f(BEE_STARTING_X, BEE_STARTING_Y), beeTexture); !maybeBeeSprite) return 1;
    sf::Sprite spriteBee = *maybeBeeSprite;

    // Configuring Bee parameters
    bool isBeeActive = false;
    float beeSpeed = 0.0f;

    // Setting up the Cloud sprites (with 1 Texture)
    sf::Texture cloudTexture;
    std::optional<sf::Sprite> maybeCloudSprite1;
    std::optional<sf::Sprite> maybeCloudSprite2;
    std::optional<sf::Sprite> maybeCloudSprite3;
    if (maybeCloudSprite1 = loadSpriteFromPath(std::string("graphics/cloud.png"), sf::Vector2f(CLOUD1_STARTING_X, CLOUD1_STARTING_Y), cloudTexture); !maybeCloudSprite1) return 1;
    sf::Sprite spriteCloud1 = *maybeCloudSprite1;
    if (maybeCloudSprite2 = loadSpriteFromPath(std::string("graphics/cloud.png"), sf::Vector2f(CLOUD2_STARTING_X, CLOUD2_STARTING_Y), cloudTexture); !maybeCloudSprite2) return 1;
    sf::Sprite spriteCloud2 = *maybeCloudSprite2;
    if (maybeCloudSprite3 = loadSpriteFromPath(std::string("graphics/cloud.png"), sf::Vector2f(CLOUD3_STARTING_X, CLOUD3_STARTING_Y), cloudTexture); !maybeCloudSprite3) return 1;
    sf::Sprite spriteCloud3 = *maybeCloudSprite3;

    // Setting up cloud speeds
    bool isCloud1Active = false;
    bool isCloud2Active = false;
    bool isCloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Configure Branch Sprites and data structure

    sf::Texture textureBranch;
    for (int i = 0; i < NUM_BRANCHES; i++) {
        if (branches[i] = loadSpriteFromPath(std::string("graphics/branch.png"), sf::Vector2f(-2000.f, -2000.f), textureBranch); !branches[i]) return 1;
    }

    // Player Sprite
    sf::Texture texturePlayer;
    std::optional<sf::Sprite> maybePlayerSprite;
    if (maybePlayerSprite = loadSpriteFromPath(std::string("graphics/player.png"), sf::Vector2f(580.f, 720.f), texturePlayer); !maybePlayerSprite) return 1;
    sf::Sprite spritePlayer = *maybePlayerSprite;

    // Gravestone Sprite
    sf::Texture textureGrave;
    std::optional<sf::Sprite> maybeGraveSprite;
    if (maybeGraveSprite = loadSpriteFromPath(std::string("graphics/rip.png"), sf::Vector2f(600, 860), textureGrave); !maybeGraveSprite) return 1;
    sf::Sprite spriteGrave = *maybeGraveSprite;

    // Axe Sprite
    sf::Texture textureAxe;
    std::optional<sf::Sprite> maybeAxeSprite;
    if (maybeAxeSprite = loadSpriteFromPath(std::string("graphics/axe.png"), sf::Vector2f(700, 830), textureAxe); !maybeAxeSprite) return 1;
    sf::Sprite spriteAxe = *maybeAxeSprite;

    // Log Sprite
    sf::Texture textureLog;
    std::optional<sf::Sprite> maybeLogSprite;
    if (maybeLogSprite = loadSpriteFromPath(std::string("graphics/log.png"), sf::Vector2f(810, 780), textureLog); !maybeLogSprite) return 1;
    sf::Sprite spriteLog = *maybeLogSprite;
    
    // **************************
    // game sounds
    // **************************

    //sf::SoundBuffer chopBuffer("sound/chop.wav");
    sf::SoundBuffer OOTBuffer("sound/out_of_time.wav");
    //sf::SoundBuffer deathBuffer("sound/death.wav");
        
    sf::Sound soundOOT(OOTBuffer);
    //soundOOT.play();
    //std::unique_ptr<sf::Sound> soundOOT = std::make_unique<sf::Sound>(OOTBuffer);
    //sf::Sound soundChop(chopBuffer);
    //std::unique_ptr<sf::Sound> soundChop = std::make_unique<sf::Sound>(chopBuffer);
    //sf::Sound soundDeath(deathBuffer);
	//std::unique_ptr<sf::Sound> soundDeath = std::make_unique<sf::Sound>(deathBuffer);


    // **************************
    // game state
    // **************************
    bool isPaused = true;
    unsigned int score = 0;
    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timerBarWidth / timeRemaining;
    sf::Clock clock;
    side playerSide = side::LEFT;
    bool isLogActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    bool isKeyPressEnabled = false;

    // Make all the branches disappear
    for (int i = 1; i < NUM_BRANCHES; i++)
    {
        branchPositions[i] = side::NONE;
    }


    // **********************
    // Game Loop
    // **********************
    while (window.isOpen())
    {

        // manages the clicking of the 'x' for the window 
        // manages the key release event too
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            // 'key release logic
            if (event->is<sf::Event::KeyReleased>() && !isPaused) {
                isKeyPressEnabled = true;
                spriteAxe.setPosition(sf::Vector2f(2000, spriteAxe.getPosition().y));
            }
        }

        // Get Keyboard events
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            // Reset the time and the score
            score = 0;
            timeRemaining = 6.0f;
            isPaused = false;

            // Make all the branches disappear
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteGrave.setPosition(sf::Vector2f(675, 2000));
            // Move the player into position
            spritePlayer.setPosition(sf::Vector2f(580, 720));
            isKeyPressEnabled = true;
        }
        
        if (isKeyPressEnabled) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                playerSide = side::RIGHT;
                score++;
                timeRemaining += (2 / score) + 0.15;
                spriteAxe.setPosition(sf::Vector2f(AXE_POSITION_RIGHT, spriteAxe.getPosition().y));
                spritePlayer.setPosition(sf::Vector2f(1200, 720));
                updateBranches(score);
                spriteLog.setPosition(sf::Vector2f(810,720));
                logSpeedX = -5000;
                isLogActive = true;
                isKeyPressEnabled = false;
                //soundChop.play();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2 / score) + 0.15;
                spriteAxe.setPosition(sf::Vector2f(AXE_POSITION_LEFT, spriteAxe.getPosition().y));
                spritePlayer.setPosition(sf::Vector2f(580, 720));
                updateBranches(score);
                spriteLog.setPosition(sf::Vector2f(810, 720));
                logSpeedX = 5000;
                isLogActive = true;
                isKeyPressEnabled = false;
                //soundChop.play();
            }

        }

        // Manage game time
        sf::Time dt = clock.restart();
        if (!isPaused) {
            timeRemaining -= dt.asSeconds();
        }

        // Update Time bar drawing
        timerBar.setSize(sf::Vector2f((timeBarWidthPerSecond * timeRemaining), timerBarHeight));
       
        if (timeRemaining <= 0.0f) {
            // Pause the game
            isPaused = true;
            myStartingText.setString("OUT OF TIME!");
            centerTextBox(myStartingText);
            //soundOOT.play();
        }

        // Move Sprites around
        if (!isPaused) {

            // Moving the Bee
            if (!isBeeActive) {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(sf::Vector2f(2000., height));
                isBeeActive = true;
            }
            else {
                spriteBee.setPosition(sf::Vector2f(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y));
                if (spriteBee.getPosition().x < -100)
                {
                    isBeeActive = false;
                }
            }

            // Moving the clouds
            if (!isCloud1Active) {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(sf::Vector2f(-200, height));
                isCloud1Active = true;
            }
            else {
                spriteCloud1.setPosition(sf::Vector2f(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y));
                if (spriteCloud1.getPosition().x > 1920)
                {
                    isCloud1Active = false;
                }
            }

            if (!isCloud2Active) {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(sf::Vector2f(-200, height));
                isCloud2Active = true;
            }
            else {
                spriteCloud2.setPosition(sf::Vector2f(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y));
                if (spriteCloud2.getPosition().x > 1920)
                {
                    isCloud2Active = false;
                }
            }

            if (!isCloud3Active) {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(sf::Vector2f(-200, height));
                isCloud3Active = true;
            }
            else {
                spriteCloud3.setPosition(sf::Vector2f(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y));
                if (spriteCloud3.getPosition().x > 1920)
                {
                    isCloud3Active = false;
                }
            }

        }
        
        // Update the score text
        std::stringstream ss;
        ss << "Score: " << score;
        myScoreText.setString(ss.str());

        // Manage Branch Heights
        for (int i = 0; i < NUM_BRANCHES;i++) {
            float height = i * 150;
            if (!branches[i].has_value()) continue; // Prevent access violation
            sf::Sprite& branch = *branches[i];
            if (branchPositions[i] == side::LEFT) {
                branch.setPosition(sf::Vector2f(810, height));
                branch.setRotation(sf::degrees(180));
            }
            else if (branchPositions[i] == side::RIGHT) {
                branch.setPosition(sf::Vector2f(1100, height));
                branch.setRotation(sf::degrees(0));
            }
            else {
                branch.setPosition(sf::Vector2f(3000, height));
            }
        }

        // Manage Log movement
        if (isLogActive) {
            spriteLog.setPosition(sf::Vector2f(spriteLog.getPosition().x+(logSpeedX * dt.asSeconds()),spriteLog.getPosition().y +(logSpeedY * dt.asSeconds())));
            // Has the log reached the right hand edge?
            if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
            {
                // Set it up ready to be a whole new log next frame
                isLogActive= false;
                spriteLog.setPosition(sf::Vector2f(810, 780));
            }
        }

        // Check for branch collision with player
        if (branchPositions[5] == playerSide) {
            isPaused = true;
            isKeyPressEnabled = false;
            spriteGrave.setPosition(sf::Vector2f(525, 760));
            spritePlayer.setPosition(sf::Vector2f(2000, 660));
            spriteAxe.setPosition(sf::Vector2f(2000, 660));
            myStartingText.setString("SQUISHED");
            centerTextBox(myStartingText);
            //soundDeath.play();

        }


        // Draw calls
        window.clear();
        window.draw(spriteBackground);
        
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            if (branches[i].has_value()) {
                sf::Sprite& branch = *branches[i];
                window.draw(branch);
            }
        }
        window.draw(spriteTree1);

        window.draw(spritePlayer);
        window.draw(spriteLog);
        window.draw(spriteGrave);
        window.draw(spriteAxe);

        window.draw(spriteBee);
        window.draw(myScoreText);
        window.draw(timerBar);

        if (isPaused) {
            window.draw(myStartingText);
        }

        window.display();
    }

    return 0;
}

// Function definition
void updateBranches(int seed)
{
    
    // Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {

        branchPositions[j] = branchPositions[j - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }

}