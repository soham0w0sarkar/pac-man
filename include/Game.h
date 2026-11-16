#pragma once

#include <GhostManager.h>
#include <Maze.h>
#include <Pacman.h>
#include <Pathfinding.h>
#include <SFML/Graphics.hpp>
#include <Sounds.h>
#include <optional>

class Game {
public:
  Game();
  void run();

private:
  sf::RenderWindow window;

  Maze maze;
  Pacman pacman;
  GhostManager ghosts;
  Pathfinding pathfinding;
  Sounds sounds;
  sf::Clock clock;

  sf::Font font;
  sf::Text titleText;

  int score = 0;
  std::optional<sf::Text> scoreText;

  void processEvents();
  void update();
  void render();
  void loadFont();
  void setScore(int value);
};
