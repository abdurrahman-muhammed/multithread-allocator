#pragma once
#include <cstddef>

class Arena
{
public:
	Arena(std::size_t capactiy);
	~Arena();

	void* allocate(std::size_t size);

	Arena(const Arena&) = delete;
	Arena& operator=(const Arena&) = delete;

private:
	std::byte* m_base;
	std::size_t m_capacity;
	std::size_t m_offset;
};