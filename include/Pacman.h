#pragma once

#include <Maze.h>
#include <SFML/Graphics.hpp>

class Pacman {
public:
  Pacman(Maze &maze);

  void spawn(Maze &maze);
  void handleInput();
  bool update(float deltaTime);
  void render(sf::RenderWindow &window);
  sf::Vector2i getTile() const;
  sf::Vector2f getPosition() const;

private:
  Maze &maze;

  sf::Texture openTexture;
  sf::Texture closedTexture;
  sf::Sprite sprite;

  sf::Vector2f position;
  sf::Vector2f targetPosition;

  sf::Vector2i currentTile;
  sf::Vector2i targetTile;

  sf::Vector2i direction{0, 0};
  sf::Vector2i requestedDirection{0, 0};

  float speed = 150.0f;
  float mouthTimer = 0.f;
  bool mouthOpen = true;

  void loadTextures();
  void updateSprite();
};
