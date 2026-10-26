#include "benchmark.h"
#include "Arena.h"

int main()
{
	Arena arena(256);

	void* a = arena.allocate(64, 16);
	void* b = arena.allocate(32, 16);

	arena.free(a);

	void* c = arena.allocate(24, 16);

	std::cout << (a == c) << '\n';
}