
#include "c45_tree_builder.h"
#include "c45_tree.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

C45_tree C45_treeBuilder::buildTree(std::string fName) {

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

    std::cout << data.size() << " data samples" << std::endl;
    std::cout << labels.size() << " labels" << std::endl;
}

void C45_treeBuilder::sampleToData(std::vector<std::string> sample) {

    std::vector<double> currentFeatures;

    int i = 0; 

    while (i < sample.size() -1) {
        currentFeatures.push_back(std::stod(sample[i]));
        ++i;
    }

    data.push_back(currentFeatures);

    if (sample[i] == "Iris-setosa") {
        labels.push_back(SETOSA);
    }
    else if (sample[i] == "Iris-versicolor") {
        labels.push_back(VERSICOLOR);
    }
    else if (sample[i] == "Iris-virginica") {
        labels.push_back(VIRGINICA);
    }
    
}













