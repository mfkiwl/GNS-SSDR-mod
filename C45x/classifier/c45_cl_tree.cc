#include "c45_cl_tree.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

C45_clTree::C45_clTree() {}

C45_clTree::C45_clTree(std::string fName) {

    std::map<int, C45_clNode> nodes = readTreeFile(fName);

    std::cout << std::endl << "classifier ready" << std::endl << std::endl;
}

std::string C45_clTree::classify(std::map<std::string, double> instance) {

    std::string label = "no_label";

    return label;
}

std::map<int, C45_clNode> C45_clTree::readTreeFile(std::string fName) {

    std::map<int, C45_clNode> nodes;

    std::ifstream tFile(fName);

    if (! tFile.is_open()) {
        std::cout << "Error opening " << fName << std::endl;
        abort();
    }
    else {
        std::cout << "Opened " << fName << std::endl;
    }

    std::string line;

    // skip header
    std::getline(tFile, line)

    while (std::getline(tFile, line)) {

        std::istringstream s(line); 
        std::string field;      
        std::vector<std::string> nodeFields;

        while (std::getline(s, field, ',')) {
            nodeFields.push_back(field);        
        }      
    }

    tFile.close();

    std::cout << "Closed " << fName << std::endl;

    return nodes;
}







