# OpenSyobonAction (Universal macOS Port)

## Description

This is a modern, universal (Apple Silicon + Intel) port of Open Syobon Action (しょぼんのアクション), a notoriously challenging platformer.  
This fork brings the game up to date with SDL2, supports both ARM64 and x86_64 Macs, and includes a number of codebase improvements and cleanups.

## Installation

### Prerequisites

You need the following libraries installed for both ARM64 and x86_64 architectures:

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- SDL2_gfx

#### On Apple Silicon (ARM64 Homebrew, default: `/opt/homebrew`):

```sh
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx
```

#### On Intel (x86_64 Homebrew, default: `/usr/local`):

If you have x86_64 Homebrew installed (under `/usr/local`), run:

```sh
arch -x86_64 /usr/local/bin/brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx
```

### Building

To build a universal binary (runs on both Apple Silicon and Intel Macs):

```sh
make clean
make
```

This will produce a `SyobonAction` binary that is universal.

### Running

```sh
./SyobonAction
```

#### Command-line options

- `-nosound` — disables sound

## Controls

- Arrow keys: Move
- Z or Up: Jump
- Space (hold): Run (double speed)
- O: Self-destruct
- ESC: Quit
- F1: Return to title
- On the title screen, press 1–9 to start at a specific stage, 0 for a random dungeon (may be buggy)

## Summary of Changes from the Original

- **SDL2 Port:** Migrated from SDL1 to SDL2 for modern compatibility.
- **Universal Binary:** The build system now produces a binary that runs natively on both ARM64 and x86_64 Macs.
- **Dependency Updates:** Uses SDL2_gfx for graphics primitives; all SDL2 dependencies are now required.
- **Makefile:** Updated to support universal builds and proper Homebrew paths for both architectures.
- **Code Modernization:** Numerous cleanups, bugfixes, and improved resource management.
- **Sound System:** Improved audio initialization and error handling.
- **Input Handling:** Modernized controller and keyboard input code.
