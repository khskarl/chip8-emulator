#ifndef CHIP8_HPP
#define CHIP8_HPP
#include <stdint.h>

namespace Chip8
{
uint8_t* GetDisplayMemory();
uint8_t* GetKeypadMemory();
bool     GetDrawFlag();

void Initialize ();
void LoadGame (uint8_t* buffer, int size);
void Cycle ();

};

#endif
