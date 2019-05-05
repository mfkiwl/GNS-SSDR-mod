#ifndef C_45_TREE_H
#define C_45_TREE_H

#include <vector>
#include <map>
#include <iostream>

enum NodeType {
    ROOT, INNER, LEAF
};

class C45_tree {

public:

    C45_tree() { type = ROOT; }

private:

    friend class C45_treeBuilder;

    int nClasses;
    int nFeatures;
    NodeType type;
    int splitFeature; // index
    double splitValue;
    C45_tree * parent = nullptr;
    C45_tree * left = nullptr;
    C45_tree * right = nullptr;
    std::map<long, std::vector<double>> data;  
  
    int classification = -1;
    bool isRoot() { return type == ROOT; }
    bool isLeaf() { return type == LEAF; }
};

#endif
