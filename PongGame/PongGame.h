//
//  PongGame.h
//  PongGame
//
//

#ifndef __PongGame__PongGame__
#define __PongGame__PongGame__

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class PongGame {
public:
    PongGame();
    void run();
    
private:
    sf::RenderWindow mainWindow;
    
    const int mainWindowWidth = 640;
    const int mainWindowHeight = 480;
    sf::Time timePerFrame;

    // rectangles
    sf::RectangleShape topRectangle;
    sf::RectangleShape leftRectangle;
    sf::RectangleShape rightRectangle;
    sf::RectangleShape bottomRectangle;
    sf::Font font;
    sf::Text statisticsText;
    sf::Time statisticsUpdateTime;
    std::size_t statisticsNumberFrames;
    // ball
    sf::RectangleShape ball;

    // ball speed
    sf::Vector2f ballSpeed;
    
private:
    void processEvents();
    void update();
    void render();
    
    bool doRectanglesIntersect(const sf::RectangleShape &rectangle1, sf::RectangleShape &rectangle2);
    void setUpBorderRectangle(sf::RectangleShape &topRectangle);
    void setUpBall(int const mainWindowWidth, int const mainWindowHeight, sf::RectangleShape &ball);
    void updateStatistics(sf::Time elapsedTime);
};

#endif /* defined(__PongGame__PongGame__) */
