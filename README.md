# ChessBot (Paused Development Branch)

## About This Project

ChessBot is a personal project aimed at building a fully playable chess engine in C++ using bitboards, object-oriented design, and efficient move generation techniques. The project includes core classes like `Board`, `Game`, `MoveInformation`, `MoveGenerator`, and `MoveParser`, with a focus on both clean architecture and performance.

## Project Paused — Why?

After rewriting and reorganizing the original project to improve clarity and remove tight coupling between classes, I hit a wall.

While I successfully improved structure and introduced better coding practices, I found myself spending **more time fixing structural and linking errors** than working on actual chess logic. The project turned into a cleanup job rather than a learning or creative experience.

My main goal with this project was to have something to code outside of class, and I succeeded at this. This chess project has been the beginning to a journey of improvement and hard work as a coding professional.

Rather than continue to grind against code debt, I've decided to **pause development** on this branch (`fresh-start`). This doesn't mean the project is abandoned — just deferred until I have:
- A cleaner design approach
- More time to refactor from scratch
- Possibly external help or mentorship to resolve architectural blockers

## Branches

- `main` — Original version where I hit the wall
- `fresh-start` — Refactor branch attempting to restructure
- `mostly-working-game` — Peak functionality of a base, 2 player game

## What I Learned
- Planning ahead and the importance of a strong understanding of the project infrastructure
- The importance of clean class boundaries and modular design
- How circular dependencies in C++ can break compilation
- Bitboard representations and chess move encoding
- Why starting small and testing early is essential --> The value of debugger infrastructure

## Future Plans

I plan to return to this project after a break or after gaining more experience. In the meantime, I may:
- Start smaller scoped games or visualizers
- Re-implement core ideas in Python or JavaScript
- Use this experience to fuel better project planning

## Current Status

**Paused** as of 6/27
`fresh-start` branch preserved for future resumption  
Last major focus: Decoupling `MoveGenerator` and fixing interdependencies

---

Thanks for checking out this project. If you're a fellow developer who sees potential or wants to collaborate, feel free to reach out.
