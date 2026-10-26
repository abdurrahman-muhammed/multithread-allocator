#include "benchmark.h"
#include "Arena.h"

int main()
{
	Arena arena(256);

	void* a = arena.allocate(64, 16);

	arena.free(a);

	void* b = arena.allocate(24, 16);
	void* c = arena.allocate(16, 16);

	std::cout << "a: " << a << '\n';
	std::cout << "b: " << b << '\n';
	std::cout << "c: " << c << '\n';

	std::cout << "a == b: " << (a == b) << '\n';
}