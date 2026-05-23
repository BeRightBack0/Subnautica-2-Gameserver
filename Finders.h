#pragma once

namespace Finders {
	uintptr_t FindEOSInitBase();
	uintptr_t FindEOSInitListen();
	uintptr_t FindNetDriverInitBase();
	uintptr_t FindNetDriverInitListen();
	uintptr_t FindCreateGameModeForURL();
	std::vector<uintptr_t> FindNullFuncs();
	uintptr_t FindLoadMap();
	std::vector<uintptr_t> FindNetModes();
	uintptr_t FindScuffness();
	uintptr_t FindProperium();
};