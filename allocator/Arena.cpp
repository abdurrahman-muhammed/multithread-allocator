#include "Arena.h"

#include <new>
#include <iostream>
#include <algorithm>

Arena::Arena(std::size_t capactiy)
	: m_capacity(capactiy), m_offset(0), m_freeList(nullptr)
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
	if (alignment == 0 || (alignment & (alignment - 1)) != 0)
	{
		return nullptr;
	}

	BlockHeader* current = m_freeList;
	BlockHeader* previous = nullptr;

	while (current != nullptr)
	{
		std::byte* user = reinterpret_cast<std::byte*>(current) + sizeof(BlockHeader);
		auto address = reinterpret_cast<std::uintptr_t>(user);

		if (current->size >= size && address % alignment == 0)
		{
			break;
		}

		previous = current;
		current = current->nextFree;
	}

	if (current != nullptr)
	{
		if (previous == nullptr)
		{
			m_freeList = current->nextFree;
		}
		else
		{
			previous->nextFree = current->nextFree;
		}

		current->nextFree = nullptr;

		std::byte* userMemory = reinterpret_cast<std::byte*>(current) + sizeof(BlockHeader);
		std::byte* remainderAddress = userMemory + size;

		std::size_t headerAlignment = alignof(BlockHeader);
		std::size_t blockRemainder = reinterpret_cast<std::uintptr_t>(remainderAddress) % headerAlignment;
		std::size_t blockPadding = 0;

		if (blockRemainder != 0)
		{
			blockPadding = headerAlignment - blockRemainder;
		}

		if (current->size >=size + blockPadding + sizeof(BlockHeader) + MIN_PAYLOAD)
		{
			remainderAddress += blockPadding;

			BlockHeader* remainder = reinterpret_cast<BlockHeader*>(remainderAddress);

			remainder->size = current->size - size - sizeof(BlockHeader) - blockPadding;

			remainder->nextFree = m_freeList;
			m_freeList = remainder;

			current->size = size;
		}
		
		return userMemory;
	}

	std::byte* blockStart = m_base + m_offset;
	std::byte* userMemory = blockStart + sizeof(BlockHeader);

	std::size_t effectiveAlignment = std::max(alignment, alignof(BlockHeader));
	std::size_t remainder = reinterpret_cast<std::uintptr_t>(userMemory) % effectiveAlignment;

	std::size_t padding = 0;

	if (remainder != 0)
	{
		padding = effectiveAlignment - remainder;
	}

	std::byte* alignedUser = userMemory + padding;

	std::size_t total = sizeof(BlockHeader) + padding + size;

	if (total > m_capacity - m_offset)
	{
		return nullptr;
	}

	BlockHeader* header = reinterpret_cast<BlockHeader*>(alignedUser - sizeof(BlockHeader));
	header->size = size;
	header->nextFree = nullptr;

	m_offset += total;

	return alignedUser;
}

void Arena::free(void* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	std::byte* user = static_cast<std::byte*>(ptr);

	BlockHeader* header = reinterpret_cast<BlockHeader*>(user - sizeof(BlockHeader));
	
	header->nextFree = m_freeList;
	m_freeList = header;


	std::cout << "Freed block size: " << header->size << '\n';
}
