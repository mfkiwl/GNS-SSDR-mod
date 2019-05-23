#include "c45_cl_tree.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

C45_clTree::C45_clTree() {}

C45_clTree::C45_clTree(std::string fName) {

    readTreeFile(fName);

    std::cout << std::endl << "classifier ready" << std::endl << std::endl;
}

std::string C45_clTree::classify(std::map<std::string, double> &instance) {

    std::string label = "no_label";

    C45_clNode node = root;

    while (! node.leaf) {

        if (instance[node.feature] < node.value) {
            node = nodeMap[node.left];
        }
        else {
            node = nodeMap[node.right];
        }
    }

    return node.label;
}

void C45_clTree::printInfo() {

    std::cout   << std::endl << "CLASSIFIER TREE" << std::endl
                << "n. features: " << features.size() << std::endl
                << "n. nodes: " << nodeMap.size() << std::endl
                << "features considered:" << std::endl;

    for (auto it = features.begin(); it != features.end(); ++it) {
        std::cout << "    " << *it << std::endl;
    } 

    std::cout << std::endl;
}

void C45_clTree::readTreeFile(std::string fName) {

    std::map<int, std::vector<std::string>> treeData;

    std::ifstream tFile(fName);

    if (! tFile.is_open()) {
        std::cout << "Error opening " << fName << std::endl;
        abort();
    }
    else {
        std::cout << "Opened " << fName << std::endl;
    }

    // store features in vector
    std::string line;
    std::getline(tFile, line);
    std::istringstream s(line);
    std::string field;

    while (std::getline(s, field, ',')) {
        features.push_back(field);        
    }

    while (std::getline(tFile, line)) {

        std::istringstream s(line);     
        std::vector<std::string> nodeFields;

        while (std::getline(s, field, ',')) {
            nodeFields.push_back(field);        
        }    

        int nodeId = std::stoi(nodeFields[0]);
        treeData[nodeId] = nodeFields;  
    }

    tFile.close();

    std::cout << "Closed " << fName << std::endl;

    for (auto it = treeData.begin(); it != treeData.end(); ++it) {
        C45_clNode n = makeNode(it->second);
        nodeMap[n.id] = n;
    }   

    root = nodeMap[0];
}

C45_clNode C45_clTree::makeNode(std::vector<std::string> nodeData) {

    C45_clNode node;

    node.id = std::stoi(nodeData[0]);

    if (nodeData[1] == "_") {

        node.leaf = true;
        node.label = nodeData[5];
    }
    else {

        node.leaf = false;
        node.feature = nodeData[1];
        node.value = std::stod(nodeData[2]);
        node.left = std::stoi(nodeData[3]);
        node.right = std::stoi(nodeData[4]);
    }

    return node;
}





