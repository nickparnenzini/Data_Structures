#ifndef VECTOR_T_H
#define VECTOR_T_H

#include "allocator_t.h"
#include <iterator>

static const int MAX_SIZE = 100;

namespace custom_stl 
{

template < class T, class Allocator = allocator_t<T> >
class vector_t
{

public:
    typedef T                                                                  value_type;
    typedef Allocator                                                          allocator_type;
    typedef typename Allocator::reference                                      reference;
    typedef typename Allocator::const_reference                                const_reference;
    typedef typename Allocator::pointer                                        pointer;
    typedef std::iterator<std::random_access_iterator_tag, value_type>         iterator;
    typedef const std::iterator<std::random_access_iterator_tag, value_type>   const_iterator;
    typedef std::reverse_iterator<iterator>                                    reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                              const_reverse_iterator;
    typedef typename Allocator::size_alloc                                     size_type;

    vector_t() = default;

    vector_t(size_type n) {
        size_vector= n; 
        if ( n > MAX_SIZE) {
            capacity_vector = n;
        } 
        else {
            capacity_vector = MAX_SIZE;
        }    
        p = alloc.allocate(capacity_vector); 
    }

    vector_t(vector_t& x) {
        size_vector= x.size;
        capacity_vector = x.capacity_vector;
        if ( x.size > 0 ) {
            p = alloc.allocate(capacity_vector);
            for ( size_type i = 0; i < size_vector; i++ ) {
                alloc.construct(&p[i], x[i]);
            }
        }
        else {
            p = nullptr;
        }
    }

    ~vector_t() {
        if ( size_vector > 0 ) {
            for ( size_type i = 0; i < size_vector; i++ ) {
                alloc.destroy(&p[i]);
            }
            alloc.deallocate(p, size_vector);
            size_vector = 0;
            p = 0;
        }
    }

    vector_t& operator=(const vector_t&) {
        // TODO
    }

    iterator begin() {
        return &p[0];
    }

    iterator end() {
        iterator it = begin();
        for ( size_type i = 0; i < size_vector; i++ ) {
            ++it;
        }
        return it;
    }

    reverse_iterator rbegin() {
        return &p[size-1];
    }

    reverse_iterator rend() {
        iterator it = begin();
        return --it;
    }

    int size() const {
        return (int)size_vector;
    }

    size_type max_size() const {
        return capacity_vector;
    }

    void resize(size_type n, value_type val = value_type()) {
        if ( n < size_vector) {
            for(size_type i = n; i < size_vector-1; i++) {
                alloc.destroy(&p[i]); 
            }  
        }
        else if ( n > size_vector) {
            if ( n < capacity_vector ) {
                for(size_type i = size_vector; i < n; i++) {
                    alloc.construct(&p[i], val);
                }  
            }    
            else {
                capacity_vector = n; 
                pointer tmp = alloc.allocate(capacity_vector);
                for(size_type i = 0; i < size_vector; i++) {
                    alloc.construct(&tmp[i], p[i]);
                }   
                for(size_type i = size_vector; i < n; i++) {
                    alloc.construct(&tmp[i], val);
                }   
                // we have to free the memory previously allocated   
                for ( size_type i = 0; i < size_vector; i++ ) {
                    // using destroy is not sufficient to free memory
                    alloc.destroy(&p[i]); 
                }
                alloc.deallocate(p,size_vector);
                p = tmp;         
            }   
        }
        size_vector = n;
    }

    size_type capacity() const {
        return capacity_vector;
    }

    bool empty() const {
        return (size_vector> 0);
    }

    void reserve(size_type n) {
        if ( n > capacity_vector ) { 
            pointer buf = alloc.allocate(n);
            for (size_type i = 0; i < size_vector; i++) {
                alloc.construct(&buf[i], p[i]); 
            }
            /* free memory allocated previously */
            for (size_type i = 0; i < size_vector; i++) {
                alloc.destroy(&p[i]);
            }
            alloc.deallocate(p,capacity_vector);
            capacity_vector = n;
            p = buf;
        }  
    }

    reference operator[](size_type n) {
        // this method does not have to throw an exception 
        // in case the value n is out of range
        return p[n];    
    }

    reference at(size_type n) {
        if ( n < 0 || n >= size_vector) {
            throw std::out_of_range("Index is out of range");
        }
        return p[n];  
    }

    reference front() {
        return p[0];
    }
    reference back() {
        return p[size_vector-1];
    }

    void assign(size_type n, const value_type& val) {
        for ( size_type i = 0; i < size_vector; i++ ) {
            alloc.destroy(&p[i]);
        }
           
        if ( n <= capacity_vector ) {
            for ( size_type i = 0; i < size_vector; i++ ) {
                alloc.construct(&p[i], val);
            }   
        } 
        else if ( n > capacity_vector ) {
            alloc.deallocate(p, capacity_vector);
            capacity_vector = n;
            pointer tmp = alloc.allocate(capacity_vector);
            for ( size_type i = 0; i < size_vector; i++ ) {
                alloc.construct(&tmp[i], val);
            }
            p = tmp;         
        } 
        size_vector = n;
    }

    void push_back(const value_type& val) {
        if( ++size_vector >= capacity_vector ) {
            reserve(capacity_vector*2);       
        }
    }

    void pop_back() {
        alloc.destroy(&p[size_vector--]);
    }

    iterator insert(iterator position, const value_type& val) {
        if ( ++size_vector < capacity_vector ) {
            alloc.construct(&p[size_vector-1], p[size_vector-2]);
        }  
        else {
            resize(size_vector, p[size_vector-1]);
        }
        iterator it = end() - 1;
        for ( ; it != position; --it ) {
            *it = *(it-1);  
        }
        *position = val;
        return position; 
    }

    iterator erase(iterator position) {
        if ( position < begin() || position >= end() ) {
            throw std::out_of_range("Iterator position is out of range");
        }
        else {
            size_type pos = position - begin();
            alloc.destroy(&p[pos]);
            for ( size_type i = pos; i < size_vector- 1; i++ ) {
                p[i] = p[i+1];
            }      
            alloc.destroy(&p[size_vector-1]);
            size_vector--;
        }
        return position;
    }

    void swap(vector_t& x) {
       pointer tmp = x.p;
       x.p = p;
       p = tmp;
       size_type tmpSize = x.size;
       x.size_vector = size;
       size_vector = tmpSize;
       size_type tmpcapacity_vector = x.capacity_vector;
       x.capacity_vector = capacity_vector;
       capacity_vector = tmpcapacity_vector;
       allocator_type tmpAlloc = x.alloc;
       x.alloc = alloc;
       alloc = tmpAlloc;
    }

    void clear() {
        ~vector_t();
    }

    allocator_type get_allocator() const {
        return alloc;
    }

private:
    allocator_type alloc {};
    pointer p {nullptr};
    size_type size_vector {0};
    size_type capacity_vector {MAX_SIZE};
};

} // namespace custom_stl

#endif  // VECTOR_T_H