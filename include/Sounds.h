#pragma once

#include <SFML/Audio.hpp>

class Sounds {
public:
  Sounds();

  void playChomp();
  void playPower();
  void playEatGhost();
  void playDeath();
  void playStart();

private:
  sf::SoundBuffer chompBuffer;
  sf::SoundBuffer powerBuffer;
  sf::SoundBuffer eatGhostBuffer;
  sf::SoundBuffer deathBuffer;
  sf::SoundBuffer startBuffer;

  sf::Sound chomp;
  sf::Sound power;
  sf::Sound eatGhost;
  sf::Sound death;
  sf::Sound start;

  bool loadBuffer(sf::SoundBuffer &buffer, const char *relative);
};
