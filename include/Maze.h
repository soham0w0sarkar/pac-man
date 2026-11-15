#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Maze {
public:
  Maze();

  void render(sf::RenderWindow &window);
  void centerIn(sf::Vector2u windowSize);

  sf::Vector2i getPlayerSpawn() const;
  int getRows() const;
  int getColumns() const;
  bool isWall(int row, int column) const;
  sf::Vector2f getWorldPosition(int row, int column) const;
  sf::Vector2f getWorldPosition(sf::Vector2i tile) const;
  bool collectPellet(int row, int col);

  static constexpr int TILE_SIZE = 32;

private:
  std::vector<std::vector<int>> grid;
  sf::Vector2f offset{0.f, 0.f};

  sf::Texture wallTexture;
  sf::Texture pelletTexture;
  sf::Texture berryTexture;

  void createMaze();
  void loadTextures();
};
