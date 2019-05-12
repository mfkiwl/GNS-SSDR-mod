
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
}

void C45_tree::printInfo() {

    std::cout << "root id: " << root.id << std::endl;
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

    std::map<long, std::vector<double>> nodeData = node->data;

    if (allSameClass(nodeData) || nodeData.size() <= minSize || node->depth >= maxDepth) {
            
        makeLeaf(node);
        nodes.push_back(*node);
        return;
    }

    setParameters(node);

    if (node->leftData.size() < minSize || node->rightData.size() < minSize || node->gain == 0) {

        makeLeaf(node->parent);
        return;
    }

    // recursive case

    C45_node * left = new C45_node();
    C45_node * right = new C45_node();
    
    left->parent = node;
    left->depth = node->depth +1;
    left->data = node->leftData;

    right->parent = node;
    right->depth = node->depth +1;
    right->data = node->rightData;

    node->left = left;
    node->right = right;
    node->data.clear();

    nodes.push_back(*node);

    doSplit(left);
    doSplit(right);
}

////////////////////
//  TREE TO FILE  //
////////////////////

void C45_tree::saveTree(std::string outName) {

    std::ofstream out(outName);

    if (! out.is_open()) {
        std::cout << "Error opening " << outName << std::endl;
        return;
    }

    std::cout << "Opened " << outName << std::endl;

    for (int i = 0; i < nodes.size(); ++i) {

        std::cout << "writing node " << nodes[i].id << std::endl;
        out << nodes[i].toString() << std::endl;
    } 

    out.close();
    std::cout << "Closed " << outName << std::endl;
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

void C45_tree::setParameters(C45_node * node) {

    int i = 0;
    
    double bestGain = - DBL_MAX;
    int bestFeat = -1;
    double bestVal = - 1;
    std::map<long, std::vector<double>> bestLeft;
    std::map<long, std::vector<double>> bestRight;

    while (i < nFeatures) {

        double val = bestValue(node->data, i);

        std::pair<  std::map<long, std::vector<double>>, 
                    std::map<long, std::vector<double>> > subsets = splitData(node->data, i, val);

        std::map<long, std::vector<double>> leftData = subsets.first;
        std::map<long, std::vector<double>> rightData = subsets.second;

        double g = gain(node->data, leftData, rightData, i);

        if (g > bestGain) {

            bestFeat = i;
            bestVal = val;
            bestLeft = leftData;
            bestRight = rightData;
        }

        ++i;
    }

    node->splitFeature = bestFeat;
    node->splitValue= bestVal;
    node->leftData = bestLeft;
    node->rightData = bestRight;
    node->gain = bestGain;
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
    node->data.clear();
    node->classification = majority(node->data);
}




















