
#include "c45_tree.h"
#include <iostream>


int main(int argc, char * argv[]) {

    C45_tree t1(3, 4, 5, "iris/normalizedIris.data");

    t1.printInfo();
    
    t1.buildTree("test.tree");

    std::cout << std::endl << "done" << std::endl << std::endl;
}
