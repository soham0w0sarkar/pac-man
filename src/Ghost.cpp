#include <Assets.h>
#include <Ghost.h>

#include <cmath>
#include <iostream>
#include <vector>

namespace {
const sf::Vector2i kDirs[] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};
}

Ghost::Ghost(const Maze &maze, GhostId id, sf::Color color)
    : maze(maze), id(id), color(color), sprite(texture), eyesSprite(eyesTexture) {
  loadTexture();
  sprite.setOrigin(sprite.getLocalBounds().getCenter());
  eyesSprite.setOrigin(eyesSprite.getLocalBounds().getCenter());
}

GhostId Ghost::getId() const { return id; }

sf::Vector2i Ghost::spawnTile() const {
  const sf::Vector2i base = maze.getGhostSpawn();
  switch (id) {
  case GhostId::Blinky:
    return base;
  case GhostId::Pinky:
    return {base.x - 1, base.y};
  case GhostId::Inky:
    return {base.x + 1, base.y};
  case GhostId::Clyde:
    return {base.x, base.y + 1};
  }
  return base;
}

void Ghost::spawn() {
  currentTile = spawnTile();
  targetTile = currentTile;
  direction = {0, 0};
  position = maze.getWorldPosition(currentTile);
  targetPosition = position;
  syncSprites();
}

void Ghost::loadTexture() {
  const auto path = findAsset("assets/sprites/ghost.png");
  const auto eyesPath = findAsset("assets/sprites/ghost_eyes.png");

  if (!path || !texture.loadFromFile(*path)) {
    std::cerr << "Failed to load ghost.png\n";
  }
  if (!eyesPath || !eyesTexture.loadFromFile(*eyesPath)) {
    std::cerr << "Failed to load ghost_eyes.png\n";
  }

  texture.setSmooth(false);
  eyesTexture.setSmooth(false);
  sprite.setTexture(texture, true);
  eyesSprite.setTexture(eyesTexture, true);
  sprite.setColor(color);
}

void Ghost::syncSprites() {
  sprite.setPosition(position);
  eyesSprite.setPosition(position);
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

  syncSprites();
}

void Ghost::render(sf::RenderWindow &window) {
  window.draw(sprite);
  window.draw(eyesSprite);
}

sf::Vector2i Ghost::getTile() const { return currentTile; }

sf::Vector2f Ghost::getPosition() const { return position; }
