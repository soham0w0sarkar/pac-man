#include <Assets.h>
#include <Maze.h>

#include <iostream>

Maze::Maze() {
  createMaze();
  initialGrid = grid;
  recountPellets();
  totalPelletCount = pelletCount;
  loadTextures();
}

void Maze::recountPellets() {
  pelletCount = 0;
  for (const auto &row : grid) {
    for (int cell : row) {
      if (cell == 2) {
        ++pelletCount;
      }
    }
  }
}

void Maze::resetPellets() {
  grid = initialGrid;
  recountPellets();
}

void Maze::loadTextures() {
  const auto wallPath = findAsset("assets/sprites/wall.png");
  const auto pelletPath = findAsset("assets/sprites/pellet.png");
  const auto berryPath = findAsset("assets/sprites/berry.png");

  if (!wallPath || !wallTexture.loadFromFile(*wallPath)) {
    std::cerr << "Failed to load wall.png\n";
  }
  if (!pelletPath || !pelletTexture.loadFromFile(*pelletPath)) {
    std::cerr << "Failed to load pellet.png\n";
  }
  if (!berryPath || !berryTexture.loadFromFile(*berryPath)) {
    std::cerr << "Failed to load berry.png\n";
  }

  wallTexture.setSmooth(false);
  pelletTexture.setSmooth(false);
  berryTexture.setSmooth(false);
}

void Maze::createMaze() {
  grid = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
       1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
      {1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1},
      {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
      {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1,
       1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
      {1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1,
       1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
      {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1,
       1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
      {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1,
       1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1,
       1, 0, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 0,
       0, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 4,
       0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0,
       0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0,
       0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1,
       1, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1,
       1, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1,
       1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
      {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
       1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
      {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
      {1, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 5,
       0, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 3, 1},
      {1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1,
       1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1},
      {1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1,
       1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1},
      {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1,
       1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
      {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
      {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
       1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
      {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  };
}

bool Maze::isDoor(int row, int column) const {
  return row == 9 && (column == 12 || column == 15);
}

bool Maze::isGhostHouse(int row, int column) const {
  return row >= 11 && row <= 15 && column >= 9 && column <= 18;
}

bool Maze::isBlockedForPacman(int row, int column) const {
  return isWall(row, column) || isDoor(row, column) ||
         isGhostHouse(row, column);
}

bool Maze::isBlockedForGhost(int row, int column, bool allowHouse) const {
  if (isWall(row, column)) {
    return true;
  }
  if (!allowHouse && isGhostHouse(row, column)) {
    return true;
  }
  return false;
}

bool Maze::isTunnelRow(int row) const { return row == 14; }

sf::Vector2i Maze::wrapTile(sf::Vector2i tile) const {
  if (tile.x < 0) {
    tile.x = getColumns() - 1;
  } else if (tile.x >= getColumns()) {
    tile.x = 0;
  }
  return tile;
}

void Maze::centerIn(sf::Vector2u) {
  // World origin stays at (0,0); Game fits the maze with a view.
  offset = {0.f, 0.f};
}

void Maze::render(sf::RenderWindow &window) {
  sf::Sprite wall(wallTexture);
  sf::Sprite pellet(pelletTexture);
  sf::Sprite berry(berryTexture);

  wall.setOrigin(wall.getLocalBounds().getCenter());
  pellet.setOrigin(pellet.getLocalBounds().getCenter());
  berry.setOrigin(berry.getLocalBounds().getCenter());

  for (std::size_t row = 0; row < grid.size(); row++) {
    for (std::size_t col = 0; col < grid[row].size(); col++) {
      const sf::Vector2f center =
          getWorldPosition(static_cast<int>(row), static_cast<int>(col));

      if (grid[row][col] == 1) {
        wall.setPosition(center);
        window.draw(wall);
      } else if (grid[row][col] == 2) {
        pellet.setPosition(center);
        window.draw(pellet);
      } else if (grid[row][col] == 3) {
        berry.setPosition(center);
        window.draw(berry);
      }
    }
  }
}

int Maze::getRows() const { return static_cast<int>(grid.size()); }

int Maze::getColumns() const { return static_cast<int>(grid[0].size()); }

bool Maze::isWall(int row, int column) const {
  if (row < 0 || column < 0 || row >= getRows() || column >= getColumns()) {
    return true;
  }
  return grid[row][column] == 1;
}

sf::Vector2f Maze::getWorldPosition(int row, int column) const {
  return offset +
         sf::Vector2f(static_cast<float>(column * TILE_SIZE) + TILE_SIZE / 2.f,
                      static_cast<float>(row * TILE_SIZE) + TILE_SIZE / 2.f);
}

sf::Vector2f Maze::getWorldPosition(sf::Vector2i tile) const {
  return getWorldPosition(tile.y, tile.x);
}

sf::Vector2i Maze::getPacmanSpawn() const {
  for (int row = 0; row < getRows(); row++) {
    for (int col = 0; col < getColumns(); col++) {
      if (grid[row][col] == 5) {
        return {col, row};
      }
    }
  }

  return {1, 1};
}

sf::Vector2i Maze::getGhostSpawn() const {
  for (int row = 0; row < getRows(); row++) {
    for (int col = 0; col < getColumns(); col++) {
      if (grid[row][col] == 4) {
        return {col, row};
      }
    }
  }

  return {1, 1};
}

bool Maze::collectPellet(int row, int col) {
  if (row < 0 || row >= getRows() || col < 0 || col >= getColumns())
    return false;

  if (grid[row][col] == 2) {
    grid[row][col] = 0;
    if (pelletCount > 0) {
      --pelletCount;
    }
    return true;
  }

  return false;
}

int Maze::remainingPellets() const { return pelletCount; }

int Maze::totalPellets() const { return totalPelletCount; }

bool Maze::collectBerry(int row, int col) {
  if (row < 0 || row >= getRows() || col < 0 || col >= getColumns())
    return false;

  if (grid[row][col] == 3) {
    grid[row][col] = 0;
    return true;
  }

  return false;
}