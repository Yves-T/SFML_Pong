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
    mainWindow.setFramerateLimit(60);
    mainWindow.setVerticalSyncEnabled(true);

    timePerFrame = sf::seconds(1.f / 60.f);

    topRectangle.setPosition(0, 0);
    topRectangle.setSize(sf::Vector2f(mainWindowWidth, borderSize));

    leftRectangle.setPosition(-borderSize, 0);
    leftRectangle.setSize(sf::Vector2f(borderSize, mainWindowHeight));

    rightRectangle.setPosition(mainWindowWidth, 0);
    rightRectangle.setSize(sf::Vector2f(borderSize, mainWindowHeight));

    bottomRectangle.setPosition(0, mainWindowHeight - borderSize);
    bottomRectangle.setSize(sf::Vector2f(mainWindowWidth, borderSize));

    setUpBorderRectangle(topRectangle);
    setUpBorderRectangle(leftRectangle);
    setUpBorderRectangle(rightRectangle);
    setUpBorderRectangle(bottomRectangle);

    ballSpeed = sf::Vector2f(100, 100);
    setUpBall(mainWindowWidth, mainWindowHeight, ball);

    setUpPlayer(player1);
    player1.setPosition(margin - borderSize, mainWindowHeight / 2 - 25);

    setUpPlayer(player2);
    player2.setPosition(mainWindowWidth - margin, mainWindowHeight / 2 - 25);

    setUpMiddleLine();

    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        // handle error here
    }

    statisticsText.setFont(font);
    statisticsText.setPosition(5.f, 5.f);
    statisticsText.setCharacterSize(10);
    statisticsText.setColor(sf::Color::Black);

    setupTitle();
    setUpStartText();
    setupWonText();
    setupLostText();
    setupScore();
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
        } else if ((event.type == sf::Event::KeyPressed) && (gameState == INTRO)) {
            gameState = PLAYING;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player1Speed = sf::Vector2f(0, -moveDistance / player1NavigationSpeed);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player1Speed = sf::Vector2f(0, moveDistance / player1NavigationSpeed);
        }
    }
}

void PongGame::update() {

    if(gameState != PLAYING) {
        return;
    }

    // auto move player 2 pad
    if (ball.getPosition().y < player2.getPosition().y) {
        player2Speed = sf::Vector2f(0, -moveDistance / 0.07);
        player2.move(player2Speed * timePerFrame.asSeconds());
    } else if (ball.getPosition().y + ball.getSize().y > player2.getPosition().y + player2.getSize().y) {
        player2Speed = sf::Vector2f(0, moveDistance / 0.07);
        player2.move(player2Speed * timePerFrame.asSeconds());
    }

    // move player 1 pad
    player1.move(player1Speed * timePerFrame.asSeconds());

    // block players pad inside the play area
    if (doRectanglesIntersect(player1, bottomRectangle) || doRectanglesIntersect(player1, topRectangle)) {
        updatePositionOfPlayer(player1);
    }

    if (doRectanglesIntersect(player2, bottomRectangle) || doRectanglesIntersect(player2, topRectangle)) {
        updatePositionOfPlayer(player2);
    }

    // ball collides with top and bottom
    if (doRectanglesIntersect(ball, topRectangle)) {
        handleBallHitsTopOrBottom(topRectangle);
    }

    if (doRectanglesIntersect(ball, bottomRectangle)) {
        handleBallHitsTopOrBottom(bottomRectangle);
    }

    // ball collides with player1 and player2
    if (doRectanglesIntersect(ball, player1)) {
        handleBallCollidesWithPlayer(player1);
    }

    if (doRectanglesIntersect(ball, player2)) {
        handleBallCollidesWithPlayer(player2);
    }

    // check for scoring
    if (doRectanglesIntersect(ball, leftRectangle)) {
        handleScoring(leftRectangle);
    }

    if (doRectanglesIntersect(ball, rightRectangle)) {
        handleScoring(rightRectangle);
    }

    // detect if game is over
    determineGameState();

    ball.move(ballSpeed * timePerFrame.asSeconds());
}

void PongGame::determineGameState() {
    if (player1Score >= 11 && player1Score >= player2Score + 2) {
        gameState = PLAYER1_WON;
    }

    if (player2Score >= 11 && player2Score >= player1Score + 2) {
        gameState = PLAYER1_LOST;
    }
}

void PongGame::handleScoring(sf::RectangleShape &border) {
    if (&border == &rightRectangle) {
        player1Score++;
    } else {
        player2Score++;
    }
    std::__1::stringstream stringStream;
    stringStream << player1Score << "    " << player2Score;
    score.setString(stringStream.str());
    score.setPosition(mainWindowWidth / 2 - score.getGlobalBounds().width / 2, 40);
    sf::FloatRect ballBounds = ball.getGlobalBounds();
    if (&border == &rightRectangle) {
        sf::FloatRect playerBounds = player1.getGlobalBounds();
        ball.setPosition(playerBounds.left + ballBounds.width + 5, mainWindowHeight / 2);
        ballSpeed.x = 100.1;
        ballSpeed.y = 100.1;
    } else {
        sf::FloatRect playerBounds = player2.getGlobalBounds();
        ball.setPosition(playerBounds.left - ballBounds.width - 5, mainWindowHeight / 2);
        ballSpeed.x = -1 * 100.1;
        ballSpeed.y = 100.1;
    }
}

