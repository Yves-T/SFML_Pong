//
//  PongGame.cpp
//  PongGame
//

#include "PongGame.h"

PongGame::PongGame()
:mainWindow() {
    mainWindow.create(sf::VideoMode(mainWindowWidth, mainWindowHeight), "Pong");

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

    ballSpeed = sf::Vector2f(0.1, 0.1);
    setUpBall(mainWindowWidth, mainWindowHeight, ball);
}

void PongGame::run() {
    while (mainWindow.isOpen()) {
        processEvents();
        update();
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

    ball.move(ballSpeed.x, ballSpeed.y);
}

void PongGame::render() {
    // Clear screen
    mainWindow.clear(sf::Color::White);
    mainWindow.draw(topRectangle);
    mainWindow.draw(leftRectangle);
    mainWindow.draw(rightRectangle);
    mainWindow.draw(bottomRectangle);
    mainWindow.draw(ball);

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
