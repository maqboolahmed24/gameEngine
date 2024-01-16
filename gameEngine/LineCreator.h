// LineCreator.h
#include <SFML/Graphics.hpp>
#include <iostream>

class LineCreator {
public:
    LineCreator() {
        float length;
        std::cout << "Enter the length of the line: ";
        std::cin >> length;

        line.setSize(sf::Vector2f(length, 5)); // Fixed height of 5
        line.setFillColor(sf::Color::Black);
        line.setPosition(300, 300); // Example starting position
    }

    sf::RectangleShape getLine() const {
        return line;
    }

private:
    sf::RectangleShape line;
};
