#include "benchmark.h"
#include "Arena.h"

int main()
{
	Arena arena(100);

	void* a = arena.allocate(20);
	void* b = arena.allocate(30);
	void* c = arena.allocate(60);

	auto A = reinterpret_cast<std::uintptr_t>(a);
	auto B = reinterpret_cast<std::uintptr_t>(b);
	
	std::cout << A << std::endl;
	std::cout << B << std::endl;
	std::cout << B - A << std::endl;
}