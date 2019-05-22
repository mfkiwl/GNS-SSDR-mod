#include "c45_cl_node.h"

C45_clNode::C45_clNode() {}

C45_clNode::C45_clNode(std::string feature, double value) 
    :   feature(feature), value(value)
{
    leaf = false;
}

C45_clNode::C45_clNode(std::string label) 
    : label(label)
{
    leaf = true;
}

bool C45_clNode::isLeaf() {
    return leaf;
}
