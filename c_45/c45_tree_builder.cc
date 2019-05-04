
#include "c45_tree_builder.h"
#include "c45_tree.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <utility> 

C45_tree C45_treeBuilder::buildTree(std::string fName, int nClasses, int maxDepth, int minSize) {

    maxDpt = maxDepth;
    minSz = minSize;
    tree.nClasses = nClasses;

    readCsv(fName);

    return tree;
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

void C45_treeBuilder::setSplit(std::map<long, std::vector<double>> data) {
   
    int bestGain; 
    int bestFeature = -1;
    double bestThreshold;

    std::map<long, std::vector<double>> left;
    std::map<long, std::vector<double>> right;

    for (int i = 0; i < tree.nFeatures; ++i) {

        std::vector<std::map<long, std::vector<double>>> sorted = makeSortedVector(data, i);

        // TODO implement splits and evaluate gains
    }
}

double C45_treeBuilder::getEntropy(std::map<long, std::vector<double>> data) {

    long s = data.size();

    if (s == 0) {
        return 0;
    }

    double classSums[tree.nClasses];

    for (auto it = data.begin(); it != data.end()) {

        int cl = labels[*it];

        for (int i = 0; i < tree.nClasses; ++i) {
            ++classSums[cl];
        }
    }

    double entropy = 0;
    
    for (int i = 0; i < tree.nClasses) {
        double csum == classSums[i];
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

void C45_treeBuilder::addNode() {

}

std::vector<std::map<long, std::vector<double>>> C45_treeBuilder::makeSortedVector(
                                                    std::map<long, std::vector<double>> data, 
                                                    int feature) {

    std::vector<pair> vec;

    for (auto it = data.begin(); it != data.end(); ++it) {

        std::map<long, std::vector<double> item = it->second;
        double value = item.second[feature]

        std::pair<double, std::map<long, std::vector<double>> p;
        p = std::make_pair(value, item);

        vec.push_back(p);
    }

    std::sort(vec.begin(), vec.end());

    return vec;
}


























