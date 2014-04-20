//
//  PongGame.h
//  PongGame
//
//

#ifndef __PongGame__PongGame__
#define __PongGame__PongGame__

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

static const float player1NavigationSpeed = 0.05;

class PongGame {
public:
    PongGame();

    void run();

private:
    sf::RenderWindow mainWindow;

    const int mainWindowWidth = 640;
    const int mainWindowHeight = 480;
    const int borderSize = 12;
    const int margin = 50;
    const int moveDistance = 5;

    sf::Time timePerFrame;

    // rectangles
    sf::RectangleShape topRectangle;
    sf::RectangleShape leftRectangle;
    sf::RectangleShape rightRectangle;
    sf::RectangleShape bottomRectangle;

    sf::RectangleShape player1;
    sf::RectangleShape player2;

    sf::RectangleShape middleLine;

    sf::Font font;
    sf::Text statisticsText;
    sf::Time statisticsUpdateTime;
    std::size_t statisticsNumberFrames;

    sf::Text title;
    sf::Text startText;
    sf::Text wonText;
    sf::Text lostText;
    sf::Text score;

    unsigned int player1Score = 0;
    unsigned int player2Score = 0;

    enum states {
        INTRO, PLAYING, PLAYER1_WON, PLAYER1_LOST
    };

    int gameState = INTRO;

    // ball
    sf::RectangleShape ball;

    // ball speed
    sf::Vector2f ballSpeed;

    // player speed
    sf::Vector2f player1Speed;
    sf::Vector2f player2Speed;

private:
    void processEvents();

    void update();

    void render();

    bool doRectanglesIntersect(const sf::RectangleShape &rectangle1, sf::RectangleShape &rectangle2);

    void setUpBorderRectangle(sf::RectangleShape &topRectangle);

    void setUpBall(int const mainWindowWidth, int const mainWindowHeight, sf::RectangleShape &ball);

    void setUpPlayer(sf::RectangleShape &player);

    void setUpMiddleLine();

    void updateStatistics(sf::Time elapsedTime);

    void setupTitle();

    void setUpStartText();

    void setupWonText();

    void setupLostText();

    void setupScore();

    int getYCoordinate(const int playerPosition, const int topConstraint, const int bottomConstraint);

    void updatePositionOfPlayer(sf::RectangleShape &player);

    void handleBallHitsTopOrBottom(sf::RectangleShape &player);

    void increaseBallSpeed();

    void handleBallCollidesWithPlayer(sf::RectangleShape &player);

    void handleScoring(sf::RectangleShape &border);

    void determineGameState();
};

#endif /* defined(__PongGame__PongGame__) */
