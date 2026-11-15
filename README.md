# Pac-Man (experiment)

A small C++ / SFML experiment — building a Pac-Man-style game from scratch to learn game loop structure, maze rendering, and (eventually) movement, ghosts, and collision.

This is a work-in-progress playground, not a finished clone.

## Status

- [x] Window + game loop
- [x] Classic-style maze (walls, pellets, power pellets)
- [ ] Player movement
- [ ] Ghost AI / pathfinding
- [ ] Collision, scoring, lives
- [ ] Sprites, sound, UI

## Requirements

- CMake 3.20+
- C++20 compiler
- [SFML 3](https://www.sfml-dev.org/) (`Graphics`, `Window`, `System`)

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

## Layout

```
include/   headers
src/       game sources
assets/    fonts, sounds, sprites (placeholders)
```

## License

Personal experiment — do what you want with it.
