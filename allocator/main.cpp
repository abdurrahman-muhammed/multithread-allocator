#include "benchmark.h"
#include "Arena.h"

int main()
{
	Arena arena(100);

	void* a = arena.allocate(3);
	void* b = arena.allocate(7);
	void* c = arena.allocate(5);

	auto A = reinterpret_cast<std::uintptr_t>(a);
	auto B = reinterpret_cast<std::uintptr_t>(b);
	auto C = reinterpret_cast<std::uintptr_t>(c);
	
	std::cout << "A % 16: " << A % 16 << std::endl;
	std::cout << "B % 16: " << B % 16<< std::endl;
	std::cout << "C % 16: " << C % 16<< std::endl;
}