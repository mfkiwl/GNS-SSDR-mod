
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
}


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


void C45_tree::buildTree(std::string csvName) {

    readCsv(csvName);
}


















