#pragma once

#include <Maze.h>
#include <SFML/Graphics.hpp>

class Ghost {
public:
  Ghost(const Maze &maze, sf::Color color);

  void spawn();
  void update(float deltaTime);
  void render(sf::RenderWindow &window);
  sf::Vector2i getTile() const;
  sf::Vector2f getPosition() const;

private:
  const Maze &maze;

  sf::Texture texture;
  sf::Sprite sprite;

  sf::Vector2f position;
  sf::Vector2f targetPosition;

  sf::Vector2i currentTile;
  sf::Vector2i targetTile;

  sf::Vector2i direction{0, 0};

  float speed = 100.0f;

  void loadTexture(sf::Color color);
  void chooseDirection();
};
