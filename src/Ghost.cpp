#include <Assets.h>
#include <Ghost.h>

#include <cmath>
#include <iostream>
#include <vector>

namespace {
const sf::Vector2i kDirs[] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};
}

Ghost::Ghost(const Maze &maze, sf::Color color)
    : maze(maze), sprite(texture) {
  loadTexture(color);
  sprite.setOrigin(sprite.getLocalBounds().getCenter());
}

void Ghost::spawn() {
  currentTile = maze.getGhostSpawn();
  targetTile = currentTile;
  direction = {0, 0};
  position = maze.getWorldPosition(currentTile);
  targetPosition = position;
  sprite.setPosition(position);
}

void Ghost::loadTexture(sf::Color color) {
  const auto path = findAsset("assets/sprites/ghost.png");
  if (!path || !texture.loadFromFile(*path)) {
    std::cerr << "Failed to load ghost.png\n";
    return;
  }

  texture.setSmooth(false);
  sprite.setTexture(texture, true);
  (void)color;
}

void Ghost::chooseDirection() {
  std::vector<sf::Vector2i> options;
  options.reserve(4);

  const sf::Vector2i reverse{-direction.x, -direction.y};

  for (const auto &dir : kDirs) {
    const int nextRow = currentTile.y + dir.y;
    const int nextCol = currentTile.x + dir.x;
    if (maze.isWall(nextRow, nextCol)) {
      continue;
    }
    if (direction != sf::Vector2i{0, 0} && dir == reverse) {
      continue;
    }
    options.push_back(dir);
  }

  // Dead end: allow reversing
  if (options.empty()) {
    for (const auto &dir : kDirs) {
      const int nextRow = currentTile.y + dir.y;
      const int nextCol = currentTile.x + dir.x;
      if (!maze.isWall(nextRow, nextCol)) {
        options.push_back(dir);
      }
    }
  }

  if (options.empty()) {
    direction = {0, 0};
    return;
  }

  direction = options[static_cast<std::size_t>(std::rand()) % options.size()];
}

void Ghost::update(float deltaTime) {
  if (position == targetPosition) {
    currentTile = targetTile;
    chooseDirection();

    targetTile = {currentTile.x + direction.x, currentTile.y + direction.y};
    targetPosition = maze.getWorldPosition(targetTile);
  }

  const float dx = targetPosition.x - position.x;
  const float dy = targetPosition.y - position.y;
  const float distance = std::sqrt(dx * dx + dy * dy);

  if (distance > 0.0f && direction != sf::Vector2i{0, 0}) {
    const float movement = speed * deltaTime;

    if (movement >= distance) {
      position = targetPosition;
    } else {
      position.x += (dx / distance) * movement;
      position.y += (dy / distance) * movement;
    }
  }

  sprite.setPosition(position);
}

void Ghost::render(sf::RenderWindow &window) { window.draw(sprite); }

sf::Vector2i Ghost::getTile() const { return currentTile; }

sf::Vector2f Ghost::getPosition() const { return position; }
