
#include "c45_tree_builder.h"
#include "c45_tree.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <utility> 
#include <algorithm>
#include <float.h>
#include <iostream>

long global_id = 0;

void C45_treeBuilder::buildTree(std::string csvName, 
                                std::string outName, 
                                int nClasses, 
                                int minSize) {

    minSz = minSize;
    tree.nClasses = nClasses;

    readCsv(csvName);

    std::ofstream out(outName);

    if (! out.is_open()) {
        std::cout << "Error opening out file" << std::endl;
        return;
    }

    doSplits(tree, out);

    out.close();
}

void C45_treeBuilder::readCsv(std::string fName) {

    std::ifstream csv;
    csv.open(fName.c_str());

    if (! csv.is_open()) {
        std::cout << "Error opening " << fName << std::endl;
        return;
    }

    else {
        std::cout << "File opened successfully" << std::endl;
    }

    std::string line;
    sampleCount = 0; 

    while (std::getline(csv, line)) {

        std::istringstream s(line);
        std::string field;

        std::vector<std::string> currentSample;
        
        while (std::getline(s, field, ',')) {

            currentSample.push_back(field);
        }

        if (sampleCount == 0) {
            tree.nFeatures = currentSample.size() -1;
        }

        sampleToData(currentSample);

        sampleCount++;
    }

    csv.close();
}

void C45_treeBuilder::sampleToData(std::vector<std::string> sample) {

    std::vector<double> currentFeatures;

    int i = 0; 

    while (i < sample.size() -1) {
        currentFeatures.push_back(std::stod(sample[i]));
        ++i;
    }

    data[i] = currentFeatures;

    if (sample[i] == "Iris-setosa") {
        labels.push_back(0);
    }
    else if (sample[i] == "Iris-versicolor") {
        labels.push_back(1);
    }
    else if (sample[i] == "Iris-virginica") {
        labels.push_back(2);
    }
}

void C45_treeBuilder::doSplits(C45_tree node, std::ofstream& out) {

    // Base cases
    if (node.data.size() <= minSz || allSame(node)) {

        node.type = LEAF;
        node.data.clear();
        node.classification = majority(node);

        return;
    }

    // Recursive case
   
    double bestGain = - DBL_MAX; 
    int bestFeature = -1;
    double bestThreshold;
    std::map<long, std::vector<double>> bestLeft;
    std::map<long, std::vector<double>> bestRight;

    for (int feat = 0; feat < tree.nFeatures; ++feat) {

        std::vector<std::pair<double, long>> sorted = makeSortedVector(node.data, feat);

        for (auto it = sorted.begin(); it != sorted.end(); ++it) {

            std::pair < std::map<long, std::vector<double>>, 
                        std::map<long, std::vector<double>>> subsets = splitData (  
                                                            node.data, 
                                                            feat, 
                                                            it->first);

            std::map<long, std::vector<double>> dataLeft = subsets.first;
            std::map<long, std::vector<double>> dataRight = subsets.second;

            double gain = getGain(node.data, dataLeft, dataRight);

            if (gain > bestGain) {

                bestGain = gain;
                bestFeature = feat;
                
                if (it == sorted.begin()) {
                    bestThreshold = it->first / 2;
                }
                else {
                    bestThreshold = ((it -1)->first + it->first) / 2;
                }

                bestLeft = dataLeft;
                bestRight = dataRight;
            }
        }
    }

    node.type = INNER;
    node.splitFeature = bestFeature;
    node.splitValue = bestThreshold;

    C45_tree leftChild;
    C45_tree rightChild;

    leftChild.nClasses = node.nClasses;
    leftChild.nFeatures = node.nFeatures;
    leftChild.data = bestLeft;
    leftChild.id = ++global_id;

    rightChild.nClasses = node.nClasses;
    rightChild.nFeatures = node.nFeatures;
    rightChild.data = bestRight;
    rightChild.id = ++global_id;

    node.left = &leftChild;
    node.right = &rightChild;
    
    node.data.clear();

    doSplits(leftChild, out);
    doSplits(rightChild, out);
}

double C45_treeBuilder::getEntropy(std::map<long, std::vector<double>> data) {

    long s = data.size();

    if (s == 0) {
        return 0;
    }

    double classSums[tree.nClasses];

    for (int i = 0; i < tree.nClasses; ++i) {
        classSums[i] = 0;
    }

    for (auto it = data.begin(); it != data.end(); ++it) {

        int cl = labels[it->first];

        for (int i = 0; i < tree.nClasses; ++i) {
            ++classSums[cl];
        }
    }

    double entropy = 0;
    
    for (int i = 0; i < tree.nClasses; ++i) {
        double csum = classSums[i];
        csum /= sampleCount;
        entropy += csum * log2(csum);
    }

    return -entropy;
}

double C45_treeBuilder::getGain(std::map<long, std::vector<double>> top,
                                std::map<long, std::vector<double>> left,
                                std::map<long, std::vector<double>> right ) {

    double eTop = getEntropy(top);
    double eLeft = getEntropy(left) * left.size() / top.size();
    double eRight = getEntropy(right) * right.size() / top.size();

    return eTop - eLeft - eRight;
}

std::vector<std::pair<double, long>> C45_treeBuilder::makeSortedVector(
                                                    std::map<long, std::vector<double>> data, 
                                                    int feature) {

    std::vector<std::pair<double, long>> vec;

    for (auto it = data.begin(); it != data.end(); ++it) {

        double value = it->second[feature];
        long idx = it->first;

        std::pair<double, int> p = std::make_pair(value, idx);

        vec.push_back(p);
    }

    std::sort(vec.begin(), vec.end());

    return vec;
}

std::pair < std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>>> C45_treeBuilder::splitData (  
                                                    std::map<long, std::vector<double>> data, 
                                                    int feature, 
                                                    double threshold) {

    std::pair < std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>>> subsets;

    for (auto it = data.begin(); it != data.end(); ++it) {

        long idx = it->first;
        double val = it->second[feature];

        if (val < threshold) {

            subsets.first[idx] = it->second;
        }
        else {
            subsets.second[idx] = it->second; 
        }
    }

    return subsets;
}

int C45_treeBuilder::majority(C45_tree node) {

    std::map<long, std::vector<double>> data = node.data;

    int classSums[node.nClasses];

    for (int i = 0; i < node.nClasses; ++i) {
        classSums[i] = 0;
    }

    for (auto it = data.begin(); it != data.end(); ++it) {
        ++classSums[labels[it->first]];
    }

    long maxSum = 0;
    int maxIdx = -1;

    for (int i = 0; i < node.nClasses; ++i) {
        
        if (classSums[i] >= maxSum) {

            maxSum = classSums[i];
            maxIdx = i;
        }
    }  

    return maxIdx;  
}

bool C45_treeBuilder::allSame(C45_tree node) {

    std::map<long, std::vector<double>> data = node.data;

    int currentClass = -1;

    for (auto it = data.begin(); it != data.end(); ++it) {

        int newClass = labels[it->first];

        if (it == data.begin()) {
            currentClass = newClass;
        }
        else {

            if (newClass != currentClass) {
                return false;
            }
        }
    }

    return true;
}























