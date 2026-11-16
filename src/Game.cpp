#include "Maze.h"
#include <Assets.h>
#include <Game.h>

#include <iostream>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Pac-Man", sf::State::Fullscreen),
      pacman(maze), titleText(font), ghosts(maze), pathfinding(maze) {
  maze.centerIn(window.getSize());
  pacman.spawn(maze);
  ghosts.spawnAll();
  loadFont();
  sounds.playStart();
}

void Game::loadFont() {
  const auto path = findAsset("assets/fonts/PressStart2P-Regular.ttf");
  if (!path || !font.openFromFile(*path)) {
    std::cerr << "Failed to load PressStart2P-Regular.ttf\n";
    return;
  }

  const float rightEdge = static_cast<float>(window.getSize().x) - 40.f;

  titleText.setFont(font);
  titleText.setCharacterSize(22);
  titleText.setFillColor(sf::Color::Yellow);
  titleText.setString("PAC-MAN");

  const auto titleBounds = titleText.getLocalBounds();
  titleText.setOrigin({titleBounds.size.x, 0.f});
  titleText.setPosition({rightEdge, 24.f});

  scoreText.emplace(font, "Score: 0", 28);
  scoreText->setFillColor(sf::Color::White);

  const auto scoreBounds = scoreText->getLocalBounds();
  scoreText->setOrigin({scoreBounds.size.x, 0.f});
  scoreText->setPosition({rightEdge, 64.f});
}

void Game::setScore(int value) {
  score = value;
  if (scoreText) {
    scoreText->setString("Score: " + std::to_string(score));
    const auto bounds = scoreText->getLocalBounds();
    scoreText->setOrigin({bounds.size.x, 0.f});
  }
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
    }
  }
}

void Game::update() {
  const float deltaTime = clock.restart().asSeconds();
  pacman.handleInput();

  const PacmanUpdate pacmanUpdate = pacman.update(deltaTime);
  if (pacmanUpdate.atePellet) {
    setScore(score + 10);
    sounds.playChomp();
  }
  if (pacmanUpdate.ateBerry) {
    sounds.playPower();
  }

  ghosts.update(deltaTime);

  for (Ghost *ghost : ghosts.all()) {
    if ((ghost->getPosition() - pacman.getPosition()).length() >=
        Maze::TILE_SIZE * 0.5f) {
      continue;
    }

    if (pacman.isPowered()) {
      setScore(score + 200);
      ghost->spawn();
      sounds.playEatGhost();
    } else {
      pacman.spawn(maze);
      setScore(0);
      sounds.playDeath();

      const sf::Vector2i pacmanSpawn = maze.getPacmanSpawn();
      const sf::Vector2i ghostTile = ghost->getTile();
      const int dx = ghostTile.x - pacmanSpawn.x;
      const int dy = ghostTile.y - pacmanSpawn.y;
      if (dx * dx + dy * dy <= 5 * 5) {
        ghost->spawn();
      }
    }
    break;
  }
}

void Game::render() {
  window.clear(sf::Color::Black);
  maze.render(window);
  pacman.render(window);
  ghosts.render(window);
  window.draw(titleText);
  if (scoreText) {
    window.draw(*scoreText);
  }
  window.display();
}
