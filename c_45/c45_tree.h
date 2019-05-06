#ifndef C_45_TREE_H
#define C_45_TREE_H

#include <map>
#include <vector>
#include <utility>
#include <fstream>

class C45_tree {

public:

    C45_tree(int nClasses, int nFeatures, int minSize, std::string csvName);
    C45_tree(int nClasses, int nFeatures, int minSize);
    void buildTree(std::string outName);
    void printInfo();

private:

    long id;
    int nClasses;
    int nFeatures;
    long nSamples = 0;
    int minSize;
    int splitFeature = -1;
    double splitValue = -1;
    C45_tree * parent = nullptr;
    C45_tree * left = nullptr;
    C45_tree * right = nullptr;
    std::map<long, std::vector<double>> data;
    std::map<long, std::vector<double>> dataLeft;
    std::map<long, std::vector<double>> dataRight;
    std::vector<int> labels;
    int classification = -1;

    void readCsv(std::string csvName);
    bool isRoot();
    bool isLeaf();
    void sampleToData(std::vector<std::string> sample);

    double entropy(std::map<long, std::vector<double>> data, int feat);

    void setSplitFeature(C45_tree *node, std::map<long, std::vector<double>> data);

    void setSplitValue(C45_tree *node, std::map<long, std::vector<double>> data, int feature);

    std::pair<  std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>> > splitData(
                                    std::map<long, std::vector<double>> data, 
                                    int feat, 
                                    double threshold);

    std::vector<std::pair<double, long>> sortByValue(std::map<long, std::vector<double>> data, int feat);
            
    double gain(std::map<long, std::vector<double>> top,
                std::map<long, std::vector<double>> left,
                std::map<long, std::vector<double>> right,
                int feature);

    void doSplits(C45_tree *node, std::ofstream& out);

    std::string makeString();

    C45_tree makeNode(C45_tree *parent, std::map<long, std::vector<double>> data);

    C45_tree makeLeaf(C45_tree *parent, std::map<long, std::vector<double>> data);

    int majority(std::map<long, std::vector<double>> data);

    int allSame();
};

#endif











