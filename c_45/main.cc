
#include "c45_tree.h"
#include <iostream>


int main(int argc, char * argv[]) {

    C45_tree t1(2, 16, 5, 10);

    std::cout << std::endl << "Building tree..." << std::endl << std::endl;

    t1.buildTree("../../data/csvFeaturesFiles/split/cs-ds1-2-3-4-7-balanced/cs-ds1-2-3-4-7-balanced_train.csv");

    std::cout << std::endl;
    t1.printInfo();

    std::cout << std::endl << "done" << std::endl << std::endl;
}
