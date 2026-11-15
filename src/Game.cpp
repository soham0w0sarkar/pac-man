#include <Game.h>

Game::Game()
    : window(
        sf::VideoMode::getDesktopMode(),
        "Pac-Man",
        sf::State::Fullscreen
    )
{
}

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
