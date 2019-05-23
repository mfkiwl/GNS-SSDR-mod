#include "c45_cl_tree.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char * argv[]) {

    if (argc != 2) {

        std::cout << "missing argument: fileName" << std::endl;
        abort();
    }

    std::string fName = argv[1];

    C45_clTree tree(fName);

    tree.printInfo();
}
