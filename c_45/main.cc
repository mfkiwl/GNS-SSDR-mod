
#include "c45_tree.h"
#include <iostream>


int main(int argc, char * argv[]) {

    C45_tree t1(2, 4, 5, 100);

    t1.buildTree("../../data/csvFeaturesFiles/split/cs-ds1-2-3-4-7-balanced/cs-ds1-2-3-4-7-balanced_train.csv");
    t1.printInfo();

    std::cout << std::endl << "done" << std::endl << std::endl;
}
