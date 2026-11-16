#pragma once

#include <GhostManager.h>
#include <Maze.h>
#include <Pacman.h>
#include <Pathfinding.h>
#include <SFML/Graphics.hpp>
#include <Sounds.h>
#include <optional>

enum class PlayState { Playing, Dying, LevelClear, GameOver, Paused };

class Game {
public:
  Game();
  void run();

private:
  Maze maze;
  sf::RenderWindow window;
  sf::View mazeView;

  Pacman pacman;
  GhostManager ghosts;
  Pathfinding pathfinding;
  Sounds sounds;
  sf::Clock clock;

  sf::Font font;
  sf::Text titleText;

  int score = 0;
  int highScore = 0;
  int lives = 3;
  int level = 1;
  int ghostEatStreak = 0;
  PlayState state = PlayState::Playing;
  float stateTimer = 0.f;
  bool fruitSpawned = false;
  bool fruitVisible = false;
  sf::Vector2i fruitTile{13, 17};

  std::optional<sf::Text> scoreText;
  std::optional<sf::Text> highScoreText;
  std::optional<sf::Text> livesText;
  std::optional<sf::Text> levelText;
  std::optional<sf::Text> statusText;

  void processEvents();
  void update();
  void render();
  void loadFont();
  void layoutForWindow();
  void setScore(int value);
  void updateHud();
  void applyLevelTuning();
  void resetRound(bool fullLevelReset);
  void startLevel();
  void handleDeath();
  void handleLevelClear();
  void restartGame();
  float powerDurationForLevel() const;
  void elroyThresholdsForLevel(int &elroy1, int &elroy2) const;
};
