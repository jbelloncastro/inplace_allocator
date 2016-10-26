
#include "inplace_allocator.h"
#include "fallback_allocator.h"

#include <cstdint>
#include <iostream>
#include <set>

typedef fallback_allocator<int,std::allocator> allocator_t;
//typedef inplace_allocator<int> allocator_t;

int main( int argc, char* argv[] ) {
	uint8_t array[1024];
	allocator_t alloc(array, 1024);

    std::set<int, std::less<int>, allocator_t > values(alloc);
    for( int i = 0; i < 10; ++i ) {
        std::set<int>::iterator it = values.insert(i).first;
        std::cout << "set[" << i << "] = " << *it;
        std::cout << " @ unknown" << std::endl;
    }

    return 0;
}
