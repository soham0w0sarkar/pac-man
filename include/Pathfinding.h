#pragma once

#include <Ghost.h>
#include <Maze.h>
#include <SFML/System/Vector2.hpp>
#include <vector>

struct PathContext {
  const Maze &maze;
  sf::Vector2i ghostTile;
  sf::Vector2i ghostDirection;
  sf::Vector2i pacmanTile;
  sf::Vector2i pacmanDirection{0, 0};
};

class Pathfinding {
public:
  explicit Pathfinding(const Maze &maze);
  static sf::Vector2i nextDirection(GhostId id, const PathContext &ctx);

private:
  static sf::Vector2i blinky(const PathContext &ctx);
  static sf::Vector2i pinky(const PathContext &ctx);
  static sf::Vector2i inky(const PathContext &ctx);
  static sf::Vector2i clyde(const PathContext &ctx);

  std::vector<std::vector<int>> distanceGrid;
  static std::vector<sf::Vector2i>
  legalDirections(const Maze &maze, sf::Vector2i tile, sf::Vector2i currentDir);
  static sf::Vector2i stepToward(const PathContext &ctx,
                                 sf::Vector2i targetTile);
};
