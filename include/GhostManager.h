#pragma once

#include <Ghost.h>
#include <Maze.h>
#include <SFML/Graphics.hpp>
#include <array>

class Pathfinding;

class GhostManager {
public:
  explicit GhostManager(const Maze &maze);

  void spawnAll();
  void setMode(GhostMode mode);
  void enterFrightened();
  void exitFrightened();
  void setElroyThresholds(int elroy1, int elroy2);
  void setFrightenedFlash(bool flash);
  void update(float deltaTime, Pathfinding &pathfinding, sf::Vector2i pacmanTile,
              sf::Vector2i pacmanDirection, int pelletsRemaining,
              bool pacmanPowered);
  sf::Vector2i blinkyTile() const;
  GhostMode phaseMode() const;
  void render(sf::RenderWindow &window);

  std::array<Ghost *, 4> all();
  std::array<const Ghost *, 4> all() const;

private:
  Ghost blinky;
  Ghost pinky;
  Ghost inky;
  Ghost clyde;

  int nextReleaseIndex = 0;
  float releaseTimer = 0.f;
  static constexpr float kReleaseGap = 1.5f;

  int phaseIndex = 0;
  float phaseTimer = 0.f;
  bool phasePaused = false;
  GhostMode activePhase = GhostMode::Scatter;

  int elroyThreshold = 20;
  int elroy2Threshold = 10;

  // Level-1 style durations: S,C,S,C,S,C,S,C(forever)
  static constexpr float kPhaseDurations[] = {7.f, 20.f, 7.f, 20.f,
                                              5.f, 20.f, 5.f, 1e9f};

  void updateReleases(float deltaTime, bool pacmanPowered);
  void updatePhase(float deltaTime);
  void applyPhaseMode();
};
