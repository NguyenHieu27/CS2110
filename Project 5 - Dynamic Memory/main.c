#include "bst.h"
#include "data_lib.h"

int main(void) {
    // Add your own test cases here to test your functions :)
    int * test_int = malloc(sizeof(int));
    *test_int = 4;
    Node * test_node = create_node((void *) test_int);
    destroy_node(test_node, 1);

    
    return 0;
}