#ifndef C_45_TREE_H
#define C_45_TREE_H

#include "c45_node.h"
#include "dataSet.h"
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <float.h>
#include <thread>
#include <mutex>

class SplitInfo {

    public:

    SplitInfo() {
        bestGain = -DBL_MAX;
    }

    void update(double gain, std::string feature, double value,
                DataSet &left, DataSet &right) {

        std::lock_guard<std::mutex> guard(update_mutex);

        if (gain > bestGain) {

            bestGain = gain;
            bestFeature = feature;
            bestValue = value;
            bestLeft = left;
            bestRight = right;
        }
    }

    std::string getFeature() {
        return bestFeature;
    }

    double getValue() {
        return bestValue;
    }

    double getGain() {
        return bestGain;
    }

    DataSet getLeft() {
        return bestLeft;
    }

    DataSet getRight() {
        return bestRight;
    }

    private:

    std::mutex update_mutex;

    double bestGain;
    std::string bestFeature;
    double bestValue;
    DataSet bestLeft;
    DataSet bestRight;  
};

class C45_tree {

public:

    C45_tree(   std::string csvName, 
                std::vector<std::string> featureNames, 
                std::vector<std::string> classes, 
                int minSize,
                int maxDepth,
                int threadsPerFeature);

    void saveTree(std::string outName);
    void clearTree();

    void printInfo();

private:

    // data members

    C45_node root;
    std::vector<C45_node *> nodes;
    std::vector<std::string> classes;
    std::vector<std::string> featureNames;

    int minSize;
    int maxDepth;
    int threadsPerFeature;

    // tree construction (recusrsively split nodes)
    void doSplit(C45_node * node);

    // aux

    double gain(DataSet &top,
                DataSet &left,
                DataSet &right);

    void applyBestSplit(C45_node * node);

    void findSplitValue(DataSet &data,
                        std::string &feture, 
                        SplitInfo * splitInfo,
                        int start,
                        int stop);

    void makeLeaf(C45_node * node);
};


#endif











