#include <Game.h>

Game::Game() : window(sf::VideoMode({896, 992}), "Pac-Man") {}

void Game::run() {
  while (window.isOpen()) {
    processEvents();
    update();
    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
  }
}

void Game::update() {}

void Game::render() {
  window.clear(sf::Color::Black);
  maze.render(window);
  window.display();
}
