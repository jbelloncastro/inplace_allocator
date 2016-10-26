
#include "inplace_allocator.h"
#include <cstdint>
#include <iostream>
#include <set>
#include <algorithm>

int main( int argc, char* argv[] ) {
	uint8_t array[1024];
	inplace_allocator<int> alloc(array, 1024);

    std::set<int, std::less<int>, inplace_allocator<int> > values(alloc);
    for( int i = 0; i < 10; ++i ) {
        std::set<int>::iterator it = values.insert(i).first;
        std::cout << "set[" << i << "] = " << *it;
        std::cout << " @ unknown" << std::endl;
    }

    return 0;
}
