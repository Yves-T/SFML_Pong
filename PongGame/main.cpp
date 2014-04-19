#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

int main(int, char const **) {
    using namespace sf;

    // Create the main window
    RenderWindow window(VideoMode(800, 600), "SFML window");

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

        // Clear screen
        window.clear();

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
