#ifndef C_45_NODE_H
#define C_45_NODE_H

#include "dataSet.h"
#include <vector>
#include <map>

class C45_node {

public:
    
    C45_node();


private:

    friend class C45_tree;

    long id;
    bool leaf;
    std::string splitFeature = "_";
    double splitValue = -1;
    double gain = 0;
    C45_node * parent = nullptr;
    C45_node * left = nullptr;
    C45_node * right = nullptr;
    int depth = 0;
    std::string label = "_";
    DataSet data;
    std::string majority;

    bool isRoot();
    bool isLeaf();
    std::string toString();
};

#endif
