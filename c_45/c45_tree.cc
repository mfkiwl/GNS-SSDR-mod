
#include "c45_tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <float.h>
#include <algorithm>



C45_tree::C45_tree(int nClasses, int nFeatures, int minSize, int maxDepth) 
    :   nClasses(nClasses), 
        nFeatures(nFeatures), 
        minSize(minSize),
        maxDepth(maxDepth)
{
    root = C45_node();
    nodes.push_back(root);
}

void C45_tree::printInfo() {

    std::cout << "nSamples: " << nSamples << std::endl;
    std::cout << "labels: " << labels.size() << std::endl;
    std::cout << "root Data size: " << root.data.size() << std::endl;
    std::cout << "nodes: " << nodes.size() << std::endl;
}




//////////////////
//  DATA INPUT  //
//////////////////



// THIS IMPLEMENTATION IS SPECIFIC TO TE CSV FORMAT USED BY ALARI
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

        sampleToData(sample);
        ++nSamples;
    }

    csv.close();
    std::cout << "Closed " << csvName << std::endl;
}


// THIS IMPLEMENTATION IS SPECIFIC TO TE CSV FORMAT USED BY ALARI (J48 16 features)
void C45_tree::sampleToData(std::vector<std::string> &sample) {

    std::vector<double> sampleFeatures;

    sampleFeatures.push_back(std::stod(sample[28])); // min Doppler
    sampleFeatures.push_back(sqrt(std::stod(sample[30]))); // stddev Doppler
    sampleFeatures.push_back(std::stod(sample[3])); // min valid sats
    sampleFeatures.push_back(std::stod(sample[18])); // min pseudorange
    sampleFeatures.push_back(std::stod(sample[13])); // min sig over noise
    sampleFeatures.push_back(sqrt(std::stod(sample[5]))); // stddev valid sats
    sampleFeatures.push_back(sqrt(std::stod(sample[15]))); // stddev sig over noise
    sampleFeatures.push_back(sqrt(std::stod(sample[20]))); // stddev pseudorange
    sampleFeatures.push_back(std::stod(sample[7])); // max sats changed
    sampleFeatures.push_back(std::stod(sample[9])); // avg sats changed
    sampleFeatures.push_back(std::stod(sample[19])); // avg pseudorange
    sampleFeatures.push_back(std::stod(sample[29])); // avg Doppler
    sampleFeatures.push_back(sqrt(std::stod(sample[10]))); // stddev sats changed
    sampleFeatures.push_back(std::stod(sample[12])); // max sig over noise
    sampleFeatures.push_back(std::stod(sample[22])); // max carrier phase
    sampleFeatures.push_back(std::stod(sample[14])); // avg sig ovr noise

    root.data[nSamples] = sampleFeatures;  
  
    if (sample[sample.size()-1] == "clean") {

        labels.push_back(0);
    }
    else {
        labels.push_back(1);
    }
}



/////////////////////////
//  TREE CONSTRUCTION  //
/////////////////////////



void C45_tree::buildTree(std::string csvName) {

    readCsv(csvName);

    doSplit(&root);
}


void C45_tree::doSplit(C45_node * node) {

    std::cout << "splitting node " << node->id << std::endl;

    // base cases

    if (node == nullptr) {
        return;
    }

    std::map<long, std::vector<double>> nodeData = node->data;

    if (allSameClass(nodeData) || nodeData.size() <= minSize || node->depth >= maxDepth) {
            
        makeLeaf(node);
    }

    int splitFeature = bestFeature(nodeData); 

    double splitValue = bestValue(nodeData, splitFeature);

    std::pair<  std::map<long, std::vector<double>>, 
                std::map<long, std::vector<double>> > subsets = splitData(  nodeData,
                                                                            splitFeature,
                                                                            splitValue );

    std::map<long, std::vector<double>> leftData = subsets.first;
    std::map<long, std::vector<double>> rightData = subsets.second;

    if (leftData.size() < minSize || rightData.size() < minSize) {

        makeLeaf(node->parent);
        return;
    }

    double g = gain(nodeData, leftData, rightData, splitFeature);

    if (g == 0) {

        makeLeaf(node->parent);
        return;
    }

    // recursive case

    C45_node left, right;
    
    left.parent = node;
    left.depth = node->depth +1;
    left.data = leftData;

    right.parent = node;
    right.depth = node->depth +1;
    right.data = rightData;

    node->splitFeature = splitFeature;
    node->splitValue = splitValue;
    node->left = &left;
    node->right = &right;
    node->data.clear();

    doSplit(&left);
    doSplit(&right);
}


///////////////////////////
//  AUXILIARY FUNCTIONS  //
///////////////////////////


double C45_tree::entropy(std::map<long, std::vector<double>> &data, int feat) {

    double s = 0;

    for (int cls = 0; cls < nClasses; ++cls) {

        double p = 0; 

        for (int row = 0; row < data.size(); ++row) {

            if (labels[row] == cls) {
                p++;
            }
        }

        p /= data.size();

        s += p * log2(p);
    }

    return -s;
}


double C45_tree::gain(  std::map<long, std::vector<double>> &top,
                        std::map<long, std::vector<double>> &left,
                        std::map<long, std::vector<double>> &right,
                        int feature) {

    if (left.size() == 0 || right.size() == 0) {
        return 0;
    }

    double eTop = entropy(top, feature);
    double eLeft = entropy(left, feature) * left.size() / top.size();
    double eRight = entropy(right, feature) * right.size() / top.size();

    return eTop - eLeft -eRight; 

}


int C45_tree::bestFeature(std::map<long, std::vector<double>> &data) {

    int i = 0;
    double bestEntropy = - DBL_MAX;
    int bestFeat = -1;

    while (i < nFeatures) {

        float e = entropy(data, i);

        if (e > bestEntropy) {

            bestEntropy = e;
            bestFeat = i;
        }

        ++i;
    }

    return bestFeat;
}

double C45_tree::bestValue(std::map<long, std::vector<double>> &data, int feature) {

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

        if (g > bestGain) {

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

    return bestValue;
}

std::vector<std::pair<double, long>> C45_tree::sortByValue(
                            std::map<long, std::vector<double>> &data, int feat) {

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

std::pair<  std::map<long, std::vector<double>>, 
            std::map<long, std::vector<double>> > C45_tree::splitData(
                                    std::map<long, std::vector<double>> &data, 
                                    int feat, double threshold) {

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


bool C45_tree::allSameClass(std::map<long, std::vector<double>> &data) {

    auto it = data.begin();
    int currentClass = labels[it->first];
    ++it;

    while (it != data.end() && labels[it->first] == currentClass) {
        ++it;
    }

    return it == data.end();
}


int C45_tree::majority(std::map<long, std::vector<double>> &data) {

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


void C45_tree::makeLeaf(C45_node * node) {

    node->left = nullptr;
    node->right = nullptr;

    node->classification = majority(node->data);
}

std::map<long, std::vector<double>> C45_tree::copyData(std::map<long, std::vector<double>> &data) {

    std::map<long, std::vector<double>> mapCopy;
    
    for (auto it = data.begin(); it != data.end(); ++it) {

        std::vector<double> vecCopy;

        for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
            vecCopy.push_back(*it1);
        }
        
        mapCopy[it->first] = vecCopy;
    }

    return mapCopy;
}




















