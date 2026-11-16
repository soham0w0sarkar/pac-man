#include "Ghost.h"
#include "Maze.h"
#include <Assets.h>
#include <Game.h>

#include <algorithm>
#include <iostream>

namespace {
constexpr float kDeathDelay = 2.0f;
constexpr float kLevelClearDelay = 2.5f;
constexpr int kFruitPoints = 100;
} // namespace

Game::Game()
    : window(sf::VideoMode({1280, 800}), "Pac-Man"), pacman(maze),
      titleText(font), ghosts(maze), pathfinding(maze) {
  layoutForWindow();
  loadFont();
  startLevel();
  sounds.playStart();
}

void Game::layoutForWindow() {
  const float mazeW = static_cast<float>(maze.getColumns() * Maze::TILE_SIZE);
  const float mazeH = static_cast<float>(maze.getRows() * Maze::TILE_SIZE);
  const auto size = window.getSize();
  const float scale = std::min(static_cast<float>(size.x) / mazeW,
                               static_cast<float>(size.y) / mazeH);

  mazeView.setSize(
      {static_cast<float>(size.x) / scale, static_cast<float>(size.y) / scale});
  mazeView.setCenter({mazeW * 0.5f, mazeH * 0.5f});
  window.setView(mazeView);

  const float rightEdge = static_cast<float>(size.x) - 40.f;
  titleText.setPosition({rightEdge, 24.f});
  if (scoreText) {
    scoreText->setPosition({rightEdge, 64.f});
  }
  if (highScoreText) {
    highScoreText->setPosition({rightEdge, 104.f});
  }
  if (livesText) {
    livesText->setPosition({40.f, 24.f});
  }
  if (levelText) {
    levelText->setPosition({40.f, 64.f});
  }
  if (statusText) {
    statusText->setPosition(
        {static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f});
  }
}

void Game::loadFont() {
  const auto path = findAsset("assets/fonts/PressStart2P-Regular.ttf");
  if (!path || !font.openFromFile(*path)) {
    std::cerr << "Failed to load PressStart2P-Regular.ttf\n";
    return;
  }

  titleText.setFont(font);
  titleText.setCharacterSize(22);
  titleText.setFillColor(sf::Color::Yellow);
  titleText.setString("PAC-MAN");

  const auto titleBounds = titleText.getLocalBounds();
  titleText.setOrigin({titleBounds.size.x, 0.f});

  scoreText.emplace(font, "Score: 0", 18);
  scoreText->setFillColor(sf::Color::White);
  scoreText->setOrigin({scoreText->getLocalBounds().size.x, 0.f});

  highScoreText.emplace(font, "High: 0", 18);
  highScoreText->setFillColor(sf::Color(180, 180, 180));
  highScoreText->setOrigin({highScoreText->getLocalBounds().size.x, 0.f});

  livesText.emplace(font, "Lives: 3", 18);
  livesText->setFillColor(sf::Color::White);

  levelText.emplace(font, "Level: 1", 18);
  levelText->setFillColor(sf::Color::White);

  statusText.emplace(font, "", 28);
  statusText->setFillColor(sf::Color::Yellow);
  statusText->setOrigin({0.f, 0.f});

  layoutForWindow();
  updateHud();
}

float Game::powerDurationForLevel() const {
  if (level <= 1) {
    return 6.f;
  }
  if (level == 2) {
    return 5.f;
  }
  if (level <= 4) {
    return 4.f;
  }
  if (level <= 6) {
    return 3.f;
  }
  if (level <= 8) {
    return 2.f;
  }
  if (level <= 10) {
    return 1.5f;
  }
  return 1.f;
}

void Game::elroyThresholdsForLevel(int &elroy1, int &elroy2) const {
  if (level <= 1) {
    elroy1 = 20;
    elroy2 = 10;
  } else if (level == 2) {
    elroy1 = 30;
    elroy2 = 15;
  } else if (level <= 5) {
    elroy1 = 40;
    elroy2 = 20;
  } else if (level <= 8) {
    elroy1 = 50;
    elroy2 = 30;
  } else {
    elroy1 = 60;
    elroy2 = 40;
  }
}

void Game::applyLevelTuning() {
  pacman.setPowerDuration(powerDurationForLevel());
  int elroy1 = 20;
  int elroy2 = 10;
  elroyThresholdsForLevel(elroy1, elroy2);
  ghosts.setElroyThresholds(elroy1, elroy2);
}

void Game::updateHud() {
  if (scoreText) {
    scoreText->setString("Score: " + std::to_string(score));
    scoreText->setOrigin({scoreText->getLocalBounds().size.x, 0.f});
  }
  if (highScoreText) {
    highScoreText->setString("High: " + std::to_string(highScore));
    highScoreText->setOrigin({highScoreText->getLocalBounds().size.x, 0.f});
  }
  if (livesText) {
    livesText->setString("Lives: " + std::to_string(lives));
  }
  if (levelText) {
    levelText->setString("Level: " + std::to_string(level));
  }

  if (statusText) {
    std::string status;
    switch (state) {
    case PlayState::Paused:
      status = "PAUSED";
      break;
    case PlayState::Dying:
      status = "READY!";
      break;
    case PlayState::LevelClear:
      status = "LEVEL CLEAR";
      break;
    case PlayState::GameOver:
      status = "GAME OVER";
      break;
    case PlayState::Playing:
      status.clear();
      break;
    }
    statusText->setString(status);
    const auto bounds = statusText->getLocalBounds();
    statusText->setOrigin({bounds.size.x * 0.5f, bounds.size.y * 0.5f});
  }

  layoutForWindow();
}

void Game::setScore(int value) {
  score = value;
  if (score > highScore) {
    highScore = score;
  }
  updateHud();
}