void PongGame::handleBallCollidesWithPlayer(sf::RectangleShape &player) {
    sf::FloatRect playerBounds = player.getGlobalBounds();
    sf::FloatRect ballBounds = ball.getGlobalBounds();
    ballSpeed.x = -ballSpeed.x;
    ballSpeed.y = (ballBounds.top + ballBounds.height / 2 - playerBounds.top - playerBounds.height / 2) / 1;
    float correction = 0;
    if (&player2 == &player) {
        correction = ballBounds.left + ballBounds.width - playerBounds.left;
        ballBounds.left = playerBounds.left - ballBounds.width - correction;
    } else if (&player1 == &player) {
        correction = playerBounds.left + playerBounds.width - ballBounds.left;
        ballBounds.left = playerBounds.left + playerBounds.width + correction;
    }

    ball.setPosition(ballBounds.left, ballBounds.top);
    increaseBallSpeed();
}

void PongGame::increaseBallSpeed() {
    // increase ball speed by 1%
    ballSpeed.x = ballSpeed.x * 1.01;
    ballSpeed.y = ballSpeed.y * 1.01;
}

void PongGame::handleBallHitsTopOrBottom(sf::RectangleShape &border) {
    sf::FloatRect rectangleBounds = border.getGlobalBounds();
    sf::FloatRect ballRectangleBounds = ball.getGlobalBounds();
    ballSpeed.y = -ballSpeed.y;

    float ballYPosition = 0;
    if (&bottomRectangle == &border) {
        // ball hits bottom rectangle
        ballYPosition = rectangleBounds.top - rectangleBounds.height - ballRectangleBounds.top;
    } else if (&topRectangle == &border) {
        // ball hits top rectangle
        ballYPosition = rectangleBounds.top + rectangleBounds.height - ballRectangleBounds.top;
    }
    ball.move(sf::Vector2f(0, ballSpeed.y + 2 * ballYPosition) * timePerFrame.asSeconds());
}

void PongGame::updatePositionOfPlayer(sf::RectangleShape &player) {
    sf::FloatRect topRectangleGlobalBounds = topRectangle.getGlobalBounds();
    sf::FloatRect bottomRectangleGlobalBounds = bottomRectangle.getGlobalBounds();
    sf::Vector2f playerPosition = player.getPosition();
    float topConstraint = topRectangleGlobalBounds.top + topRectangleGlobalBounds.height + 5;
    float bottomConstraint = bottomRectangleGlobalBounds.top - player.getSize().y - 5;
    playerPosition.y = getYCoordinate((int const) playerPosition.y, (int const) topConstraint,
            (int const) bottomConstraint);
    player.setPosition(playerPosition);
}

void PongGame::render() {
    // Clear screen
    mainWindow.clear(sf::Color::White);

    switch (gameState) {
        case INTRO:
            mainWindow.draw(title);
            mainWindow.draw(startText);
            break;
        case PLAYING:
            mainWindow.draw(middleLine);
            mainWindow.draw(leftRectangle);
            mainWindow.draw(rightRectangle);
            mainWindow.draw(player1);
            mainWindow.draw(player2);
            mainWindow.draw(ball);
            mainWindow.draw(score);
            mainWindow.draw(topRectangle);
            mainWindow.draw(bottomRectangle);
            break;
        case PLAYER1_WON:
            mainWindow.draw(wonText);
            break;
        case PLAYER1_LOST:
            mainWindow.draw(lostText);
            break;
        default:
            break;
    }

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
    ball.setOutlineThickness(2);
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

void PongGame::setUpPlayer(sf::RectangleShape &player) {
    player.setSize(sf::Vector2f(borderSize, 90));
    player.setFillColor(sf::Color(0, 122, 245));
    player.setOutlineColor(sf::Color::Red);
    player.setOutlineThickness(3);
}

void PongGame::setUpMiddleLine() {
    middleLine.setFillColor(sf::Color(100, 100, 100, 30));
    middleLine.setOutlineColor(sf::Color(0, 0, 100, 30));
    middleLine.setOutlineThickness(2);
    middleLine.setPosition(mainWindowWidth / 2, 0);
    middleLine.setSize(sf::Vector2f(0, mainWindowHeight));
}

void PongGame::setupTitle() {
    title = sf::Text("Pong SFML 2", font, 50);
    title.setPosition(mainWindowWidth / 2 - title.getGlobalBounds().width / 2, 100);
    title.setColor(sf::Color::Blue);
}

void PongGame::setUpStartText() {
    startText = sf::Text("Press any key to startText", font, 30);
    startText.setPosition(mainWindowWidth / 2 - startText.getGlobalBounds().width / 2, 400);
    startText.setColor(sf::Color::Red);
}

void PongGame::setupWonText() {
    wonText = sf::Text("You have won this game.\n\n Congratulations !", font, 20);
    float xCoordinate = mainWindowWidth / 2 - wonText.getGlobalBounds().width / 2;
    float yCoordinate = mainWindowHeight / 2 - lostText.getGlobalBounds().height / 2;
    wonText.setPosition(xCoordinate, yCoordinate);
}

void PongGame::setupLostText() {
    lostText = sf::Text("You have lostText this game, \n better luck next time!", font, 20);
    lostText.setPosition(mainWindowWidth / 2 - score.getGlobalBounds().width / 2, 40);
    lostText.setColor(sf::Color::Red);
}

void PongGame::setupScore() {
    score = sf::Text("0   0", font, 50);
    score.setPosition(mainWindowWidth / 2 - score.getGlobalBounds().width / 2, 40);
    score.setColor(sf::Color(0, 0, 100, 50));
}

int PongGame::getYCoordinate(const int playerPosition, const int topConstraint, const int bottomConstraint) {
    return std::min(std::max(topConstraint, playerPosition), bottomConstraint);;
}
