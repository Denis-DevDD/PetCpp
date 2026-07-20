# PetCpp

A collection of C++ pet projects, ordered roughly by how they were written — from
early exercises in game logic to later, algorithm-heavy work with a strong focus
on testing and balance tuning.

## Projects

### Early — game logic

These are earlier projects, focused on modeling game rules, state and turn-based logic.

- **[CardMagic](CardMagic/)** — a turn-based, two-player trading-card game (à la
  *Magic: The Gathering* / *Hearthstone*), running entirely in the terminal.
- **[DND](DND/)** — a text-based, turn-based dungeon crawler loosely inspired by
  *Dungeons & Dragons*, with rooms, enemies, loot, items and equipment.

### Later — algorithms & balance testing

These are later projects, built around real algorithms and heavy testing of
simulation behavior and balance.

- **[NavMesh](NavMesh/)** — a from-scratch 2D navigation mesh library
  (triangulation, A*, string-pulling) with a raylib visualizer of the full pipeline.
- **[SciFiSim](SciFiSim/)** — a data-driven, deterministic sci-fi city-builder
  simulation with a headless core, hot-reloadable JSON content, and a headless
  balance runner for large-scale analysis.

## More info

Each project has its own `README.md` and `docs/` folder with full details —
features, build instructions, gameplay, screenshots and design notes.
