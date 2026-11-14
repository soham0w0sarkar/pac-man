#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Maze {
public:
  Maze();

  void render(sf::RenderWindow &window);

private:
  static constexpr int TILE_SIZE = 32;

  std::vector<std::vector<int>> grid;

  void createMaze();
  int getRows();
  int getColumns();
  bool isWall(int row, int column);
  int getWorldPosition(int row, int column);
};
