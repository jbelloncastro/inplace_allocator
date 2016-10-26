
#include "inplace_allocator.h"
#include "fallback_allocator.h"

#include <cstdint>
#include <iostream>
#include <vector>

typedef fallback_allocator<int,std::allocator> allocator_t;
//typedef inplace_allocator<int> allocator_t;

int main( int argc, char* argv[] ) {
	uint8_t array[100];
	allocator_t alloc(array, 100);

    std::vector<int,allocator_t> values(alloc);
    for( int i = 0; i < 20; ++i ) {
        values.push_back(i);
        std::cout << "vector[" << i << "] = " << values[i];
        std::cout << " @ " << std::hex << &values[i] << std::endl;
    }

    return 0;
}
