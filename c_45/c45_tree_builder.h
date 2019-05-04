#ifndef C45_TREE_BUILDER_H
#define C45_TREE_BUILDER_H

#include "c45_tree.h"
#include <vector>
#include <map>

class C45_treeBuilder {

public:
 
    C45_tree buildTree(std::string fileName, int nClasses, int maxDepth, int minSize);

private:

    C45_tree tree;
    std::map<long, std::vector<double>> data;
    std::vector<int> labels;
    int maxDpt;
    int minSz;
    long sampleCount;
    int currentSplitFeature;
    double currentSplitValue;


    void readCsv(std::string fileName);

    void sampleToData(std::vector<std::string> sample);

    void setSplit(std::map<long, std::vector<double>> data);

    void addNode();

    double getEntropy(std::vector<std::vector<double>> data);

    double getGain( std::map<long, std::vector<double>> top,
                    std::map<long, std::vector<double>> left,
                    std::map<long, std::vector<double>> right );

    std::vector<std::map<long, std::vector<double>>> makeSortedVector(
                                                    std::map<long, std::vector<double>> data, 
                                                    int feature);
};

#endif











