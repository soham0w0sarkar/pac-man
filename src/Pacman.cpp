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

  auto load = [](const char *label, const auto &path, sf::Texture &texture) {
    if (!path) {
      std::cerr << "Missing asset: " << label << "\n";
      return;
    }
    if (!texture.loadFromFile(*path)) {
      std::cerr << "Failed to load " << label << " from " << path->string()
                << "\n";
    }
  };

  load("pacman.png", openPath, openTexture);
  load("pacman_closed.png", closedPath, closedTexture);
  load("pacman_powered.png", poweredOpenPath, poweredOpenTexture);
  load("pacman_powered_closed.png", poweredClosedPath, poweredClosedTexture);

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
  powered = false;
  poweredTimer = 0.f;
  speed = 150.f;

  position = maze.getWorldPosition(currentTile);
  targetPosition = position;
  updateSprite();
}

void Pacman::clearPower() {
  powered = false;
  poweredTimer = 0.f;
  speed = 150.f;
}

void Pacman::setPowerDuration(float seconds) { powerDuration = seconds; }

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

sf::Vector2i Pacman::nextTileAlong(sf::Vector2i dir) const {
  sf::Vector2i next{currentTile.x + dir.x, currentTile.y + dir.y};
  if (maze.isTunnelRow(currentTile.y) &&
      (next.x < 0 || next.x >= maze.getColumns())) {
    next = maze.wrapTile(next);
  }
  return next;
}

bool Pacman::canEnter(sf::Vector2i tile) const {
  return !maze.isBlockedForPacman(tile.y, tile.x);
}

PacmanUpdate Pacman::update(float deltaTime) {
  PacmanUpdate result;

  if (position == targetPosition) {
    currentTile = targetTile;

    result.atePellet = maze.collectPellet(currentTile.y, currentTile.x);
    result.ateBerry = maze.collectBerry(currentTile.y, currentTile.x);

    if (requestedDirection != sf::Vector2i{0, 0} &&
        canEnter(nextTileAlong(requestedDirection))) {
      direction = requestedDirection;
    }

    if (!canEnter(nextTileAlong(direction))) {
      direction = {0, 0};
    }

    if (direction == sf::Vector2i{0, 0}) {
      targetTile = currentTile;
      targetPosition = position;
    } else {
      targetTile = nextTileAlong(direction);
      targetPosition = maze.getWorldPosition(targetTile);
      if (std::abs(targetTile.x - currentTile.x) > 1) {
        position = targetPosition;
      }
    }
  }

  if (result.ateBerry) {
    powered = true;
    poweredTimer = powerDuration;
    speed = 200.f;
    result.powerStarted = true;
  }

  if (powered) {
    poweredTimer -= deltaTime;
    if (poweredTimer <= 0.f) {
      poweredTimer = 0.f;
      powered = false;
      speed = 150.f;
      result.powerEnded = true;
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

    if (result.atePellet) {
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
  return result;
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

float Pacman::powerRemaining() const { return poweredTimer; }

sf::Vector2i Pacman::getTile() const { return currentTile; }

sf::Vector2i Pacman::getDirection() const { return direction; }

sf::Vector2f Pacman::getPosition() const { return position; }
