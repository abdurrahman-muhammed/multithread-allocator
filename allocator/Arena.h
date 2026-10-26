#pragma once
#include <cstddef>
#define NOMINMAX
#include <Windows.h>

struct BlockHeader
{
	std::size_t size;
	BlockHeader* nextFree;
};

class Arena
{
public:
	Arena(std::size_t capactiy);
	~Arena();

	void* allocate(std::size_t size, std::size_t allignment = 16);
	void free(void* ptr);

	Arena(const Arena&) = delete;
	Arena& operator=(const Arena&) = delete;

private:
	std::byte* m_base;
	std::size_t m_capacity;
	std::size_t m_offset;
	BlockHeader* m_freeList;

	static constexpr std::size_t MIN_PAYLOAD = 16;
};