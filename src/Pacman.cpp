#include <Assets.h>
#include <Pacman.h>

#include <cmath>
#include <iostream>

Pacman::Pacman(Maze &maze) : maze(maze), sprite(openTexture) {
  loadTextures();
  sprite.setTexture(openTexture, true);
  sprite.setOrigin(sprite.getLocalBounds().getCenter());
  spawn(maze);
}

void Pacman::loadTextures() {
  const auto openPath = findAsset("assets/sprites/pacman.png");
  const auto closedPath = findAsset("assets/sprites/pacman_closed.png");
  const auto poweredOpenPath = findAsset("assets/sprites/pacman_powered.png");
  const auto poweredClosedPath =
      findAsset("assets/sprites/pacman_powered_closed.png");

  if (!openPath || !openTexture.loadFromFile(*openPath)) {
    std::cerr << "Failed to load pacman.png\n";
  }
  if (!closedPath || !closedTexture.loadFromFile(*closedPath)) {
    std::cerr << "Failed to load pacman_closed.png\n";
  }
  if (!poweredOpenPath || !poweredOpenTexture.loadFromFile(*poweredOpenPath)) {
    std::cerr << "Failed to load pacman_powered.png\n";
  }
  if (!poweredClosedPath ||
      !poweredClosedTexture.loadFromFile(*poweredClosedPath)) {
    std::cerr << "Failed to load pacman_powered_closed.png\n";
  }

  openTexture.setSmooth(false);
  closedTexture.setSmooth(false);
  poweredOpenTexture.setSmooth(false);
  poweredClosedTexture.setSmooth(false);
}

void Pacman::spawn(Maze &maze) {
  currentTile = maze.getPacmanSpawn();
  targetTile = currentTile;
  direction = {0, 0};
  requestedDirection = {0, 0};
  mouthOpen = true;
  mouthTimer = 0.f;

  position = maze.getWorldPosition(currentTile);
  targetPosition = position;
  updateSprite();
}

void Pacman::render(sf::RenderWindow &window) { window.draw(sprite); }

void Pacman::handleInput() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    requestedDirection = {0, -1};
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    requestedDirection = {0, 1};
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    requestedDirection = {-1, 0};
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    requestedDirection = {1, 0};
}

bool Pacman::update(float deltaTime) {
  bool collectedPellet = false;
  bool collectedBerry = false;

  if (position == targetPosition) {
    currentTile = targetTile;

    collectedPellet = maze.collectPellet(currentTile.y, currentTile.x);
    collectedBerry = maze.collectBerry(currentTile.y, currentTile.x);

    const int nextReqRow = currentTile.y + requestedDirection.y;
    const int nextReqCol = currentTile.x + requestedDirection.x;

    if (requestedDirection != sf::Vector2i{0, 0} &&
        !maze.isWall(nextReqRow, nextReqCol)) {
      direction = requestedDirection;
    }

    const int nextRow = currentTile.y + direction.y;
    const int nextCol = currentTile.x + direction.x;

    if (maze.isWall(nextRow, nextCol)) {
      direction = {0, 0};
    }

    targetTile = {currentTile.x + direction.x, currentTile.y + direction.y};
    targetPosition = maze.getWorldPosition(targetTile);
  }

  if (collectedBerry) {
    powered = true;
    poweredTimer = 10.f;
  }

  if (powered) {
    poweredTimer -= deltaTime;
    if (poweredTimer <= 0.f) {
      poweredTimer = 0.f;
      powered = false;
    }
  }

  const float dx = targetPosition.x - position.x;
  const float dy = targetPosition.y - position.y;
  const float distance = std::sqrt(dx * dx + dy * dy);
  const bool moving = distance > 0.0f && direction != sf::Vector2i{0, 0};

  if (moving) {
    const float movement = speed * deltaTime;

    if (movement >= distance) {
      position = targetPosition;
    } else {
      position.x += (dx / distance) * movement;
      position.y += (dy / distance) * movement;
    }

    if (collectedPellet) {
      mouthOpen = false;
      mouthTimer = 0.f;
    } else if (!mouthOpen) {
      mouthTimer += deltaTime;
      if (mouthTimer >= 0.1f) {
        mouthOpen = true;
        mouthTimer = 0.f;
      }
    }
  } else {
    mouthOpen = true;
    mouthTimer = 0.f;
  }

  updateSprite();
  return collectedPellet;
}

void Pacman::updateSprite() {
  const sf::Texture &open = powered ? poweredOpenTexture : openTexture;
  const sf::Texture &closed = powered ? poweredClosedTexture : closedTexture;

  sprite.setTexture(mouthOpen ? open : closed, true);
  sprite.setOrigin(sprite.getLocalBounds().getCenter());
  sprite.setPosition(position);

  if (direction == sf::Vector2i{1, 0})
    sprite.setRotation(sf::degrees(0.f));
  else if (direction == sf::Vector2i{-1, 0})
    sprite.setRotation(sf::degrees(180.f));
  else if (direction == sf::Vector2i{0, -1})
    sprite.setRotation(sf::degrees(270.f));
  else if (direction == sf::Vector2i{0, 1})
    sprite.setRotation(sf::degrees(90.f));
}

bool Pacman::isPowered() const { return powered; }

sf::Vector2i Pacman::getTile() const { return currentTile; }

sf::Vector2f Pacman::getPosition() const { return position; }
