#include <SFML\Graphics.hpp>
int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "nibiruman2080");
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}