#include "c45_node.h"
#include <string>

long nodeCounter = 0;

C45_node::C45_node() {
    id = nodeCounter++;
}


bool C45_node::isRoot() {
    return parent == nullptr;
}

bool C45_node::isLeaf() {
    return left == nullptr && right == nullptr;
}

std::string C45_node::toString() {


    std::string parentId;
    std::string leftId;
    std::string rightId;
    std::string classif;

    if (parent == nullptr) {
        parentId = "_";
    }
    else {
        rightId = std::to_string(right->id);
    }

    if (left == nullptr) {
        leftId = "_";
    }
    else {
        leftId = std::to_string(left->id);
    }

    if (right == nullptr) {
        rightId = "_";
    }
    else {
        rightId = std::to_string(right->id);
    }

    if (classification == -1) {
        classif = "_";
    }
    else {
        classif = std::to_string(classification);
    }

    std::string s = std::to_string(id) + "," +
                    std::to_string(splitFeature) + "," +
                    std::to_string(splitValue) + "," +
                    parentId + "," +
                    leftId + "," +
                    rightId + "," +
                    classif;

    return s;
} 









