#include <Pathfinding.h>

#include <limits>
#include <queue>
#include <vector>

namespace {
// Tie-break order matches classic preference: Up, Left, Down, Right.
const sf::Vector2i kDirs[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

sf::Vector2i clampToMaze(const Maze &maze, sf::Vector2i tile) {
  const int maxX = maze.getColumns() - 1;
  const int maxY = maze.getRows() - 1;
  if (tile.x < 0) {
    tile.x = 0;
  } else if (tile.x > maxX) {
    tile.x = maxX;
  }
  if (tile.y < 0) {
    tile.y = 0;
  } else if (tile.y > maxY) {
    tile.y = maxY;
  }
  return tile;
}

std::vector<sf::Vector2i> giveNeighbours(const Maze &maze, sf::Vector2i tile) {
  std::vector<sf::Vector2i> result;
  result.reserve(4);
  for (const sf::Vector2i &dir : kDirs) {
    sf::Vector2i next{tile.x + dir.x, tile.y + dir.y};
    if (maze.isTunnelRow(tile.y) &&
        (next.x < 0 || next.x >= maze.getColumns())) {
      next = maze.wrapTile(next);
    }
    result.push_back(next);
  }
  return result;
}

bool isUnvisitedWalkable(const std::vector<std::vector<int>> &distanceGrid,
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
  return cell == std::numeric_limits<int>::max();
}

sf::Vector2i neighborStep(sf::Vector2i from, sf::Vector2i neighbor) {
  sf::Vector2i dir = neighbor - from;
  if (dir.x > 1) {
    dir.x = -1;
  } else if (dir.x < -1) {
    dir.x = 1;
  }
  return dir;
}
} // namespace

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

sf::Vector2i Pathfinding::directionTo(const PathContext &ctx,
                                      sf::Vector2i target) {
  return stepToward(ctx, target);
}

namespace {
sf::Vector2i scatterTarget(GhostId id, const Maze &maze) {
  const int maxX = maze.getColumns() - 1;
  const int maxY = maze.getRows() - 1;
  switch (id) {
  case GhostId::Blinky:
    return {maxX, -1}; // top-right (outside)
  case GhostId::Pinky:
    return {-1, -1}; // top-left
  case GhostId::Inky:
    return {maxX + 1, maxY + 1}; // bottom-right
  case GhostId::Clyde:
    return {-1, maxY + 1}; // bottom-left
  }
  return {0, 0};
}

sf::Vector2i chaseTarget(GhostId id, const PathContext &ctx) {
  switch (id) {
  case GhostId::Blinky:
    return ctx.pacmanTile;
  case GhostId::Pinky:
    return ctx.pacmanTile + ctx.pacmanDirection * 4;
  case GhostId::Inky: {
    const sf::Vector2i pivot = ctx.pacmanTile + ctx.pacmanDirection * 2;
    return pivot * 2 - ctx.blinkyTile;
  }
  case GhostId::Clyde: {
    constexpr int kFleeRange = 8;
    const int dx = ctx.ghostTile.x - ctx.pacmanTile.x;
    const int dy = ctx.ghostTile.y - ctx.pacmanTile.y;
    if (dx * dx + dy * dy > kFleeRange * kFleeRange) {
      return ctx.pacmanTile;
    }
    return scatterTarget(GhostId::Clyde, ctx.maze);
  }
  }
  return ctx.pacmanTile;
}
} // namespace

sf::Vector2i Pathfinding::blinky(const PathContext &ctx) {
  if (ctx.mode == GhostMode::Scatter && !ctx.cruiseElroy) {
    return directionTo(ctx, scatterTarget(GhostId::Blinky, ctx.maze));
  }
  return directionTo(ctx, chaseTarget(GhostId::Blinky, ctx));
}

sf::Vector2i Pathfinding::pinky(const PathContext &ctx) {
  if (ctx.mode == GhostMode::Scatter) {
    return directionTo(ctx, scatterTarget(GhostId::Pinky, ctx.maze));
  }
  return directionTo(ctx, chaseTarget(GhostId::Pinky, ctx));
}

sf::Vector2i Pathfinding::inky(const PathContext &ctx) {
  if (ctx.mode == GhostMode::Scatter) {
    return directionTo(ctx, scatterTarget(GhostId::Inky, ctx.maze));
  }
  return directionTo(ctx, chaseTarget(GhostId::Inky, ctx));
}

sf::Vector2i Pathfinding::clyde(const PathContext &ctx) {
  if (ctx.mode == GhostMode::Scatter) {
    return directionTo(ctx, scatterTarget(GhostId::Clyde, ctx.maze));
  }
  return directionTo(ctx, chaseTarget(GhostId::Clyde, ctx));
}

sf::Vector2i Pathfinding::stepToward(const PathContext &ctx,
                                     sf::Vector2i target) {
  constexpr int kInfinity = std::numeric_limits<int>::max();

  for (int row = 0; row < ctx.maze.getRows(); ++row) {
    for (int col = 0; col < ctx.maze.getColumns(); ++col) {
      distanceGrid[static_cast<std::size_t>(row)]
                  [static_cast<std::size_t>(col)] =
                      ctx.maze.isBlockedForGhost(row, col, ctx.allowHouse)
                          ? -1
                          : kInfinity;
    }
  }

  // Scatter corners sit outside / on walls. Seed that clamped cell anyway so
  // ghosts are still pulled toward the corner without treating it as failure.
  target = clampToMaze(ctx.maze, target);

  std::queue<sf::Vector2i> queue;
  distanceGrid[static_cast<std::size_t>(target.y)]
              [static_cast<std::size_t>(target.x)] = 0;
  queue.push(target);

  while (!queue.empty()) {
    const sf::Vector2i first = queue.front();
    queue.pop();
    const int currentDistance =
        distanceGrid[static_cast<std::size_t>(first.y)]
                    [static_cast<std::size_t>(first.x)];

    for (sf::Vector2i neighbor : giveNeighbours(ctx.maze, first)) {
      if (!isUnvisitedWalkable(distanceGrid, neighbor)) {
        continue;
      }
      distanceGrid[static_cast<std::size_t>(neighbor.y)]
                  [static_cast<std::size_t>(neighbor.x)] = currentDistance + 1;
      queue.push(neighbor);
    }
  }

  const sf::Vector2i start = ctx.ghostTile;
  if (start.y < 0 || start.x < 0 || start.y >= ctx.maze.getRows() ||
      start.x >= ctx.maze.getColumns()) {
    return {0, 0};
  }

  const int startDistance = distanceGrid[static_cast<std::size_t>(start.y)]
                                        [static_cast<std::size_t>(start.x)];
  const sf::Vector2i reverse{-ctx.ghostDirection.x, -ctx.ghostDirection.y};

  auto isWalkableNeighbor = [&](sf::Vector2i neighbor) {
    if (neighbor.x < 0 || neighbor.y < 0 ||
        neighbor.y >= static_cast<int>(distanceGrid.size()) ||
        neighbor.x >= static_cast<int>(distanceGrid[0].size())) {
      return false;
    }
    return distanceGrid[static_cast<std::size_t>(neighbor.y)]
                       [static_cast<std::size_t>(neighbor.x)] != -1;
  };

  auto pickDownhill = [&](bool allowReverse) -> sf::Vector2i {
    for (const sf::Vector2i &neighbor : giveNeighbours(ctx.maze, start)) {
      const sf::Vector2i dir = neighborStep(start, neighbor);
      if (!allowReverse && ctx.ghostDirection != sf::Vector2i{0, 0} &&
          dir == reverse) {
        continue;
      }
      if (!isWalkableNeighbor(neighbor)) {
        continue;
      }
      if (distanceGrid[static_cast<std::size_t>(neighbor.y)]
                      [static_cast<std::size_t>(neighbor.x)] ==
          startDistance - 1) {
        return dir;
      }
    }
    return {0, 0};
  };

  // Dead-end / already on seed: keep current heading if possible, else any
  // non-reverse open exit. Reverse only as a last resort.
  auto pickContinue = [&]() -> sf::Vector2i {
    if (ctx.ghostDirection != sf::Vector2i{0, 0}) {
      const sf::Vector2i ahead = start + ctx.ghostDirection;
      sf::Vector2i wrapped = ahead;
      if (ctx.maze.isTunnelRow(start.y) &&
          (wrapped.x < 0 || wrapped.x >= ctx.maze.getColumns())) {
        wrapped = ctx.maze.wrapTile(wrapped);
      }
      if (isWalkableNeighbor(wrapped)) {
        return ctx.ghostDirection;
      }
    }

    for (const sf::Vector2i &neighbor : giveNeighbours(ctx.maze, start)) {
      const sf::Vector2i dir = neighborStep(start, neighbor);
      if (ctx.ghostDirection != sf::Vector2i{0, 0} && dir == reverse) {
        continue;
      }
      if (isWalkableNeighbor(neighbor)) {
        return dir;
      }
    }

    if (ctx.ghostDirection != sf::Vector2i{0, 0} &&
        isWalkableNeighbor(start + reverse)) {
      return reverse;
    }
    return {0, 0};
  };

  if (startDistance == 0 || startDistance == kInfinity || startDistance < 0) {
    return pickContinue();
  }

  const sf::Vector2i chosen = pickDownhill(false);
  if (chosen != sf::Vector2i{0, 0}) {
    return chosen;
  }

  // Only reverse when the ghost is in a true dead end.
  const sf::Vector2i reversed = pickDownhill(true);
  if (reversed != sf::Vector2i{0, 0}) {
    return reversed;
  }

  return pickContinue();
}
