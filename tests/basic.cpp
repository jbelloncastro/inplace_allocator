
#include "inplace_allocator.h"
#include "fallback_allocator.h"

#include <array>
#include <cstdint>
#include <iostream>

typedef fallback_allocator<int,std::allocator<int> > allocator;
//typedef inplace_allocator<int> allocator;

constexpr size_t size = 100;

int main(int argc, char* argv[]) {
	uint8_t array[size];
	allocator alloc(array, size);
    std::cout << "Arena: " << std::hex << static_cast<void*>(array)
              << " size: " << std::dec << size << std::endl;

	std::array<int*,10> chunks;
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i] = alloc.allocate(8);
        *chunks[i] = i;
		std::cout << "Allocated @ " << static_cast<void*>(chunks[i]) << std::endl;
	}
    std::cout << " End allocation " << std::endl;
	for (int i = 0; i < chunks.size(); ++i) {
		std::cout << " i: " << *chunks[i] << std::endl;
        alloc.deallocate(chunks[i], 8);
	}

	return 0;
}
