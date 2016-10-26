
#include "inplace_allocator.h"
#include <cstdint>
#include <iostream>
#include <vector>

int main( int argc, char* argv[] ) {
	uint8_t array[500];
	inplace_allocator<int> alloc(array, 500);

    std::vector<int,inplace_allocator<int> > values(alloc);
    for( int i = 0; i < 20; ++i ) {
        values.push_back(i);
        std::cout << "vector[" << i << "] = " << values[i];
        std::cout << " @ " << std::hex << &values[i] << std::endl;
    }

    return 0;
}
