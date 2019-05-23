#include "c45_tester.h"
#include <fstream>
#include <sstream>
#include <iostream>

C45_tester::C45_tester(std::string testFileName) 
    :   correctClass(0),
        falsePositives(0),
        falseNegatives(0)
{
    readTestFile(testFileName);
}

void C45_tester::runTest(C45_clTree classifier) {

    std::cout << std::endl << "running test" << std::endl;

    for (int i = 0; i < instances.size(); ++i) {

        std::string predicted = classifier.classify(instances[i]);
        std::string trueLabel = labels[i];

        if (predicted == trueLabel) {
            ++correctClass;
        }
        else if (predicted == "spoofed" && trueLabel == "clean") {
            ++falsePositives;
        }
        else if (predicted == "clean" && trueLabel == "spoofd") {
            ++falseNegatives;
        }
    }
}

void C45_tester::printResult() {

    std::cout   << std::endl << "TEST RESULTS:" << std::endl << std::endl
                << "      Instances: " << instances.size() << std::endl
                << "        Correct: " << correctClass << std::endl
                << "False Positives: " << falsePositives << std::endl
                << "False Negatives: " << falseNegatives << std::endl
                << std::endl;
}

void C45_tester::readTestFile(std::string fName) {

    std::ifstream dataFile(fName);

    if (! dataFile.is_open()) {
        std::cout << "Error opening " << fName << std::endl;
        return;
    }
    else {
        std::cout << "Opened " << fName << std::endl;
    }

    // store headers in vector
    std::string line;
    std::getline(dataFile, line);
    std::istringstream s(line);
    std::string field;
    std::vector<std::string> headers;

    while (std::getline(s, field, ',')) {
        headers.push_back(field);        
    }

    while (std::getline(dataFile, line)) {

        std::istringstream s(line);        
        std::vector<std::string> sample;
        std::map<std::string, double> instance;

        while (std::getline(s, field, ',')) {
            sample.push_back(field);        
        }

        int i = 0;

        while (i < sample.size() -1) {

            instance[headers[i]] = std::stod(sample[i]);    
            ++i;
        }
        
        instances.push_back(instance);
        labels.push_back(sample[i]);
    }    

    dataFile.close();
    std::cout << "Closed " << fName << std::endl;
}













