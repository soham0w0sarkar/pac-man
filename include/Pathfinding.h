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
  sf::Vector2i blinkyTile{0, 0};
  GhostMode mode = GhostMode::Chase;
  bool cruiseElroy = false;
  bool allowHouse = false;
};

class Pathfinding {
public:
  explicit Pathfinding(const Maze &maze);
  sf::Vector2i nextDirection(GhostId id, const PathContext &ctx);
  sf::Vector2i directionTo(const PathContext &ctx, sf::Vector2i target);

private:
  sf::Vector2i blinky(const PathContext &ctx);
  sf::Vector2i pinky(const PathContext &ctx);
  sf::Vector2i inky(const PathContext &ctx);
  sf::Vector2i clyde(const PathContext &ctx);

  std::vector<std::vector<int>> distanceGrid;
  sf::Vector2i stepToward(const PathContext &ctx, sf::Vector2i targetTile);
};
