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

    std::string leftId;
    std::string rightId;
    std::string splitVal;

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

    if (isLeaf()) {
        splitVal = "_";
    }
    else {
        splitVal = std::to_string(splitValue);
    }

    std::string s = std::to_string(id) + "," +
                    splitFeature + "," +
                    splitVal + "," +
                    leftId + "," +
                    rightId + "," +
                    label;

    return s;
} 









