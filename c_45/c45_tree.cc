
#include "c45_tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <float.h>
#include <algorithm>

long idCounter = 0;

C45_tree::C45_tree( int nClasses, int nFeatures, int minSize, std::string csvName) 
    : nClasses(nClasses), nFeatures(nFeatures), minSize(minSize)
{

    id = idCounter++;

    C45_tree::readCsv(csvName);
}

C45_tree::C45_tree(int nClasses, int nFeatures, int minSize) 
    : nClasses(nClasses), nFeatures(nFeatures), minSize(minSize)
{
    id = idCounter++;
}

// INFO/PRINTS

void C45_tree::printInfo() {

    std::cout   << std::endl << "id: " << id << std::endl
                << "classes: " << nClasses << std::endl
                << "features: " << nFeatures << std::endl
                << "minsize: " << minSize << std::endl
                << "split feature: " << splitFeature << std::endl
                << "split value: " << splitValue << std::endl
                << "data size: " << data.size() << std::endl
                << "labels size: " << labels.size() << std:: endl
                << "left child: ";

    if (left != nullptr) {
        std::cout << left->id << std::endl;
    }
    else {
        std::cout << "none" << std::endl;
    }

    std::cout << "right child: ";

    if (left != nullptr) {
        std::cout << right->id << std::endl;
    }
    else {
        std::cout << "none" << std::endl;
    }

    std::cout << "classified as: " << classification << std::endl;
}

bool C45_tree::isRoot() {
    return parent == nullptr;
}

bool C45_tree::isLeaf() {

    return left == nullptr && right == nullptr;
}

// READ CSV

void C45_tree::readCsv(std::string csvName) {

    std::ifstream csv(csvName);

    if (! csv.is_open()) {
        std::cout << "Error opening " << csvName << std::endl;
        return;
    }
    else {
        std::cout << "Opened " << csvName << std::endl;
    }

    std::string line;

    while (std::getline(csv, line)) {

        std::istringstream s(line);
        std::string field;
        std::vector<std::string> sample;

        while (std::getline(s, field, ',')) {
            sample.push_back(field);        
        }

        if (nSamples == 0) {
            nFeatures = sample.size() -1;
        }

        sampleToData(sample);

        ++nSamples;
    }

    csv.close();
    std::cout << "Closed " << csvName << std::endl;
}

