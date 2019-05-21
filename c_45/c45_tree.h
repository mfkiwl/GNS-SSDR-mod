#ifndef C_45_TREE_H
#define C_45_TREE_H

#include "c45_node.h"
#include "dataSet.h"
#include <map>
#include <vector>
#include <utility>
#include <fstream>

class C45_tree {

public:

    C45_tree(   std::string csvName, 
                std::vector<std::string> featureNames, 
                std::vector<std::string> classes, 
                int minSize);

    void saveTree(std::string outName);
    void printInfo();

private:

    // data members

    C45_node root;
    std::vector<C45_node> nodes;
    std::vector<std::string> classes;

    int minSize;

    // tree construction (recusrsively split nodes)
    void doSplit(C45_node * node);

    // aux

    double gain(DataSet &top,
                DataSet &left,
                DataSet &right);

    void applyBestSplit(C45_node * node);

    void findSplitValue(DataSet &data,
                        std::string &feture, 
                        double &bestGain,
                        std::string &bestFeature,
                        double &bestValue,
                        DataSet &bestLeft,
                        DataSet &bestRight);

    void makeLeaf(C45_node * node);
};


#endif











