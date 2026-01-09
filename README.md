# Truth or Dare Discord Bot (C++ / D++)

A lightweight Discord bot written in modern C++ using the **D++ (DPP)** library.
This bot provides Truth-or-Dare gameplay via slash commands, with persistent
JSON-backed storage for custom prompts.

## Features

- '/truth' — Get a random truth prompt
- '/dare' — Get a random dare prompt
- '/random' — Get either a truth or a dare
- '/add' — Add new truth or dare prompts via slash command
- Persistent storage using JSON (survives restarts and persists across servers)
- Thread-safe prompt access

## Tech Stack

- **C++17**
- **D++ (DPP) v16**
- **CMake**
- **nlohmann/json**

## Project Structure
tod_bot/
├── commands/
│ ├── add.cpp
│ ├── truth.cpp
│ ├── dare.cpp
│ └── random.cpp
├── utils/
│ └── json_store.cpp
├── main.cpp
├── CMakeLists.txt
└── README.md


## Setup

### 1. Build

```bash
cmake -S . -B build
cmake --build build -j$(nproc)
```

### 2. Run
```bash
export DISCORD_TOKEN=your_bot_token_here
./build/truthbot
```

## Notes

- Prompt data is stored in data/prompts.json

- Designed for easy extension with new commands

## License

AGPL