#pragma once

#include "Ghost.h"
#include <Maze.h>
#include <Pacman.h>
#include <SFML/Graphics.hpp>
#include <optional>

class Game {
public:
  Game();
  void run();

private:
  sf::RenderWindow window;

  Maze maze;
  Pacman pacman;
  Ghost ghost;
  sf::Clock clock;

  sf::Font font;
  sf::Text titleText;

  int score = 0;
  std::optional<sf::Text> scoreText;

  void processEvents();
  void update();
  void render();
  void loadFont();
};
