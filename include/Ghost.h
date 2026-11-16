#pragma once

#include <Maze.h>
#include <SFML/Graphics.hpp>

enum class GhostId { Blinky, Pinky, Inky, Clyde };

enum class GhostMode { Chase, Scatter, Frightened, Eaten, Waiting, Leaving };

class Pathfinding;

class Ghost {
public:
  Ghost(const Maze &maze, GhostId id, sf::Color color);

  GhostId getId() const;
  GhostMode getMode() const;
  void setMode(GhostMode mode);
  void setResumeMode(GhostMode mode);
  void setCruiseElroy(bool elroy, bool elroy2);
  void setFrightenedFlash(bool flash);
  void setLeaveAsFrightened(bool value);
  bool hasLeftHouse() const;
  void spawn();
  void eat();
  void release(bool asFrightened = false);
  void clearFrightenedLeave();
  void update(float deltaTime, Pathfinding &pathfinding, sf::Vector2i pacmanTile,
              sf::Vector2i pacmanDirection, sf::Vector2i blinkyTile,
              bool pacmanPowered);
  void render(sf::RenderWindow &window);
  sf::Vector2i getTile() const;
  sf::Vector2f getPosition() const;

private:
  const Maze &maze;
  GhostId id;
  GhostMode mode = GhostMode::Waiting;
  GhostMode resumeMode = GhostMode::Scatter;
  bool leaveAsFrightened = false;
  bool cruiseElroy = false;
  bool cruiseElroy2 = false;
  bool frightenedFlash = false;
  sf::Color color;

  sf::Texture texture;
  sf::Texture frightenedTexture;
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
  void chooseDirection(Pathfinding &pathfinding, sf::Vector2i pacmanTile,
                       sf::Vector2i pacmanDirection, sf::Vector2i blinkyTile,
                       bool pacmanPowered);
  void chooseLeavingDirection(Pathfinding &pathfinding);
  void chooseEatenDirection(Pathfinding &pathfinding, bool pacmanPowered);
  void chooseFrightenedDirection();
  void reverseDirection();
  sf::Vector2i spawnTile() const;
  sf::Vector2i exitTile() const;
  void syncSprites();
  float currentSpeed() const;
  bool houseAccess() const;
  sf::Vector2i nextTileAlong(sf::Vector2i dir) const;
  bool isOpenDir(sf::Vector2i dir) const;
};
