#include "c45_cl_tree.h"
#include "c45_tester.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char * argv[]) {

    if (argc != 3) {

        std::cout << "missing argument: fileNames" << std::endl;
        abort();
    }

    std::string treeName = argv[1];
    std::string testName = argv[2];

    C45_clTree tree(treeName);
    C45_tester tester(testName);

    tree.printInfo();

    tester.runTest(tree);
    tester.printResult();
}
