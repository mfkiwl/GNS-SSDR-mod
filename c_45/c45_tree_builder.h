#ifndef C45_TREE_BUILDER_H
#define C45_TREE_BUILDER_H

#include "c45_tree.h"
#include <vector>
#include <map>

class C45_treeBuilder {

public:
 
    C45_tree buildTree(std::string fileName);

private:

    C45_tree tree;
    std::vector<std::vector<double>> data;
    std::vector<Classification> labels;
    long sampleCount;

    void readCsv(std::string fileName);
    void sampleToData(std::vector<std::string> sample);
    int getSplitIndex(std::vector<double> data);
    double getSplitValue(int featureIdx);
};

#endif
