
#ifndef INPLACE_ALLOCATOR_H
#define INPLACE_ALLOCATOR_H

#include "alloc_node.h"
#include "debug.h"

#include <cassert>

template <typename Tp>
struct inplace_allocator {
    typedef Tp value_type;

    template <typename T>
    struct rebind { typedef inplace_allocator<T> other; };

	inplace_allocator(void* arena, size_t size) :
		_free(new (arena) free_node_base())
	{
        //LOG_FUNCTION;
        assert( size > sizeof(free_node_base) );

        size = size - sizeof(free_node_base) - sizeof(free_node);
        _free->insert_after( new(_free+1) free_node(size) );
	}

    template <typename T>
    inplace_allocator( const inplace_allocator<T>& other ) :
        _free( other._free )
    {
        //LOG_FUNCTION;
    }

	Tp* allocate( std::size_t n, const std::nothrow_t& tag ) noexcept {
        //LOG_FUNCTION;

		allocated_node* result = nullptr;
		free_node_base* current_node = _free;
		free_node* next_node = current_node->next();

		// Take in account node overheads
		// with special care of negative numbers
		const size_t requested = n * sizeof(Tp) + sizeof(allocated_node);
		const size_t necessary = std::max<long>(0, requested - sizeof(free_node));

		while (next_node && next_node->size() < necessary) {
			current_node = static_cast<free_node_base*>(next_node);
			next_node = next_node->next();
		}
		if (next_node) {
			// Found node
			if (next_node->size() > requested) {
				// Split when free space remains
				result = next_node->split(requested-sizeof(allocated_node));
			}
			else {
				// Replace when no space left on node
				current_node->remove_after();
				result = allocated_node::replace(next_node);
			}
		}
		return result? static_cast<Tp*>(result->data()) : nullptr;
	}

    Tp* allocate( std::size_t n ) {
        //LOG_FUNCTION;

        Tp* ptr = allocate(n, std::nothrow);
        if( !ptr )
            throw std::bad_alloc();
    }

	void deallocate( Tp* ptr, std::size_t n ) {
        //LOG_FUNCTION;

		std::less<free_node*> lowerThan;
		free_node* returned = free_node::replace(allocated_node::from_ptr(ptr));

		free_node* current = _free->next();
		if ( !current ) {
			_free->insert_after(returned);
		}
		else {
			free_node* next = current->next();
			// Must keep free list sorted
			while (next && lowerThan(next, returned)) {
				current = next;
				next = next->next();
			}
			current->try_join(returned);
		}
	}

private:
    template< typename T>
    friend struct inplace_allocator;

    free_node_base* _free;
};

#endif // INPLACE_ALLOCATOR_H

