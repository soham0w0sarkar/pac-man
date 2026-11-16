#include <Assets.h>
#include <Ghost.h>
#include <Pathfinding.h>

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {
const sf::Vector2i kFrightenedPriority[] = {
    {0, -1}, {-1, 0}, {0, 1}, {1, 0}};
} // namespace

Ghost::Ghost(const Maze &maze, GhostId id, sf::Color color)
    : maze(maze), id(id), color(color), sprite(texture),
      eyesSprite(eyesTexture) {
  loadTexture();
  sprite.setOrigin(sprite.getLocalBounds().getCenter());
  eyesSprite.setOrigin(eyesSprite.getLocalBounds().getCenter());
}

GhostId Ghost::getId() const { return id; }

GhostMode Ghost::getMode() const { return mode; }

void Ghost::setResumeMode(GhostMode newMode) { resumeMode = newMode; }

void Ghost::setCruiseElroy(bool elroy, bool elroy2) {
  cruiseElroy = elroy;
  cruiseElroy2 = elroy2;
}

void Ghost::setFrightenedFlash(bool flash) { frightenedFlash = flash; }

void Ghost::setLeaveAsFrightened(bool value) { leaveAsFrightened = value; }

void Ghost::setMode(GhostMode newMode) {
  if (mode == GhostMode::Waiting || mode == GhostMode::Eaten) {
    return;
  }

  const bool wasRoaming =
      mode == GhostMode::Chase || mode == GhostMode::Scatter ||
      mode == GhostMode::Frightened;
  const bool willRoam = newMode == GhostMode::Chase ||
                        newMode == GhostMode::Scatter ||
                        newMode == GhostMode::Frightened;

  if (wasRoaming && willRoam && newMode != mode) {
    reverseDirection();
  }

  mode = newMode;
  syncSprites();
}

void Ghost::eat() {
  if (mode != GhostMode::Frightened) {
    return;
  }
  mode = GhostMode::Eaten;
  leaveAsFrightened = false;
  reverseDirection();
  syncSprites();
}

bool Ghost::hasLeftHouse() const {
  return mode != GhostMode::Waiting && mode != GhostMode::Leaving;
}

bool Ghost::houseAccess() const {
  return mode == GhostMode::Leaving || mode == GhostMode::Eaten ||
         mode == GhostMode::Waiting;
}

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

sf::Vector2i Ghost::exitTile() const {
  const sf::Vector2i base = maze.getGhostSpawn();
  return {base.x - 1, base.y - 4};
}

void Ghost::spawn() {
  currentTile = spawnTile();
  targetTile = currentTile;
  direction = {0, 0};
  mode = GhostMode::Waiting;
  leaveAsFrightened = false;
  cruiseElroy = false;
  cruiseElroy2 = false;
  frightenedFlash = false;
  position = maze.getWorldPosition(currentTile);
  targetPosition = position;
  syncSprites();
}

void Ghost::release(bool asFrightened) {
  if (mode == GhostMode::Waiting) {
    mode = GhostMode::Leaving;
    leaveAsFrightened = asFrightened;
    syncSprites();
  }
}

void Ghost::clearFrightenedLeave() { leaveAsFrightened = false; }

void Ghost::loadTexture() {
  const auto path = findAsset("assets/sprites/ghost.png");
  const auto frightenedPath = findAsset("assets/sprites/ghost_frightened.png");
  const auto eyesPath = findAsset("assets/sprites/ghost_eyes.png");

  if (!path || !texture.loadFromFile(*path)) {
    std::cerr << "Failed to load ghost.png\n";
  }
  if (!frightenedPath || !frightenedTexture.loadFromFile(*frightenedPath)) {
    std::cerr << "Failed to load ghost_frightened.png\n";
  }
  if (!eyesPath || !eyesTexture.loadFromFile(*eyesPath)) {
    std::cerr << "Failed to load ghost_eyes.png\n";
  }

  texture.setSmooth(false);
  frightenedTexture.setSmooth(false);
  eyesTexture.setSmooth(false);
  sprite.setTexture(texture, true);
  eyesSprite.setTexture(eyesTexture, true);
  sprite.setColor(color);
}

void Ghost::syncSprites() {
  if (mode == GhostMode::Eaten) {
    sprite.setColor(sf::Color::Transparent);
  } else if (mode == GhostMode::Frightened) {
    if (frightenedFlash) {
      sprite.setTexture(texture, true);
      sprite.setColor(color);
    } else {
      sprite.setTexture(frightenedTexture, true);
      sprite.setColor(sf::Color::White);
    }
  } else {
    sprite.setTexture(texture, true);
    sprite.setColor(color);
  }
  sprite.setOrigin(sprite.getLocalBounds().getCenter());
  sprite.setPosition(position);
  eyesSprite.setPosition(position);
}

void Ghost::reverseDirection() {
  if (direction == sf::Vector2i{0, 0}) {
    return;
  }

  direction = {-direction.x, -direction.y};
  targetTile = currentTile;
  targetPosition = maze.getWorldPosition(currentTile);
}

sf::Vector2i Ghost::nextTileAlong(sf::Vector2i dir) const {
  sf::Vector2i next{currentTile.x + dir.x, currentTile.y + dir.y};
  if (maze.isTunnelRow(currentTile.y) &&
      (next.x < 0 || next.x >= maze.getColumns())) {
    next = maze.wrapTile(next);
  }
  return next;
}

