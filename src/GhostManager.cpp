#include <GhostManager.h>

GhostManager::GhostManager(const Maze &maze)
    : blinky(maze, GhostId::Blinky, sf::Color(255, 0, 0)),
      pinky(maze, GhostId::Pinky, sf::Color(255, 184, 255)),
      inky(maze, GhostId::Inky, sf::Color(0, 255, 255)),
      clyde(maze, GhostId::Clyde, sf::Color(255, 184, 82)) {}

void GhostManager::spawnAll() {
  for (Ghost *ghost : all()) {
    ghost->spawn();
  }
}

void GhostManager::update(float deltaTime, Pathfinding &pathfinding,
                          sf::Vector2i pacmanTile,
                          sf::Vector2i pacmanDirection) {
  for (Ghost *ghost : all()) {
    ghost->update(deltaTime, pathfinding, pacmanTile, pacmanDirection);
  }
}

void GhostManager::render(sf::RenderWindow &window) {
  for (Ghost *ghost : all()) {
    ghost->render(window);
  }
}

std::array<Ghost *, 4> GhostManager::all() {
  return {&blinky, &pinky, &inky, &clyde};
}

std::array<const Ghost *, 4> GhostManager::all() const {
  return {&blinky, &pinky, &inky, &clyde};
}
