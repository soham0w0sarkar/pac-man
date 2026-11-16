#include <Pathfinding.h>

#include <limits>
#include <queue>
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
  return stepToward(ctx, ctx.pacmanTile);
}

sf::Vector2i Pathfinding::pinky(const PathContext &ctx) {
  return stepToward(ctx, ctx.pacmanTile + ctx.pacmanDirection * 4);
}
sf::Vector2i Pathfinding::inky(const PathContext &) { return {0, 0}; }
sf::Vector2i Pathfinding::clyde(const PathContext &) { return {0, 0}; }

namespace {
const sf::Vector2i kDirs[] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};

std::vector<sf::Vector2i> giveNeighbours(sf::Vector2i tile) {
  return {tile + kDirs[0], tile + kDirs[1], tile + kDirs[2], tile + kDirs[3]};
}

bool isLegal(const std::vector<std::vector<int>> &distanceGrid,
             sf::Vector2i tile) {
  const int nx = tile.x;
  const int ny = tile.y;

  if (distanceGrid.empty() || distanceGrid[0].empty()) {
    return false;
  }
  if (nx < 0 || ny < 0) {
    return false;
  }
  if (ny >= static_cast<int>(distanceGrid.size()) ||
      nx >= static_cast<int>(distanceGrid[0].size())) {
    return false;
  }
  const int cell =
      distanceGrid[static_cast<std::size_t>(ny)][static_cast<std::size_t>(nx)];
  if (cell == -1) {
    return false;
  }
  if (cell != std::numeric_limits<int>::max()) {
    return false;
  }

  return true;
}
} // namespace

sf::Vector2i Pathfinding::stepToward(const PathContext &ctx,
                                     sf::Vector2i target) {
  constexpr int kInfinity = std::numeric_limits<int>::max();

  for (auto &row : distanceGrid) {
    for (int &cell : row) {
      if (cell != -1) {
        cell = kInfinity;
      }
    }
  }

  std::queue<sf::Vector2i> queue;

  distanceGrid[target.y][target.x] = 0;
  queue.push(target);

  while (!queue.empty()) {
    auto first = queue.front();
    queue.pop();
    auto currentDistance = distanceGrid[first.y][first.x];

    auto neighbors = giveNeighbours(first);
    for (auto &neighbor : neighbors) {
      if (!isLegal(distanceGrid, neighbor)) {
        continue;
      }
      distanceGrid[neighbor.y][neighbor.x] = currentDistance + 1;
      queue.push(neighbor);
    }
  }

  const sf::Vector2i start = ctx.ghostTile;
  const int startDistance = distanceGrid[start.y][start.x];
  if (startDistance == 0 || startDistance == kInfinity) {
    return {0, 0};
  }

  const sf::Vector2i reverse{-ctx.ghostDirection.x, -ctx.ghostDirection.y};

  auto pickDownhill = [&](bool allowReverse) -> sf::Vector2i {
    for (const sf::Vector2i &neighbor : giveNeighbours(start)) {
      const sf::Vector2i dir = neighbor - start;
      if (!allowReverse && ctx.ghostDirection != sf::Vector2i{0, 0} &&
          dir == reverse) {
        continue;
      }

      const int nx = neighbor.x;
      const int ny = neighbor.y;
      if (nx < 0 || ny < 0 || ny >= static_cast<int>(distanceGrid.size()) ||
          nx >= static_cast<int>(distanceGrid[0].size())) {
        continue;
      }

      const int neighborDistance = distanceGrid[ny][nx];
      if (neighborDistance == startDistance - 1) {
        return dir;
      }
    }
    return {0, 0};
  };

  const sf::Vector2i chosen = pickDownhill(false);
  if (chosen != sf::Vector2i{0, 0}) {
    return chosen;
  }

  return pickDownhill(true);
}
