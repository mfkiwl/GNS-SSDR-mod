#ifndef C_45_TREE_H
#define C_45_TREE_H

#include "c45_node.h"
#include <map>
#include <vector>
#include <utility>
#include <fstream>

class C45_tree {

public:

    C45_tree(int nClasses, int nFeatures, int minSize, int maxDepth);
    void buildTree(std::string csvName);
//    void saveTree(std::string outName);
    void printInfo();

private:

    // data members

    C45_node root;
    std::vector<C45_node> nodes;

    int nClasses;
    int nFeatures;
    long nSamples = 0;

    int minSize;
    int maxDepth;
    int currentDepth = 0;

    std::vector<int> labels;

    // methods

    void readCsv(std::string csvName);
    void sampleToData(std::vector<std::string> &sample);
};


#endif











