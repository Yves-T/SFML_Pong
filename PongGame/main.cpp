#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

bool doRectanglesIntersect(const sf::RectangleShape &rectangle1, sf::RectangleShape &rectangle2);

void setUpBorderRectangle(sf::RectangleShape &topRectangle);

void setUpBall(int const mainWindowWidth, int const mainWindowHeight, sf::RectangleShape &ball);

int main(int, char const **) {
    using namespace sf;

    const int mainWindowWidth = 640;
    const int mainWindowHeight = 480;

    // Create the main window
    RenderWindow window(VideoMode(mainWindowWidth, mainWindowHeight), "Pong");

    // Set the Icon
    Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Create a graphical text to display
    Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }

    // setup rectangles
    RectangleShape topRectangle;
    RectangleShape leftRectangle;
    RectangleShape rightRectangle;
    RectangleShape bottomRectangle;

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

    // setup ball
    RectangleShape ball;
    setUpBall(mainWindowWidth, mainWindowHeight, ball);

    Vector2f ballSpeed(0.1, 0.1);

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        Event event;
        while (window.pollEvent(event)) {
            // Close window : exit
            if (event.type == Event::Closed) {
                window.close();
            }

            // Escape pressed : exit
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }

        if (doRectanglesIntersect(ball, topRectangle) || doRectanglesIntersect(ball, bottomRectangle)) {
            ballSpeed.y = -ballSpeed.y;
        }

        if (doRectanglesIntersect(ball, leftRectangle) || doRectanglesIntersect(ball, rightRectangle)) {
            ballSpeed.x = -ballSpeed.x;
        }

        ball.move(ballSpeed.x, ballSpeed.y);

        // Clear screen
        window.clear(Color::White);
        window.draw(topRectangle);
        window.draw(leftRectangle);
        window.draw(rightRectangle);
        window.draw(bottomRectangle);
        window.draw(ball);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}

// test if two given rectangles intersect
bool doRectanglesIntersect(const sf::RectangleShape &rectangle1, sf::RectangleShape &rectangle2) {
    sf::FloatRect globalBoundsOfRectangle1 = rectangle1.getGlobalBounds();
    sf::FloatRect globalBoundsOfRectangle2 = rectangle2.getGlobalBounds();
    return globalBoundsOfRectangle1.intersects(globalBoundsOfRectangle2);
}

void setUpBorderRectangle(sf::RectangleShape &topRectangle) {
    const int outlineThickness = 3;
    topRectangle.setFillColor(sf::Color(100, 100, 100));
    topRectangle.setOutlineColor(sf::Color::Blue);
    topRectangle.setOutlineThickness(outlineThickness);
}

void setUpBall(int const mainWindowWidth, int const mainWindowHeight, sf::RectangleShape &ball) {
    ball.setPosition(mainWindowWidth / 2, mainWindowHeight / 2);
    ball.setSize(sf::Vector2f(20, 20));
    ball.setFillColor(sf::Color::Red);
    ball.setOutlineColor(sf::Color::Yellow);
    ball.setOutlineThickness(1);
}