void C45_tree::sampleToData(std::vector<std::string> sample) {

    std::vector<double> sampleFeatures;

    int i = 0;

    while (i < nFeatures) {
        sampleFeatures.push_back(std::stod(sample[i])); 
        ++i;
    }

    data[nSamples] = sampleFeatures;

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

// TREE CONSTRUCTION

void C45_tree::buildTree(std::string outName) {

    std::ofstream out(outName);

    if (! out.is_open()) {
        std::cout << "Error opening " << outName << std::endl;
        return;
    }

    std::cout << "opening " << outName << std::endl;

    doSplits(this, out);

    out.close();

    std::cout << "closed " << outName << std::endl;
}

void C45_tree::doSplits(C45_tree *node, std::ofstream& out) {

    setSplitFeature(node, node->data);
    setSplitValue(node, node->data, node->splitFeature);

    std::pair<  std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>> > subsets = splitData(  node->data,
                                                                            node->splitFeature,
                                                                            node->splitValue );

    std::map<long, std::vector<double>> leftData = subsets.first;
    std::map<long, std::vector<double>> rightData = subsets.second;

    double g = gain(node->data, leftData, rightData, node->splitFeature);

    if (g == 0) {
        node->classification = majority(node->data);
        return;
    }

    int as = allSame();

    if (as != -1) {
        node->classification = as;
        return;
    }

    if (leftData.size() <= minSize && rightData.size() > minSize) { 

        if (leftData.size() > 0) {

            C45_tree l = makeLeaf(node, leftData); 
            node->left = &l;
        } 
    
        C45_tree r = makeNode(node, rightData);
        node->right = &r;

        doSplits(&r, out);
    }
    else if (rightData.size() <= minSize && leftData.size() > minSize) {

        if (rightData.size() > 0) {

            C45_tree r = makeLeaf(node, leftData); 
            node->right = &r;
        } 

        C45_tree l = makeNode(node, leftData);
        node->left = &l;

        doSplits(&l, out);
    }
    else if (rightData.size() < minSize && leftData.size() < minSize) {

        node->classification = majority(node->data);
        return;
    }
    else {

        C45_tree l = makeNode(node, leftData);
        C45_tree r = makeNode(node, rightData);

        node->left = &l;
        node->right = &r;

        doSplits(&l, out);
        doSplits(&r, out);
    }
}


// AUX

double C45_tree::entropy(std::map<long, std::vector<double>> data, int feat) {

    double s = 0;

    for (int cls = 0; cls < nClasses; ++cls) {

        double p = 0; 

        for (int row = 0; row < nSamples; ++row) {

            if (labels[row] == cls) {
                p += data[row][feat];
            }
        }

        p /= nSamples;

        s += p * log2(p);
    }

    return -s;
}

void C45_tree::setSplitFeature(C45_tree *node, std::map<long, std::vector<double>> data) {

    int i = 0;
    double bestEntropy = - DBL_MAX;
    double bestFeat = -1;

    while (i < nFeatures) {

        float e = entropy(data, i);

        if (e > bestEntropy) {

            bestEntropy = e;
            bestFeat = i;
        }

        ++i;
    }


    node->splitFeature = bestFeat;
}

void C45_tree::setSplitValue(C45_tree *node, std::map<long, std::vector<double>> data, int feature) {

    double bestGain = - DBL_MAX;
    double bestValue = -1;
    std::map<long, std::vector<double>> bestLeft;
    std::map<long, std::vector<double>> bestRight;

    std::vector<std::pair<double, long>> sorted = sortByValue(data, feature);

    for (auto it = sorted.begin(); it != sorted.end(); ++it) {

        std::pair < std::map<long, std::vector<double>>, 
                    std::map<long, std::vector<double>>> subsets = splitData (  
                                                            data, 
                                                            feature, 
                                                            it->first);

        std::map<long, std::vector<double>> dataLeft = subsets.first;
        std::map<long, std::vector<double>> dataRight = subsets.second;

        double g = gain(data, dataLeft, dataRight, feature);

        if (g >= bestGain) {

            bestGain = g;  
            bestLeft = dataLeft;
            bestRight = dataRight;    

            if (it == sorted.begin()) {
                bestValue = it->first / 2;
            }
            else {
                bestValue = ((it -1)->first + it->first) / 2;
            }   
        }
    }
    
    node->splitValue = bestValue;
}

std::pair<  std::map<long, std::vector<double>>, 
            std::map<long, std::vector<double>> > C45_tree::splitData(
                                    std::map<long, std::vector<double>>
                                    data, int feat, double threshold) {

    std::pair < std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>>> subsets;

    for (auto it = data.begin(); it != data.end(); ++it) {

        long idx = it->first;
        double val = it->second[feat];

        if (val < threshold) {

            subsets.first[idx] = it->second;
        }
        else {
            subsets.second[idx] = it->second; 
        }
    }

    return subsets;
}

std::vector<std::pair<double, long>> C45_tree::sortByValue(
                    std::map<long, std::vector<double>> data, int feat) {

    std::vector<std::pair<double, long>> vec;

    for (auto it = data.begin(); it != data.end(); ++it) {

        double value = it->second[feat];
        long idx = it->first;

        std::pair<double, long> p = std::make_pair(value, idx);

        vec.push_back(p);
    }

    std::sort(vec.begin(), vec.end());

    return vec;   
}

double C45_tree::gain(  std::map<long, std::vector<double>> top,
                        std::map<long, std::vector<double>> left,
                        std::map<long, std::vector<double>> right,
                        int feature) {

    double eTop = entropy(top, feature);
    double eLeft = entropy(left, feature) * left.size() / top.size();
    double eRight = entropy(right, feature) * right.size() / top.size();

    return eTop - eLeft - eRight;    
}

std::string C45_tree::makeString() {

    std::string leftId;
    std::string rightId;
    std::string classif;

    if (left == nullptr) {
        leftId = "_";
    }
    else {
        leftId = std::to_string(left->id);
    }

    if (right == nullptr) {
        rightId = "_";
    }
    else {
        rightId = std::to_string(right->id);
    }

    if (classification == -1) {
        classif = "_";
    }
    else {
        classif = std::to_string(classification);
    }

    std::string s = std::to_string(id) + "," +
                    std::to_string(splitFeature) + "," +
                    std::to_string(splitValue) + "," +
                    leftId + "," +
                    rightId + "," +
                    classif;

    return s;
}

C45_tree C45_tree::makeNode(C45_tree *parent, std::map<long, std::vector<double>> data) {

    C45_tree node(parent->nClasses, parent->nFeatures, parent->minSize);
    node.nSamples = parent->nSamples;
    node.labels = parent->labels;
    node.data = data;
    node.parent = parent;

    return node;
}

C45_tree C45_tree::makeLeaf(C45_tree *parent, std::map<long, std::vector<double>> data) {

    C45_tree node(parent->nClasses, parent->nFeatures, parent->minSize);
    node.parent = parent;
    std::vector<int> labels = parent->labels;

    node.classification = majority(data);

    return node;
}

int C45_tree::majority(std::map<long, std::vector<double>> data) {

    int classSums[nClasses];

    for (auto it = data.begin(); it != data.end(); ++it) {
        ++classSums[labels[it->first]];
    }

    int maxSum = -1;
    int majority = -1;

    for (int i = 0; i < nClasses; ++i) {
        
        if (classSums[i] >= maxSum) {

            maxSum = classSums[i];
            majority = i;
        }
    }

    return majority;
}

int C45_tree::allSame() {

    int currentClass = -1;

    for (auto it = data.begin(); it != data.end(); ++it) {

        int newClass = labels[it->first];

        if (it == data.begin()) {

            currentClass = newClass;

            if (data.size() == 1) {
                return currentClass;
            }
        }
        else {

            if (newClass != currentClass) {
                return -1;
            }
        }
    }

    return majority(data);
}












