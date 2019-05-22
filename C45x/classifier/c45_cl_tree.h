#ifndef C45_CL_TREE
#define C45_CL_TREE

#include "c45_cl_node.h"
#include <map>

class C45_clTree {

public:

    C45_clTree();

    // creates the tree from file
    C45_clTree(std::string fName);

    // classifies an instance encoded as feature-name to value map
    std::string classify(std::map<std::string, double> instance);

private:

    C45_clNode root;

    std::map<int, C45_clNode> readTreeFile(std::string fName); 
};

#endif
