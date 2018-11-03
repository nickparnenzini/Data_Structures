#ifndef LIST_T_H
#define LIST_T_H

#include "allocator_t.h"
#include <iterator>
#include <stddef.h>
#include <iostream>

namespace custom_stl 
{

template<typename T>
struct list_node
{
    list_node(){}

    list_node(T const &t) :
        m_value(t)
    {
        m_prev = m_next = nullptr;
    }

    T m_value;
    list_node<T>* m_prev {nullptr};
    list_node<T>* m_next {nullptr};
};

template < class T>
class list_t_iterator
{
public:

    list_t_iterator(list_node<T>* pNode) :
                  m_currentNode(pNode)
    {}

    list_t_iterator& operator=(list_node<T>* pNode) {
        m_currentNode = pNode;
        return *this;
    }

    list_t_iterator& operator++() {
        if (this->m_currentNode != nullptr) {
            m_currentNode = m_currentNode->m_next;
        }
        return *this;
    }

    list_t_iterator operator++(int) {
        list_t_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    list_t_iterator& operator--() {  
        if (this->m_currentNode != nullptr) {
            this->m_currentNode = this->m_currentNode->m_prev;
        }
        return *this;
    }

    list_t_iterator operator--(int) {  
        list_t_iterator tmp = *this;
        if (this->m_currentNode != nullptr) {
            this->m_currentNode = this->m_currentNode->m_prev;
        }
        return tmp;
    }

    T operator*() {
        return this->m_currentNode->m_value;
    }

    bool operator!=(const list_t_iterator& iterator) {  
        return this->m_currentNode != iterator.m_currentNode;
    }

    bool operator==(const list_t_iterator& iterator) {  
        if (this->m_currentNode == nullptr)
            return false;

        return this->m_currentNode == iterator.m_currentNode;
    }
    
    list_node<T>* m_currentNode;
};

template < class T, class Allocator = allocator_t<T> >
class list_t
{
    friend class list_terator;

public:

    typedef T                                     value_type;
    typedef list_node<T>                          Node;
    typedef typename Allocator::pointer           pointer;
    typedef typename Allocator::const_pointer     const_pointer;
    typedef typename Allocator::reference         reference;
    typedef typename Allocator::const_reference   const_reference;
    typedef list_t_iterator<T>                    iterator;    
    typedef const list_t_iterator<T>              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename Allocator::size_alloc        size_type;
    typedef std::ptrdiff_t                        difference_type;

    list_t() {

    }

    list_t(size_type n, const value_type& val = value_type()) {
        head = createNode(val);  
        head->m_next = head->m_prev = nullptr;
        tail = head; 
        size_list = 1;  
        Node* p  = head;
        Node* q  = nullptr;
        while (n > 1) {  
            Node* tmp = createNode(val);
            p->m_next = tmp;
            tmp->m_prev = p;
            tmp->m_next = nullptr;
            q = p;
            p = p->m_next;
            n--;
            size_list++;
            tail = p; 
        } 
    }

    ~list_t() {
        clear();   
    }

    // True if the list is empty
    bool empty() const noexcept {
        return size_list == 0;
    }

    int size() {
        return (int)size_list;
    }

    iterator begin() {
        return iterator(head);  
    }

    iterator end() { 
        return iterator(nullptr); 
    }

    // TODO
    iterator rbegin() {
        //return reverse_iterator(end());
    }

    // TODO
    iterator rend() {
        //return reverse_iterator(begin());
    }

    reference front() {
        return head->m_value;
    }

    reference back() {
        return tail->m_value;
    }

    void push_front(const value_type& val) {
        Node* tmp = createNode(val);
        if (head == nullptr) {
            head = tail = tmp;
        }
        else {
            head->m_prev = tmp;
            tmp->m_next = head;
            head = tmp;
        }
        size_list++;
    }

    void pop_front() {
        if (head != nullptr) {  
            Node* tmp = head;   
            if (head->m_next != nullptr) { 
                head = head->m_next;     
            }
            head->m_prev = nullptr;  
            deleteNode(tmp);   
            if (head == nullptr) {   
                tail = nullptr;  
            }  
            size_list--;  
        }
    }

    void push_back(const value_type& val) {
        Node* tmp = createNode(val);
        if (tail == nullptr) {
            head = tail = tmp;
        }
        else {
            tail->m_next = tmp;          
            tmp->m_prev = tail;
            tail = tmp;
        }
        size_list++;
    }

    void pop_back() {
        if (tail != nullptr) {  std::cout << "Code executed!" << std::endl;
            Node* tmp = tail;
            tail = tail->m_prev;
            if (tail != nullptr) {
                tail->m_next = nullptr;
            }
            deleteNode(tmp);
            if (tail == nullptr) {
                head = nullptr;
            }
            size_list--;
        }
    }

    iterator insert(iterator position, const value_type& val) {
        Node* tmp = createNode(val);
        if (position != end()) {
            tmp->m_next = position.m_currentNode;
            tmp->m_prev = position.m_currentNode->m_prev;
            Node* previous = position.m_currentNode->m_prev;
            if (previous != nullptr) {
                previous->m_next = tmp;
            }
            position.m_currentNode->m_prev = tmp;
            if (position == head) {
                head = tmp;
            }
        }
        else {
            tail->m_next = tmp;
            tmp->m_prev = tail;
            tail = tmp;
        }
        size_list++;
        return tmp;
    }

    iterator erase(iterator position) {
        if (position == begin()) {
            Node* tmp = head;
            head = head->m_next;
            head->m_prev = nullptr;
            if (head == nullptr) {
                tail = nullptr;
            }
            deleteNode(tmp);
            return head;
        }
        else {
            Node* previous = position.m_currentNode->m_prev;
            previous->m_next = position.m_currentNode->m_next;
            if (position.m_currentNode->m_next != nullptr) {
                position.m_currentNode->m_next->m_prev = previous;
            }
            if (position == tail) {
                tail = previous;    
            }
            deleteNode(position.m_currentNode);
            return previous->m_next;
        }   
    }

    void clear() {  
        Node* current = head;
        Node* next;
        while (current != nullptr) { 
            next = current->m_next;
            deleteNode(current);
            current = next;
        } 
        size_list = 0;  
        tail = head = nullptr;  
    }

private:

    typedef static_alloc<list_node<T>, allocator_t<list_node<T>>> Alloc_type;

    Node* createNode(const value_type& val) {
        Node* tmp = getNode();
        Construct_Object(&tmp->m_value, val);
        return tmp;
    }

    void deleteNode(Node* p) {
        Destroy_Object(&p->m_value); 
        putNode(p); 
    }

    // Method used to allocate a single node in memory.
    // However, this method does not construct the object inside the node.
    Node* getNode() {
        return Alloc_type::allocate(1);
    }

    // This method deallocates memory for a sigle node
    void putNode(list_node<T>* p) {
        Alloc_type::deallocate(p, 1);
    }

    Node* head {nullptr};
    Node* tail {nullptr};
    size_type size_list {0};
};

}  // namespace custom_stl

#endif  // LIST_T_H