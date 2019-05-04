#ifndef C_45_TREE_H
#define C_45_TREE_H

#include <vector>
#include <map>
#include <iostream>

enum NodeType {
    ROOT, INNER, LEAF
};

enum Classification {
    SETOSA, VERSICOLOR, VIRGINICA
};

class C45_tree {

public:

    C45_tree() {
        type = ROOT;
    }

    void printType() {
        
        switch(type) {

            case ROOT:
                std::cout << "ROOT";
                break;

            case INNER:
                std::cout << "INNER";
                break;

            case LEAF:
                std::cout << "LEAF";
                break;
        }

        std::cout << std::endl;
    }

    NodeType type;
    int nFeatures;
    int splitFeature; // index
    double splitValue;
    C45_tree * parent;
    C45_tree * left;
    C45_tree * right;
    std::map<int, double> data;  
    std::map<int, double> dataLeft; 
    std::map<int, double> dataRight;  
};

#endif
