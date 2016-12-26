# Chip-8 Emulator
A chip-8 emulator written in C++11 for learning purposes. It runs most roms, some have bugs and some don't, I didn't think it's worth to spend so much time tracking down a bug in a small project like this.

## Dependencies
The emulator uses *SDL2* for input and window creation.

## Usage example
A makefile is present in the repository, to build go to the project's root directory using the terminal and type the following command:
```
$ make
```

To run the program, type the name of the executable followed by a rom's name, the program will look for the rom's name inside the roms directory, for example:
```
$ ./build/chip8.out pong
```

## Known bugs

-   In *invaders*, when an invader gets hit, other invaders disappear
