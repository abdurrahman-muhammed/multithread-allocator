

#include "benchmark.h"

void benchmark1()
{
	const int iterations = 1'000'000;
	const std::size_t bytes = 4096;
	unsigned long long checksum = 0;

	std::vector<void*> ptrs(iterations);

	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < iterations; i++)
	{
		void* ptr = malloc(bytes);

		if (ptr == nullptr)
		{
			std::cout << "Memory allocation failed";
			return;
		}

		unsigned char* bytesPtr = static_cast<unsigned char*>(ptr);
		bytesPtr[0] = 42;

		checksum += bytesPtr[0];;

		free(ptr);
	}

	auto end = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed = end - start;
	//std::cout << "Iterations: " << iterations << "\n";
	//std::cout << "Allocation size: " << bytes << " bytes\n";
	std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
	//std::cout << "Pairs/sec: " << iterations / elapsed.count() << '\n';
	//std::cout << "Allocator operations/sec: " << (iterations * 2) / elapsed.count() << '\n';

	//std::cout << checksum << '\n';
}

void benchmark2()
{
	const int iterations = 1'000'000;
	const std::size_t bytes = 64;

	std::vector<void*> ptrs(iterations);

	auto allocateStart = std::chrono::steady_clock::now();

	for (int i = 0; i < iterations; i++)
	{
		ptrs[i] = malloc(bytes);

		if (ptrs[i] == nullptr)
		{
			std::cout << "Allocation failed at index " << i << '\n';

			for (int j = 0; j < i; j++)
			{
				free(ptrs[j]);
			}

			return;
		}
	}

	auto allocateEnd = std::chrono::steady_clock::now();

	auto freeStart = std::chrono::steady_clock::now();

	for (void* ptr : ptrs)
	{
		free(ptr);
	}

	auto freeEnd = std::chrono::steady_clock::now();


	std::chrono::duration<double> allocationElapsed = allocateEnd - allocateStart;
	std::chrono::duration<double> freeElapsed = freeEnd - freeStart;

	double allocationNs = allocationElapsed.count() * 1'000'000'000.0 / iterations;
	double freeNs = freeElapsed.count() * 1'000'000'000.0 / iterations;

	std::cout << "Allocation time: " << allocationElapsed.count() << " seconds\n";
	std::cout << "Free time: " << freeElapsed.count() << " seconds\n";
	std::cout << "Total time: " << (allocationElapsed + freeElapsed).count() << " seconds\n";

	std::cout << "Average allocation latency: " << allocationNs << "ns\n";
	std::cout << "Average free latency: " << freeNs << "ns\n";
}
