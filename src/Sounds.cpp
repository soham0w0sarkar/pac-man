#include <Assets.h>
#include <Sounds.h>

#include <iostream>

Sounds::Sounds()
    : chomp(chompBuffer), power(powerBuffer), eatGhost(eatGhostBuffer),
      death(deathBuffer), start(startBuffer) {
  loadBuffer(chompBuffer, "assets/sounds/chomp.wav");
  loadBuffer(powerBuffer, "assets/sounds/power.wav");
  loadBuffer(eatGhostBuffer, "assets/sounds/eat_ghost.wav");
  loadBuffer(deathBuffer, "assets/sounds/death.wav");
  loadBuffer(startBuffer, "assets/sounds/start.wav");
}

bool Sounds::loadBuffer(sf::SoundBuffer &buffer, const char *relative) {
  const auto path = findAsset(relative);
  if (!path || !buffer.loadFromFile(*path)) {
    std::cerr << "Failed to load " << relative << "\n";
    return false;
  }
  return true;
}

void Sounds::playChomp() { chomp.play(); }
void Sounds::playPower() { power.play(); }
void Sounds::playEatGhost() { eatGhost.play(); }
void Sounds::playDeath() { death.play(); }
void Sounds::playStart() { start.play(); }
