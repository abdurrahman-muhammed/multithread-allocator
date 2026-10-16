#include "Arena.h"

#include <new>

Arena::Arena(std::size_t capactiy)
	: m_capacity(capactiy), m_offset(0)
{
	 void* memory = VirtualAlloc(nullptr, capactiy, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	 if (memory == nullptr)
	 {
		 throw std::bad_alloc();
	 }

	 m_base = static_cast<std::byte*>(memory);
}

Arena::~Arena()
{
	VirtualFree(m_base, 0, MEM_RELEASE);
}

void* Arena::allocate(std::size_t size, std::size_t alignment)
{
	std::byte* memory = m_base + m_offset;

	std::size_t remainder = reinterpret_cast<std::uintptr_t>(memory) % alignment;
	std::size_t padding = 0;

	if (remainder != 0)
	{
		padding = alignment - remainder;
	}

	if (m_offset + size + padding> m_capacity)
	{
		return nullptr;
	}

	std::byte* alignedMemory = memory + padding;

	m_offset += size + padding;

	return alignedMemory;
}