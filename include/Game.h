#pragma once

#include <Maze.h>
#include <SFML/Graphics.hpp>

class Game {
public:
  Game();
  void run();

private:
  sf::RenderWindow window;

  Maze maze;
  void processEvents();
  void update();
  void render();
};
