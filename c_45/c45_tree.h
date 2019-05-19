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

    C45_tree(std::string csvName, int minSize);
    void saveTree(std::string outName);
    void printInfo();

private:

    // data members

    C45_node root;
    std::vector<C45_node> nodes;

    int minSize;

    // tree construction (recusrsively split nodes)
    void doSplit(C45_node * node);

    // aux

    double entropy(std::map<long, std::vector<double>> &data, int feat);

    double gain(std::map<long, std::vector<double>> &top,
                std::map<long, std::vector<double>> &left,
                std::map<long, std::vector<double>> &right,
                int feature);


    double bestValue(std::map<long, std::vector<double>> &data, int feature);

    void setParameters(C45_node * node);

    std::pair<  std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>> > splitData(
                                    std::map<long, std::vector<double>> &data, 
                                    int feat, double threshold);

    bool allSameClass(std::map<long, std::vector<double>> &data);

    int majority(std::map<long, std::vector<double>> &data);

    void makeLeaf(C45_node * node);
};


#endif











