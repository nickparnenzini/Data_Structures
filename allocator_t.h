#ifndef ALLOCATOR_T_H
#define ALLOCATOR_T_H

#include <limits>
#include <cstdlib>

#pragma once

namespace custom_stl 
{

template <class T1, class T2>
inline void Construct_Object(T1* p, const T2& value) {
    new ((void*) p) T1(value);
}

template <class T1>
inline void Construct_Object(T1* p) {
    new ((void*) p) T1();
}

template <class Tp>
inline void Destroy_Object(Tp* pointer) {
    pointer->~Tp();
}

template <class T>
class allocator_t 
{

public:
    // type definitions
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef std::size_t     size_alloc;
    typedef std::ptrdiff_t  difference_type;

    // rebind allocator to type U
    template <class U>
    struct rebind {
        typedef allocator_t<U> other;
    };

    allocator_t() throw() {}

    allocator_t(const allocator_t&) {}
    
    template<class U>
    allocator_t(const allocator_t<U>&) throw() {}

    ~allocator_t() throw() {}

    pointer address(reference value) const {
        return &value;
    }

    const_pointer address(const_reference value) const {
        return &value;
    }

    size_alloc max_size() const throw() {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    static pointer allocate(size_alloc num, const void* = 0) {
        pointer ret = (T*) malloc(num);
        return ret;
    }

    void construct(pointer p, const T& value) {
        new((void*)p)T(value);
    }

  	void destroy(pointer p) {
        p->~T();
    }

  	static void deallocate(pointer p, size_alloc num) {
        free((void*)p);
    }
};

template <class T, class Alloc>
class static_alloc
{
    typedef typename Alloc::size_alloc size;
    
public:

    static T* allocate(size n) {
        return (T*)Alloc::allocate(n*sizeof(T));
    }

    static void deallocate(T* p, size num) {
        Alloc::deallocate(p, num*sizeof(p));
    }

    static void deallocate(T* p) {
        Alloc::deallocate(p, sizeof(T));
    }
};

template <class T1, class T2>
bool operator== (const allocator_t<T1>&,
                const allocator_t<T2>&) throw() {
   return true;
}

template <class T1, class T2>
bool operator!= (const allocator_t<T1>&,
                const allocator_t<T2>&) throw() {
   return false;
}

}  // namespace custom_stl

#endif // ALLOCATOR_T_H