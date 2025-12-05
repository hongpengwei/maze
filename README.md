Collecting workspace information# C++ Ray Casting Maze Game

A first-person maze game engine built with **C++ and SDL2**, implementing classic ray casting technology for 3D graphics rendering.

## Overview

This project recreates the raycasting technique popularized by games like Wolfenstein 3D. Players navigate through a maze from a first-person perspective, with the goal of reaching the white exit door.

## Features

- **Ray Casting Rendering**: Real-time 3D perspective using raycasting algorithm
- **First-Person Controls**:
  - `W` / `S`: Move forward / backward
  - `A` / `D`: Strafe left / right
  - Mouse: Look around (relative mouse mode)
  - `Shift`: Sprint (2x speed)
  - `ESC`: Toggle mouse lock
- **Collision Detection**: Prevents walking through walls
- **Multiple Rooms**: Navigate through color-coded chambers (green, blue, yellow)
- **Exit Detection**: Reach the white door to complete the maze
- **Performance**: FPS counter displayed in window title

## Project Structure

```
.
├── Constants.h       # Global constants and configuration
├── Player.h          # Player class header
├── Player.cpp        # Player movement, rotation, exit detection
├── RayCaster.h       # Ray casting engine header
├── RayCaster.cpp     # Ray casting core logic and maze map
└── main.cpp          # SDL initialization and main game loop
```

## Building

### Prerequisites
- C++11 or later
- SDL2 development libraries

### Compile
```bash
g++ -std=c++11 main.cpp Player.cpp RayCaster.cpp -o maze_game -lSDL2
```

### Run
```bash
./maze_game
```

## Game Mechanics

### Map Legend
| Tile | Color | Type |
|------|-------|------|
| 1 | Red | Boundary wall |
| 2 | Green | Room wall |
| 3 | Blue | Room wall |
| 4 | Yellow | Room wall |
| 6 | White | Exit door |
| 0 | — | Empty space |

### Objective
Navigate from the starting position through the interconnected rooms and reach the white exit door (marked as `6` on the map).

## Key Components

### `Player`
- Manages position, direction, and camera plane
- Handles movement with collision detection
- Supports mouse-based rotation and keyboard controls
- Detects when exit is reached

### `RayCaster`
- Implements the ray casting algorithm
- Renders 3D perspective to pixel buffer
- Applies distance-based shading for walls
- Manages texture updates to SDL

### `Constants`
- Configuration parameters (FOV, speeds, sensitivities)
- Screen dimensions and map size
- Color definitions for different wall types

## Technical Details

- **FOV**: 66 degrees (camera plane ratio: 0.66)
- **Map Size**: 24x24 grid
- **Collision Buffer**: Allows walking on empty (0) and exit (6) tiles
- **Rendering**: SDL2 texture-based pixel buffer updates
- **Frame Timing**: Delta time-based movement for frame-rate independence

## Controls Summary

| Action | Key(s) |
|--------|--------|
| Move Forward | W |
| Move Backward | S |
| Strafe Left | A |
| Strafe Right | D |
| Look Around | Mouse Movement |
| Sprint | Left Shift |
| Toggle Mouse Lock | ESC |
| Quit | Close Window |

## Future Enhancements

- Enemy AI pathfinding
- Multiple difficulty levels with different maze layouts
- Collectible items or power-ups
- Sound effects and music
- Texture mapping for walls
- Sprite rendering
- Level editor

## References

- Classic raycasting algorithm from Wolfenstein 3D era
- DDA (Digital Differential Analyzer) for grid traversal
- SDL2 for cross-platform graphics rendering

---

**Status**: Complete maze game with exit detection and player controls implemented.
