#include "Arena.h"


Arena::Arena(std::size_t capactiy)
	: m_base(new std::byte[capactiy]), m_capacity(capactiy), m_offset(0)
{
	
}

Arena::~Arena()
{
	delete[] m_base;
}

void* Arena::allocate(std::size_t size)
{
	if (m_offset + size > m_capacity)
	{
		return nullptr;
	}

	std::byte* memory = m_base + m_offset;

	m_offset += size;

	return memory;
}