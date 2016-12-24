#include "game_loader.hpp"

#include <fstream>

const std::string path = "roms/";

bool LoadGame (std::string name, uint8_t** outBuffer, int& outSize) {
	std::string fullPath = path + name;
	std::ifstream file(fullPath, std::ios::binary | std::ios::ate);

	if (file.is_open() == false)
		return false;

	outSize = file.tellg();
	file.seekg(0);

	*outBuffer = new uint8_t[outSize];
	file.read((char*)*outBuffer, outSize);

	// printf("%i\n", outSize);
	// for (int i = 0; i < outSize; i += 2) {
	// 	printf("%02x%02x\n", (*outBuffer)[i], (*outBuffer)[i + 1]);
	// }

	if (file.good()) {
		file.close();
		return true;
	}
	else {
		delete outBuffer;
		file.close();
		return false;
	}
}
