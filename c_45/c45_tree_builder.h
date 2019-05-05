#ifndef C45_TREE_BUILDER_H
#define C45_TREE_BUILDER_H

#include "c45_tree.h"
#include <vector>
#include <map>
#include <utility> 

class C45_treeBuilder {

public:
 
    C45_tree buildTree(std::string fileName, int nClasses, int minSize);

private:

    C45_tree tree;
    std::map<long, std::vector<double>> data;
    std::vector<int> labels;
    int minSz;
    long sampleCount;

    void readCsv(std::string fileName);

    void sampleToData(std::vector<std::string> sample);

    void doSplits(C45_tree node);

    double getEntropy(std::map<long, std::vector<double>>  data);

    double getGain( std::map<long, std::vector<double>> top,
                    std::map<long, std::vector<double>> left,
                    std::map<long, std::vector<double>> right );

    std::vector<std::pair<double, long>> makeSortedVector(
                                                    std::map<long, std::vector<double>> data, 
                                                    int feature);

    std::pair < std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>> > splitData (  
                                                    std::map<long, std::vector<double>> data, 
                                                    int feature, 
                                                    double value);
                                                        
    int majority(C45_tree node);

    bool allSame(C45_tree node);
};

#endif











