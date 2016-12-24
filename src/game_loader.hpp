#ifndef GAME_LOADER_HPP
#define GAME_LOADER_HPP

#include <string>
#include <stdint.h>

bool LoadGame (std::string name, uint8_t** outBuffer, int& outSize);

#endif
