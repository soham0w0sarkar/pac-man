#pragma once

#include <Maze.h>
#include <SFML/Graphics.hpp>

enum class GhostId { Blinky, Pinky, Inky, Clyde };

class Ghost {
public:
  Ghost(const Maze &maze, GhostId id, sf::Color color);

  GhostId getId() const;
  void spawn();
  void update(float deltaTime);
  void render(sf::RenderWindow &window);
  sf::Vector2i getTile() const;
  sf::Vector2f getPosition() const;

private:
  const Maze &maze;
  GhostId id;
  sf::Color color;

  sf::Texture texture;
  sf::Texture eyesTexture;
  sf::Sprite sprite;
  sf::Sprite eyesSprite;

  sf::Vector2f position;
  sf::Vector2f targetPosition;

  sf::Vector2i currentTile;
  sf::Vector2i targetTile;

  sf::Vector2i direction{0, 0};

  float speed = 100.0f;

  void loadTexture();
  void chooseDirection();
  sf::Vector2i spawnTile() const;
  void syncSprites();
};
