#ifndef CHIP8_HPP
#define CHIP8_HPP
#include <stdint.h>

namespace Chip8
{
void Initialize ();
void LoadGame (uint8_t* buffer, int size);
void Cycle ();

uint8_t* GetDisplayMemory();
bool     GetDrawFlag();
};


#endif
