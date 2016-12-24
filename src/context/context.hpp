#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <stdint.h>

namespace Context {
bool SetupContext ();
void DestroyContext ();
void HandleEvents ();
void Draw (uint8_t displayBuffer[64 * 32]);
void Sleep (float ms);
bool ShouldQuit ();
}

#endif
