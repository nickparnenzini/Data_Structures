#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "allocator_t.h"

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

template<typename T>
struct bst_node
{
    bst_node() {}

    bst_node(T const &t) :
        m_value(t)
    {
        m_left = m_right = nullptr;
    }

    T m_value;
    bst_node<T>* m_left {nullptr};
    bst_node<T>* m_right {nullptr};
};

template <class T, class Allocator = allocator_t<T> >
class binary_search_tree
{
public:

    binary_search_tree() = default;

    bool find_node(T value) {
        return search_node(m_root, value);  
    }

    void insert_node(T value) {
        insert_node_bst(m_root, value);
    }

    void delete_node(T value) {
        delete_node_bst(m_root, value);
    }

private:

    bool search_node(bst_node<T>* node, T value) {
        if (!node) 
            return false;

        if (node->m_value == value)
            return true;

        if (node->m_value > value) {
            return search_node(node->m_left, value);
        }
        else {
            return search_node(node->m_right, value);
        }
    }

    void insert_node_bst(bst_node<T>*& node, T value) {
        if (!node) {
            node = createNode(value);
            return;
        }
        if (node->m_value > value) {
            insert_node_bst(node->m_left, value);
        }
        else {
            insert_node_bst(node->m_right, value);
        }
    }

    void delete_node_bst(bst_node<T>* node, T value) {
        if (!node)
            return;

        if (node->m_value == value) {
            deleteNode(node);
        }

        if (node->m_value > value) {
            delete_node_bst(node->m_left, value);
        }
        else {
            delete_node_bst(node->m_right, value);
        }
    }

    typedef static_alloc<bst_node<T>, allocator_t<bst_node<T>>> Alloc_type;  

    bst_node<T>* createNode(const T& val) {
        bst_node<T>* tmp = getNode();
        Construct_Object(&tmp->m_value, val);
        return tmp;
    }

    void deleteNode(bst_node<T>* p) {
        Destroy_Object(&p->m_value); 
        putNode(p); 
    }

    // Method used to allocate a single node in memory.
    // However, this method does not construct the object inside the node.
    bst_node<T>* getNode() {
        return Alloc_type::allocate(1);
    }

    // This method deallocates memory for a sigle node
    void putNode(bst_node<T>* p) {
        Alloc_type::deallocate(p, 1);
    } 

    bst_node<T>* m_root;
};

}  // namespace custom_stl

#endif  // BINARY_SEARCH_TREE_H