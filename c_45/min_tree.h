#ifndef MIN_TREE_H
#define MIN_TREE_H

#include <vector>

enum MinType {
    ROOT, INNER, LEAF
};

class MinTree {

public:

    MinTree() { type = ROOT; }
    int predict(std::vector<double> sample);
    int traverse(MinTree * node, std::vector<double> sample);

    MinType type;
    int splitFeature;
    double splitValue;
    MinTree * left;
    MinTree * right;
    int classification;
};

#endif