void Game::resetRound(bool fullLevelReset) {
  if (fullLevelReset) {
    maze.resetPellets();
    fruitSpawned = false;
    fruitVisible = false;
  }
  pacman.spawn(maze);
  ghosts.spawnAll();
  ghostEatStreak = 0;
  applyLevelTuning();
}

void Game::startLevel() {
  state = PlayState::Playing;
  stateTimer = 0.f;
  resetRound(true);
  updateHud();
}

void Game::restartGame() {
  score = 0;
  lives = 3;
  level = 1;
  startLevel();
  sounds.playStart();
}

void Game::handleDeath() {
  --lives;
  ghostEatStreak = 0;
  pacman.clearPower();
  if (lives <= 0) {
    state = PlayState::GameOver;
    stateTimer = 0.f;
  } else {
    state = PlayState::Dying;
    stateTimer = kDeathDelay;
    resetRound(false);
  }
  updateHud();
}

void Game::handleLevelClear() {
  ++level;
  startLevel();
  sounds.playStart();
}

void Game::run() {
  while (window.isOpen()) {
    processEvents();
    update();
    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
      continue;
    }
    if (event->is<sf::Event::Resized>()) {
      layoutForWindow();
      continue;
    }

    const auto *key = event->getIf<sf::Event::KeyPressed>();
    if (!key) {
      continue;
    }

    if (key->code == sf::Keyboard::Key::Escape) {
      if (state == PlayState::Playing) {
        state = PlayState::Paused;
      } else if (state == PlayState::Paused) {
        state = PlayState::Playing;
      } else if (state == PlayState::GameOver) {
        window.close();
      }
      updateHud();
    } else if (key->code == sf::Keyboard::Key::Enter ||
               key->code == sf::Keyboard::Key::Space) {
      if (state == PlayState::GameOver) {
        restartGame();
      }
    }
  }
}

void Game::update() {
  const float deltaTime = clock.restart().asSeconds();

  if (state == PlayState::Paused || state == PlayState::GameOver) {
    return;
  }

  if (state == PlayState::Dying) {
    stateTimer -= deltaTime;
    if (stateTimer <= 0.f) {
      state = PlayState::Playing;
      updateHud();
    }
    return;
  }

  if (state == PlayState::LevelClear) {
    stateTimer -= deltaTime;
    if (stateTimer <= 0.f) {
      handleLevelClear();
    }
    return;
  }

  pacman.handleInput();

  const PacmanUpdate pacmanUpdate = pacman.update(deltaTime);
  if (pacmanUpdate.atePellet) {
    setScore(score + 10);
    sounds.playChomp();
  }
  if (pacmanUpdate.ateBerry) {
    setScore(score + 50);
    sounds.playPower();
  }
  if (pacmanUpdate.powerStarted) {
    ghostEatStreak = 0;
    ghosts.enterFrightened();
  }
  if (pacmanUpdate.powerEnded) {
    ghostEatStreak = 0;
    ghosts.exitFrightened();
  }

  // Bonus fruit after ~70 pellets eaten.
  const int eaten = maze.totalPellets() - maze.remainingPellets();
  if (!fruitSpawned && eaten >= 70) {
    fruitSpawned = true;
    fruitVisible = true;
  }
  if (fruitVisible && pacman.getTile() == fruitTile) {
    fruitVisible = false;
    setScore(score + kFruitPoints);
  }

  const bool flash =
      pacman.isPowered() && pacman.powerRemaining() <= 2.f &&
      (static_cast<int>(pacman.powerRemaining() * 6.f) % 2 == 0);
  ghosts.setFrightenedFlash(flash);

  ghosts.update(deltaTime, pathfinding, pacman.getTile(), pacman.getDirection(),
                maze.remainingPellets(), pacman.isPowered());

  if (maze.remainingPellets() <= 0) {
    state = PlayState::LevelClear;
    stateTimer = kLevelClearDelay;
    pacman.clearPower();
    ghosts.exitFrightened();
    updateHud();
    return;
  }

  for (Ghost *ghost : ghosts.all()) {
    if ((ghost->getPosition() - pacman.getPosition()).length() >=
        Maze::TILE_SIZE * 0.5f) {
      continue;
    }

    if (ghost->getMode() == GhostMode::Frightened && pacman.isPowered()) {
      const int points = 200 << ghostEatStreak;
      ghostEatStreak = std::min(ghostEatStreak + 1, 3);
      setScore(score + points);
      ghost->eat();
      sounds.playEatGhost();
      break;
    }

    if (ghost->getMode() == GhostMode::Eaten ||
        ghost->getMode() == GhostMode::Waiting ||
        ghost->getMode() == GhostMode::Leaving) {
      continue;
    }

    sounds.playDeath();
    handleDeath();
    break;
  }
}

void Game::render() {
  window.clear(sf::Color::Black);

  window.setView(mazeView);
  maze.render(window);

  if (fruitVisible) {
    sf::CircleShape fruit(6.f);
    fruit.setOrigin({6.f, 6.f});
    fruit.setFillColor(sf::Color(255, 80, 80));
    fruit.setPosition(maze.getWorldPosition(fruitTile));
    window.draw(fruit);
  }

  if (state != PlayState::Dying && state != PlayState::GameOver) {
    pacman.render(window);
  }
  ghosts.render(window);

  window.setView(window.getDefaultView());
  window.draw(titleText);
  if (scoreText) {
    window.draw(*scoreText);
  }
  if (highScoreText) {
    window.draw(*highScoreText);
  }
  if (livesText) {
    window.draw(*livesText);
  }
  if (levelText) {
    window.draw(*levelText);
  }
  if (statusText && !statusText->getString().isEmpty()) {
    window.draw(*statusText);
  }
  window.display();
}
