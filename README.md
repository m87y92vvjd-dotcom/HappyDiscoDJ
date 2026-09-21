# Happy Disco DJ

A cross-platform desktop music app built with JUCE and CMake.

## Features
- Modern JUCE 8 CMake-based project
- Windows and macOS desktop support
- MIDI input/output support
- DJ-style interface scaffold
- Ready for expansion with decks, library browsing, and audio engine

## Requirements
- CMake 3.22+
- C++17 compiler
- Git
- JUCE 8.0.4 (downloaded automatically via FetchContent by default)

## Build instructions

### Windows
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### macOS
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Project structure
```text
HappyDiscoDJ/
├── CMakeLists.txt
├── Source/
├── Resources/
├── README.md
└── build/
```

## Notes
This is a clean starting point for a DJ application. You can extend the `MainComponent`, `DeckComponent`, and `SplashComponent` classes to add audio playback, waveform displays, library browsing, and deck controls.

## Repository collaboration
This repository is intended for team development with pull requests and branches.
