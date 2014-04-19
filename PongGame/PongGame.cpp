//
//  PongGame.cpp
//  PongGame
//

#include "PongGame.h"
#include "ResourcePath.hpp"
#import "StringHelper.h"

PongGame::PongGame()
:mainWindow() {
    mainWindow.create(sf::VideoMode(mainWindowWidth, mainWindowHeight), "Pong");

    timePerFrame = sf::seconds(1.f / 60.f);

    const int borderSize = 30;

    topRectangle.setPosition(borderSize, 0);
    topRectangle.setSize(sf::Vector2f(mainWindowWidth - 2 * borderSize, borderSize));

    leftRectangle.setPosition(0, 0);
    leftRectangle.setSize(sf::Vector2f(borderSize, mainWindowHeight));

    rightRectangle.setPosition(mainWindowWidth - borderSize, 0);
    rightRectangle.setSize(sf::Vector2f(borderSize, mainWindowHeight));

    bottomRectangle.setPosition(borderSize, mainWindowHeight - borderSize);
    bottomRectangle.setSize(sf::Vector2f(mainWindowWidth - 2 * borderSize, borderSize));

    setUpBorderRectangle(topRectangle);
    setUpBorderRectangle(leftRectangle);
    setUpBorderRectangle(rightRectangle);
    setUpBorderRectangle(bottomRectangle);

    ballSpeed = sf::Vector2f(100, 100);
    setUpBall(mainWindowWidth, mainWindowHeight, ball);

    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        // handle error here
    }

    statisticsText.setFont(font);
    statisticsText.setPosition(5.f, 5.f);
    statisticsText.setCharacterSize(10);
    statisticsText.setColor(sf::Color::Black);
}

void PongGame::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mainWindow.isOpen()) {
        processEvents();
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            // no matter what happens, give the same  delta time to the update function
            update();
        }
        updateStatistics(elapsedTime);
        render();
    }
}

void PongGame::processEvents() {
    sf::Event event;
    while (mainWindow.pollEvent(event)) {
        // Close window : exit
        if (event.type == sf::Event::Closed) {
            mainWindow.close();
        }

        // Escape pressed : exit
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            mainWindow.close();
        }
    }
}

void PongGame::update() {
    if (doRectanglesIntersect(ball, topRectangle) || doRectanglesIntersect(ball, bottomRectangle)) {
        ballSpeed.y = -ballSpeed.y;
    }

    if (doRectanglesIntersect(ball, leftRectangle) || doRectanglesIntersect(ball, rightRectangle)) {
        ballSpeed.x = -ballSpeed.x;
    }

    ball.move(ballSpeed * timePerFrame.asSeconds());
}

void PongGame::render() {
    // Clear screen
    mainWindow.clear(sf::Color::White);
    mainWindow.draw(topRectangle);
    mainWindow.draw(leftRectangle);
    mainWindow.draw(rightRectangle);
    mainWindow.draw(bottomRectangle);
    mainWindow.draw(ball);
    mainWindow.draw(statisticsText);


    // Update the window
    mainWindow.display();
}

bool PongGame::doRectanglesIntersect(const sf::RectangleShape &rectangle1, sf::RectangleShape &rectangle2) {
    sf::FloatRect globalBoundsOfRectangle1 = rectangle1.getGlobalBounds();
    sf::FloatRect globalBoundsOfRectangle2 = rectangle2.getGlobalBounds();
    return globalBoundsOfRectangle1.intersects(globalBoundsOfRectangle2);
}

void PongGame::setUpBorderRectangle(sf::RectangleShape &topRectangle) {
    const int outlineThickness = 3;
    topRectangle.setFillColor(sf::Color(100, 100, 100));
    topRectangle.setOutlineColor(sf::Color::Blue);
    topRectangle.setOutlineThickness(outlineThickness);
}

void PongGame::setUpBall(int const mainWindowWidth, int const mainWindowHeight, sf::RectangleShape &ball) {
    ball.setPosition(mainWindowWidth / 2, mainWindowHeight / 2);
    ball.setSize(sf::Vector2f(20, 20));
    ball.setFillColor(sf::Color::Red);
    ball.setOutlineColor(sf::Color::Yellow);
    ball.setOutlineThickness(1);
}

void PongGame::updateStatistics(sf::Time elapsedTime) {
    statisticsUpdateTime += elapsedTime;
    statisticsNumberFrames += 1;

    if (statisticsUpdateTime >= sf::seconds(1.0f)) {
        statisticsText.setString(
                "Frames / Second = " + toString(statisticsNumberFrames) + "\n" +
                        "Time / Update = " + toString(statisticsUpdateTime.asMicroseconds() / statisticsNumberFrames) + "us");

        statisticsUpdateTime -= sf::seconds(1.0f);
        statisticsNumberFrames = 0;
    }
}
