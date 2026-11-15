#pragma once

#include <Ghost.h>
#include <Maze.h>
#include <SFML/Graphics.hpp>
#include <array>

class GhostManager {
public:
  explicit GhostManager(const Maze &maze);

  void spawnAll();
  void update(float deltaTime);
  void render(sf::RenderWindow &window);

  std::array<Ghost *, 4> all();
  std::array<const Ghost *, 4> all() const;

private:
  Ghost blinky;
  Ghost pinky;
  Ghost inky;
  Ghost clyde;
};