bool Ghost::isOpenDir(sf::Vector2i dir) const {
  if (dir == sf::Vector2i{0, 0}) {
    return false;
  }
  const sf::Vector2i next = nextTileAlong(dir);
  return !maze.isBlockedForGhost(next.y, next.x, houseAccess());
}

void Ghost::chooseLeavingDirection(Pathfinding &pathfinding) {
  const sf::Vector2i exit = exitTile();
  if (currentTile == exit) {
    mode = leaveAsFrightened ? GhostMode::Frightened : resumeMode;
    leaveAsFrightened = false;
    direction = {0, 0};
    syncSprites();
    return;
  }

  const PathContext ctx{maze, currentTile, {0, 0}, exit, {}, currentTile, mode,
                        false, true};
  direction = pathfinding.directionTo(ctx, exit);
}

void Ghost::chooseEatenDirection(Pathfinding &pathfinding, bool pacmanPowered) {
  const sf::Vector2i home = spawnTile();
  if (currentTile == home) {
    mode = GhostMode::Leaving;
    leaveAsFrightened = pacmanPowered;
    direction = {0, 0};
    syncSprites();
    return;
  }

  const PathContext ctx{maze,  currentTile, direction, home, {},
                        home,  mode,        false,     true};
  direction = pathfinding.directionTo(ctx, home);
}

void Ghost::chooseFrightenedDirection() {
  const sf::Vector2i reverse{-direction.x, -direction.y};

  auto isOpen = [&](sf::Vector2i dir) {
    if (dir == sf::Vector2i{0, 0}) {
      return false;
    }
    if (direction != sf::Vector2i{0, 0} && dir == reverse) {
      return false;
    }
    return isOpenDir(dir);
  };

  const sf::Vector2i randomPick =
      kFrightenedPriority[static_cast<std::size_t>(std::rand()) % 4];
  if (isOpen(randomPick)) {
    direction = randomPick;
    return;
  }

  for (const auto &dir : kFrightenedPriority) {
    if (isOpen(dir)) {
      direction = dir;
      return;
    }
  }

  for (const auto &dir : kFrightenedPriority) {
    if (isOpenDir(dir)) {
      direction = dir;
      return;
    }
  }

  direction = {0, 0};
}

void Ghost::chooseDirection(Pathfinding &pathfinding, sf::Vector2i pacmanTile,
                            sf::Vector2i pacmanDirection,
                            sf::Vector2i blinkyTile, bool pacmanPowered) {
  if (mode == GhostMode::Leaving) {
    chooseLeavingDirection(pathfinding);
    return;
  }

  if (mode == GhostMode::Eaten) {
    chooseEatenDirection(pathfinding, pacmanPowered);
    return;
  }

  if (mode == GhostMode::Frightened) {
    chooseFrightenedDirection();
    return;
  }

  const PathContext ctx{maze,     currentTile,     direction, pacmanTile,
                        pacmanDirection, blinkyTile,   mode,    cruiseElroy,
                        false};
  direction = pathfinding.nextDirection(id, ctx);
}

float Ghost::currentSpeed() const {
  if (mode == GhostMode::Eaten) {
    return speed * 2.0f;
  }
  if (mode == GhostMode::Frightened) {
    return speed * 0.75f;
  }
  if (id == GhostId::Blinky && cruiseElroy2) {
    return speed * 1.15f;
  }
  if (id == GhostId::Blinky && cruiseElroy) {
    return speed * 1.05f;
  }
  return speed;
}

void Ghost::update(float deltaTime, Pathfinding &pathfinding,
                   sf::Vector2i pacmanTile, sf::Vector2i pacmanDirection,
                   sf::Vector2i blinkyTile, bool pacmanPowered) {
  if (mode == GhostMode::Waiting) {
    syncSprites();
    return;
  }

  if (position == targetPosition) {
    currentTile = targetTile;

    if (mode == GhostMode::Leaving && currentTile == exitTile()) {
      mode = leaveAsFrightened ? GhostMode::Frightened : resumeMode;
      leaveAsFrightened = false;
      syncSprites();
    }

    if (mode == GhostMode::Eaten && currentTile == spawnTile()) {
      mode = GhostMode::Leaving;
      leaveAsFrightened = pacmanPowered;
      syncSprites();
    }

    chooseDirection(pathfinding, pacmanTile, pacmanDirection, blinkyTile,
                    pacmanPowered);

    if (direction == sf::Vector2i{0, 0}) {
      targetTile = currentTile;
      targetPosition = position;
    } else {
      targetTile = nextTileAlong(direction);
      targetPosition = maze.getWorldPosition(targetTile);

      // Instant wrap across tunnel: jump world position when wrapping.
      if (std::abs(targetTile.x - currentTile.x) > 1) {
        position = targetPosition;
      }
    }
  }

  const float dx = targetPosition.x - position.x;
  const float dy = targetPosition.y - position.y;
  const float distance = std::sqrt(dx * dx + dy * dy);

  if (distance > 0.0f && direction != sf::Vector2i{0, 0}) {
    const float movement = currentSpeed() * deltaTime;

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
  if (mode != GhostMode::Eaten) {
    window.draw(sprite);
  }
  if (mode != GhostMode::Frightened || frightenedFlash) {
    window.draw(eyesSprite);
  }
}

sf::Vector2i Ghost::getTile() const { return currentTile; }

sf::Vector2f Ghost::getPosition() const { return position; }
