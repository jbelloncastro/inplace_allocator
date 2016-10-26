
#ifndef FALLBACK_ALLOCATOR_H
#define FALLBACK_ALLOCATOR_H

#include "inplace_allocator.h"
#include <type_traits>

template <typename Tp, typename Allocator = std::allocator<Tp> >
class fallback_allocator {
public:
    template <typename T, typename A>
    struct rebind {
        typedef fallback_allocator<T,A> other;
    };

	fallback_allocator( void* arena, size_t size, Allocator fallback  = Allocator() ) :
		_main(arena,size),
        _fallback(fallback),
        _begin(static_cast<uint8_t*>(arena)),
        _end(_begin+size)
	{
	}

    template <typename T, typename A>
    fallback_allocator( const fallback_allocator<T,A>& other ) :
        _main( other._main ),
        _fallback( other._fallback ),
        _begin( other._begin ),
        _end( other._end )
    {
    }

	Tp* allocate( std::size_t n, const std::nothrow_t& tag ) noexcept {
        Tp* result = _main.allocate(n,tag);
        if( !result ) {
           result = _fallback.allocate(n,tag);
        }
        return result;
	}

    Tp* allocate( std::size_t n ) {
        Tp* result = _main.allocate(n, std::nothrow);
        if( result ) {
            return result;
        } else {
            Tp* fallback_result = nullptr;
            fallback_result = _fallback.allocate(n);

            if( fallback_result ) {
                return fallback_result;
            } else {
                throw std::bad_alloc();
            }
        }
    }

	void deallocate( Tp* ptr, std::size_t n ) {
        if( fallback_allocated(ptr) ) {
            _fallback.deallocate(ptr,n);
        } else {
            _main.deallocate(ptr,n);
        }
	}

private:
    bool fallback_allocated( Tp* ptr ) {
        // std::less<T*> required for memory order comparisons
        std::less<uint8_t*> lessThan;
        uint8_t* p = static_cast<uint8_t*>((void*)ptr);
        bool result = lessThan(p,_begin) || (!lessThan(p,_end));
        return result;
    }

    inplace_allocator<Tp> _main;
    uint8_t*              _begin;
    uint8_t*              _end;
    Allocator             _fallback;
};

#endif // FALLBACK_ALLOCATOR_H

