#include "binary_search_tree.h"
#include "list_t.h"
#include <iostream>

int main() {

    // Example for using the data structure binary_search_tree
    custom_stl::binary_search_tree<int> bst;
    bst.insert_node(5);
    if (bst.find_node(5)) {
        std::cout << "BST properly implemented!" << std::endl;
    }

    custom_stl::list_t<int> list_example;

}