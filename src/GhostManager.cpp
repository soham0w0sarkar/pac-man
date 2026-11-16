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
  nextReleaseIndex = 0;
  releaseTimer = 0.f;
  phaseIndex = 0;
  phaseTimer = 0.f;
  phasePaused = false;
  activePhase = GhostMode::Scatter;
  blinky.release();
  nextReleaseIndex = 1;
}

void GhostManager::setMode(GhostMode mode) {
  for (Ghost *ghost : all()) {
    ghost->setMode(mode);
  }
}

void GhostManager::setElroyThresholds(int elroy1, int elroy2) {
  elroyThreshold = elroy1;
  elroy2Threshold = elroy2;
}

void GhostManager::setFrightenedFlash(bool flash) {
  for (Ghost *ghost : all()) {
    ghost->setFrightenedFlash(flash);
  }
}

void GhostManager::enterFrightened() {
  phasePaused = true;
  for (Ghost *ghost : all()) {
    const GhostMode mode = ghost->getMode();
    if (mode == GhostMode::Waiting || mode == GhostMode::Eaten) {
      continue;
    }
    if (mode == GhostMode::Leaving) {
      ghost->setLeaveAsFrightened(true);
      continue;
    }
    ghost->setMode(GhostMode::Frightened);
  }
}

void GhostManager::exitFrightened() {
  phasePaused = false;
  for (Ghost *ghost : all()) {
    ghost->clearFrightenedLeave();
    if (ghost->getMode() == GhostMode::Frightened) {
      ghost->setMode(activePhase);
    }
  }
}

GhostMode GhostManager::phaseMode() const { return activePhase; }

void GhostManager::applyPhaseMode() {
  for (Ghost *ghost : all()) {
    ghost->setResumeMode(activePhase);
    const GhostMode mode = ghost->getMode();
    if (mode == GhostMode::Scatter || mode == GhostMode::Chase) {
      ghost->setMode(activePhase);
    }
  }
}

void GhostManager::updatePhase(float deltaTime) {
  if (phasePaused) {
    return;
  }

  phaseTimer += deltaTime;
  const float limit =
      kPhaseDurations[static_cast<std::size_t>(phaseIndex)];
  if (phaseTimer < limit) {
    return;
  }

  phaseTimer = 0.f;
  if (phaseIndex < 7) {
    ++phaseIndex;
  }
  activePhase =
      (phaseIndex % 2 == 0) ? GhostMode::Scatter : GhostMode::Chase;
  applyPhaseMode();
}

void GhostManager::updateReleases(float deltaTime, bool pacmanPowered) {
  auto ghosts = all();

  if (nextReleaseIndex < static_cast<int>(ghosts.size())) {
    const Ghost *previous =
        ghosts[static_cast<std::size_t>(nextReleaseIndex - 1)];
    if (!previous->hasLeftHouse()) {
      return;
    }

    releaseTimer += deltaTime;
    if (releaseTimer < kReleaseGap) {
      return;
    }

    ghosts[static_cast<std::size_t>(nextReleaseIndex)]->release(
        pacmanPowered);
    ++nextReleaseIndex;
    releaseTimer = 0.f;
    return;
  }

  for (Ghost *ghost : ghosts) {
    if (ghost->getMode() == GhostMode::Waiting) {
      ghost->release(pacmanPowered);
    }
  }
}

void GhostManager::update(float deltaTime, Pathfinding &pathfinding,
                          sf::Vector2i pacmanTile,
                          sf::Vector2i pacmanDirection, int pelletsRemaining,
                          bool pacmanPowered) {
  updateReleases(deltaTime, pacmanPowered);
  updatePhase(deltaTime);

  const bool cruiseElroy = pelletsRemaining <= elroyThreshold;
  const bool cruiseElroy2 = pelletsRemaining <= elroy2Threshold;
  blinky.setCruiseElroy(cruiseElroy, cruiseElroy2);

  const sf::Vector2i blinkyPos = blinky.getTile();
  for (Ghost *ghost : all()) {
    ghost->setResumeMode(activePhase);
    ghost->update(deltaTime, pathfinding, pacmanTile, pacmanDirection,
                  blinkyPos, pacmanPowered);
  }
}

sf::Vector2i GhostManager::blinkyTile() const { return blinky.getTile(); }

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
