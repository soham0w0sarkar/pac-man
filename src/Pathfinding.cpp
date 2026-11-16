#include <Pathfinding.h>

#include <limits>
#include <vector>

Pathfinding::Pathfinding(const Maze &maze)
    : distanceGrid(
          static_cast<std::size_t>(maze.getRows()),
          std::vector<int>(static_cast<std::size_t>(maze.getColumns()))) {
  constexpr int kInfinity = std::numeric_limits<int>::max();
  for (int row = 0; row < maze.getRows(); ++row) {
    for (int col = 0; col < maze.getColumns(); ++col) {
      distanceGrid[static_cast<std::size_t>(row)]
                  [static_cast<std::size_t>(col)] =
                      maze.isWall(row, col) ? -1 : kInfinity;
    }
  }
}

sf::Vector2i Pathfinding::nextDirection(GhostId id, const PathContext &ctx) {
  switch (id) {
  case GhostId::Blinky:
    return blinky(ctx);
  case GhostId::Pinky:
    return pinky(ctx);
  case GhostId::Inky:
    return inky(ctx);
  case GhostId::Clyde:
    return clyde(ctx);
  }
  return {0, 0};
}

sf::Vector2i Pathfinding::blinky(const PathContext &ctx) {
  auto target = ctx.pacmanTile;

  return stepToward(ctx, target);
}

sf::Vector2i Pathfinding::pinky(const PathContext &) { return {0, 0}; }
sf::Vector2i Pathfinding::inky(const PathContext &) { return {0, 0}; }
sf::Vector2i Pathfinding::clyde(const PathContext &) { return {0, 0}; }

std::vector<sf::Vector2i>
Pathfinding::legalDirections(const Maze &, sf::Vector2i, sf::Vector2i) {
  return {};
}

sf::Vector2i Pathfinding::stepToward(const PathContext &, sf::Vector2i) {

  return {0, 0};
}
