#include <iostream>
#include <stdint.h>

#include "chip8.hpp"
#include "context/context.hpp"
#include "game_loader.hpp"

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		std::cout << "*Error*: You didn't supply a ROM name\n" <<
		             "Usage example: ./build/chip8.out brix\n";
	}

	Chip8::Initialize();
	Context::SetupContext();
	Context::SetKeymap(Chip8::GetKeypadMemory());

	uint8_t* gameBuffer = nullptr;
	int gameBufferSize = 0;
	if (LoadGame(argv[1], &gameBuffer, gameBufferSize) == false) {
		std::cout << "I can't find this game, I'm so sorry :( \n";
		return 1;
	}
	Chip8::LoadGame(gameBuffer, gameBufferSize);
	delete gameBuffer;

	while (Context::ShouldQuit() == false) {
		Context::UpdateKeypad();
		Context::HandleEvents();
		Chip8::Cycle();

		if (Chip8::GetDrawFlag())
			Context::Draw(Chip8::GetDisplayMemory());

		Context::Sleep(1);
	}

	Context::DestroyContext();

	return 0;
}
