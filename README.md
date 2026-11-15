# Pac-Man (experiment)

A small C++ / SFML Pac-Man-style game built from scratch — maze, movement, ghosts, scoring, power-ups, and sound.

## Status

- [x] Fullscreen window + game loop
- [x] Classic-style maze (walls, pellets, berries)
- [x] Pac-Man movement, chomp animation, keyboard controls
- [x] Score + title HUD
- [x] Roaming ghost with sprite and house spawn
- [x] Collision (death / eat ghost when powered)
- [x] Berry power-up (faster, cyan sprites, eat ghost)
- [x] Sound effects (chomp, power, eat ghost, death, start)
- [ ] Ghost AI / pathfinding
- [ ] Lives / game over
- [ ] Multiple ghosts

## Requirements

- CMake 3.20+
- C++20 compiler
- [SFML 3](https://www.sfml-dev.org/) (`Graphics`, `Window`, `System`, `Audio`)

### macOS (Homebrew)

```bash
brew install sfml cmake
```

## Build & run

```bash
cmake -S . -B build
cmake --build build
./build/PacMan
```

Assets are copied into `build/` on every build.

## Controls

| Key | Action |
|-----|--------|
| Arrow keys | Move Pac-Man |
| Eat berry | Power up (~10s): faster, can eat the ghost |

## Layout

```
include/   headers
src/       game sources
assets/
  fonts/     Press Start 2P
  sounds/    chomp, power, eat_ghost, death, start
  sprites/   maze, Pac-Man, ghost
```

## License

Personal experiment — do what you want with it.
