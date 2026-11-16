#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Maze {
public:
  Maze();

  void render(sf::RenderWindow &window);
  void centerIn(sf::Vector2u windowSize);
  void resetPellets();

  sf::Vector2i getPacmanSpawn() const;
  sf::Vector2i getGhostSpawn() const;
  int getRows() const;
  int getColumns() const;
  bool isWall(int row, int column) const;
  bool isDoor(int row, int column) const;
  bool isGhostHouse(int row, int column) const;
  bool isBlockedForPacman(int row, int column) const;
  bool isBlockedForGhost(int row, int column, bool allowHouse) const;
  bool isTunnelRow(int row) const;
  sf::Vector2i wrapTile(sf::Vector2i tile) const;
  sf::Vector2f getWorldPosition(int row, int column) const;
  sf::Vector2f getWorldPosition(sf::Vector2i tile) const;
  bool collectPellet(int row, int col);
  bool collectBerry(int row, int col);
  int remainingPellets() const;
  int totalPellets() const;

  static constexpr int TILE_SIZE = 32;

private:
  std::vector<std::vector<int>> grid;
  std::vector<std::vector<int>> initialGrid;
  int pelletCount = 0;
  int totalPelletCount = 0;
  sf::Vector2f offset{0.f, 0.f};

  sf::Texture wallTexture;
  sf::Texture pelletTexture;
  sf::Texture berryTexture;

  void createMaze();
  void loadTextures();
  void recountPellets();
};
